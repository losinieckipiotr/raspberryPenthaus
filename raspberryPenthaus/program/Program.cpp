#include <thread>
#include <exception>
#include <string>
#include <sstream>
#include <deque>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "Program.h"
#include "../wp.h"

#include "../event/LightDetected.hpp"
#include "../event/MotionDetected.hpp"

#ifdef STATS
std::deque<long long> statistics;
#endif

using namespace program;
using namespace gpio;
using namespace std;
using namespace chrono;
using namespace event;

namespace pt = boost::property_tree;

Program* Program::_instance = nullptr;
ItemsPool<event::eventPtr> Program::_eventPool;

//wzorzec Singleton (statyczny konstruktor)
Program* Program::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new Program;
	}
	return _instance;
}

//wzorzec Singleton (statyczny destruktor)
void Program::ExitProgram()
{
	if (_instance != nullptr)
	{
		delete this;
		_instance = nullptr;
	}
}

//konstruktor, inicjuje sciezki do plikow konfiguracyjnych
Program::Program()
	:	_serializeFile("files\\serialize.xml"),
		_deviceReader(_deviceManager),
		_commander(_deviceManager),
		_creator(_deviceManager),
		_bus(GPIO::Instance(_deviceManager)),
		_io(io::IO::Instance())
{

}

//destruktor, zwolnienie pamieci dla obiektow drivers,
//usuniecie GPIO ktory tez jest singletonem
Program::~Program()
{
	for (auto &driver : _drivers)
	{
		delete driver;
	}
	_drivers.clear();

	//usuniecie obiektu GPIO
	_bus->TurnOff();

	#ifdef STATS
	ofstream file("files\\statistics.txt", ios::trunc);
	for (auto& numb : statistics)
	{
		file << numb << "\n";
	}
	file.close();
	#endif
}

//inicjalizacja - czytanie plikow konfiguracyjnych,
//tworzenie obiektow, konfiguracjia GPIO
bool Program::Init()
{
	try
	{
		try
		{
			_drivers =  _creator.DriversFromFile(_serializeFile);
		}
		catch (runtime_error& re)
		{
			_io->ErrorOutput(re.what());
		}

		//konfiguracja potrzebnych obiektow
		_Setup();
	}
	catch (exception& ex)
	{
		_io->ErrorOutput(ex.what());
		return false;
	}
	return true;
}

//konfiguracja listy zobowiazan driverow, 
//pogrupowanie urzadzen do czytania w DeviceReader,
//ustawienie GPIO(wywolanie wiringPiSetup),
//probne odczyty z urzadzen
void Program::_Setup()
{
	auto it = _drivers.begin();
	auto nextDriv = (++_drivers.begin());
	auto endList = _drivers.end();
	for (; it != endList; ++it)
	{
		if (nextDriv != endList)
		{
			(*it)->AddHandler(*nextDriv);
			++nextDriv;
		}
		else
			break;
	}

	_deviceReader.BiuldDeviceMap();
	//setup urzadzen podlaczonych do GPIO
	_bus->SetupGPIO();
	//opoznienie, dla inicjalizacji urzadzen
	this_thread::sleep_for(chrono::milliseconds(500));
}

//glowna petla programu sterujacego,
//sterowanie to 1 watek odbierajacy eventy gdy tylko jest dostepny
//eventy powstaja w kilku watkach czytajacych z urzadzen
//obsluga evenow przez lancuch  zobowiazan - poczatek lancucha to 1 driver
//TO DO: stworzyc driver konczacy lancuch ktroy odbierze wszystkie eventy
//i wyswietli blad o nieobsluzonym evencie
void Program::CoreLoop()
{
	try
	{
		//start watkow czytajacych z urzadzen(tworzacych eventy)
		_deviceReader.StartRead();
		//ustawienie flagi wyjscia
		_coreQuit = false;
		//sprawdzenie czy istnieje co najmniej 1 driver
		if (!_drivers.size())
			throw logic_error("No drivers to handle events");
		auto beginOfChain = _drivers.front();
		//petla glowna sterowania
		while (!_coreQuit)
		{
			try
			{
				beginOfChain->HandleEvent(_eventPool.Pop());
			}
			catch (runtime_error& ex)
			{
				string s = ex.what();
				if (s != "Pool has been closed.")
					io::StdIO::ErrorOutput(ex.what());
			}
		}

		//nastapilo normalne zamkniecie aplikacji
		lock_guard<mutex> lck(_program_mutex);
		//zapisanie stanu do plikow konfiguracyjncyh
		_SaveAll();
		//zapisanie wartosci domyslnych do urzadzen
		_bus->WriteDefaultAll();
		//usuniecie wszystkich obiektow urzadzen(zwalnianie pamieci)
		_deviceManager.DeleteAllDevices();
		//zamkniecie serwera
		_io->StopIO();
	}
	catch (exception& ex)
	{
		_io->ErrorOutput(ex.what());
	}
}

//zapisanie konfiguracji do pliku
void Program::_SaveAll()
{
	try
	{
		pt::ptree tree;
		string path = "serialize.drivers";
		tree.put(path, "");
		path.append(1, '.');
		for (auto &driver : _drivers)
		{
			driver->SaveToTree(tree, path);
		}
		pt::write_xml(_serializeFile, tree);
	}
	catch (exception& ex)
	{
		_io->ErrorOutput(string(ex.what()));
	}
}

//wlaczenie serwera jezeli wystapi blad, aplikacja jest wylaczana
void Program::IO()
{
	try
	{
		//TYMCZASOWE WYLACZENIE
		//_io->StartIO();
	}
	catch (exception& ex)
	{
		_io->ErrorOutput(string(ex.what()));
		StopAll();
	}
}

//ustawienie flagi wyjscia z programu
void Program::StopAll()
{
	_deviceReader.StopRead();
	_eventPool.Close();
	_coreQuit = true;
}

ItemsPool<std::shared_ptr<event::IEvent>>& program::Program::GetEventPool()
{
	return _eventPool;
}

//metoda tworzy nowe zdarzenie(trigger albo akcje)
//przy wyjsciu z programu zostanie zapisane do pliku
//konfiguracyjnego i przy ponownym uruchomieniu bedzie dzialac
string Program::Add(string& line)
{
	/*string reply;
	try
	{
		lock_guard<mutex> lck(_program_mutex);
		reply = _creator.CreateEvents(line);
	}
	catch (exception& ex)
	{
		reply = ex.what();
		_io->ErrorOutput(reply);
	}
	return reply;*/

	return "No implementation exception";
}

//metoda implementuje funkcjonalnosc dodawanie nowych urzadzen
//oraz zasad podczas dzialania aplikacji
string Program::Create(string& line)
{
	string reply;
	try
	{
		string buffer;
		stringstream str(line);
		str >> buffer >> buffer;
		//dodawanie urzadzen
		if (buffer != "Rule")
		{
			device::IDevice* dev = _creator.CreateDevice(line);
			if (dev != nullptr)
			{
				lock_guard<mutex> lck(_program_mutex);
				_deviceManager.AddDevice(dev);
				//jawny Setup
				dev->Setup();
				reply = "Created!";
			}
			else
			{
				reply = "Syntax error";
			}
		}
		//dodawanie "Rule"
		else
		{
			//Rule* rule = _creator.CreateRule(line);
			//if (rule != nullptr)
			//{
			//	lock_guard<mutex> lck(_program_mutex);
			//	_ruleManager.AddRule(rule);
			//	reply = "Created!";
			//}
			//else
			//{
			//	reply = "Syntax error";
			//}

			reply = "Syntax error";
		}
	}
	catch (exception& ex)
	{
		reply = ex.what();
		_io->ErrorOutput(reply);
	}
	return reply;
}

string Program::Execute(string& line)
{
	string reply;
	try
	{
		lock_guard<mutex> lck(_program_mutex);
		reply = _commander.ExecuteCommand(line);
	}
	catch (exception& ex)
	{
		reply = ex.what();
		_io->ErrorOutput(reply);
	}
	return reply;
}

string Program::ClearAll()
{
	string reply;
	lock_guard<mutex> lck(_program_mutex);
	try
	{
		//utworzenie nowego
		//_ruleManager.ClearRules();

		_deviceManager.DeleteAllDevices();
		reply = "All cleared!";
	}
	catch (exception& ex)
	{
		reply = ex.what();
		_io->ErrorOutput(reply);
	}

	return reply;
}

string Program::ClearEvents()
{
	string reply;
	try
	{
		lock_guard<mutex> lck(_program_mutex);
		//_ruleManager.ClearEvents();
		reply = "Events cleared!";
	}
	catch (exception& ex)
	{
		reply = ex.what();
		_io->ErrorOutput(reply);
	}
	return reply;
}

string Program::ClearRules()
{
	string reply;
	try
	{
		lock_guard<mutex> lck(_program_mutex);
		//_ruleManager.ClearRules();
		reply = "Rules cleared!";
	}
	catch (exception& ex)
	{
		reply = ex.what();
		_io->ErrorOutput(reply);
	}
	return reply;
}

string Program::GetDevsString()
{
	lock_guard<mutex> lck(_program_mutex);
	return _deviceManager.PrintDevices();
}

string Program::GetRulesString()
{
	lock_guard<mutex> lck(_program_mutex);
	//return _ruleManager.PrintRules();
	return "No implementation exception";
}

string Program::GetEventsString()
{
	lock_guard<mutex> lck(_program_mutex);
	//return _ruleManager.PrintEvents();
	return "No implementation exception";
}

void Program::_CheckDelay(time_point<system_clock,
	system_clock::duration> nextTime)
{
	auto now = system_clock::now();
	#ifdef STATS
	auto diff = nextTime - now;
	statistics.push_back(duration_cast<milliseconds>(diff).count());
	#endif
	if (nextTime <= now)
		return;
	else
	{
		this_thread::sleep_for(nextTime - now);
	}
}
