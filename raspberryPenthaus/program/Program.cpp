#include <thread>
#include <exception>
#include <string>
#include <sstream>
#include <deque>
#include <fstream>

#include "Program.h"
#include "../device/IDevice.h"
#include "../wp.h"

#ifdef STATS
std::deque<long long> statistics;
#endif

using namespace program;
using namespace gpio;
using namespace rule;
using namespace std;
using namespace chrono;

Program* Program::_instance = nullptr;

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
	:	_devsFile("files\\devices.txt"),
		_rulesFile("files\\rules.txt"),
		_eventsFile("files\\events.txt"),
		_commander(_deviceManager),
		_creator(_deviceManager, _ruleManager),
		_bus(GPIO::Instance(_deviceManager)),
		_io(io::IO::Instance())
{

}

//destruktor, wylacza urzadzenia podlaczone do GPIO
Program::~Program()
{
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
			//tworzenie obiektow podlaczonyych do GPIO
			_creator.DevicesFromFile(_devsFile);
		}
		catch (runtime_error& re)
		{
			_io->ErrorOutput(re.what());
		}
		try
		{
			//tworzenie regul zapalania i wylaczania swiatla
			_creator.RulesFromFile(_rulesFile);
		}
		catch (runtime_error& re)
		{
			_io->ErrorOutput(re.what());
		}
		try
		{
			//dodawanie triggerow i akcji
			_creator.EventsFromFile(_eventsFile);
		}
		catch (runtime_error& re)
		{
			_io->ErrorOutput(re.what());
		}
		//konfiguracja GPIO
		_Setup();
	}
	catch (exception& ex)
	{
		_io->ErrorOutput(ex.what());
		return false;
	}
	return true;
}

//glowna petla programu sterujacego
void Program::CoreLoop()
{
	try
	{
		//ustawienie flagi wyjscia
		_coreQuit = false;
		//inicjalizacja okresu petli
		const auto tick = milliseconds(CHECKING_INTERVAL);
		//wyliczenie czasu nastepnej iteracji
		auto nextTime = system_clock::now() + tick;
		//petla glowna sterowania
		while (!_coreQuit)
		{
			//odczyt czujnikow i wykonanie polecen
			_CheckAndExecute();
			//sprawdzenie opoznienia i odczekanie czasu do nastepnej iteracji
			_CheckDelay(nextTime);
			//wyliczenie czasu nastepnej iteracji
			nextTime = system_clock::now() + tick;
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

//konfiguracja GPIO
void Program::_Setup()
{
	//setup urzadzen podlaczonych do GPIO
	_bus->SetupGPIO();
	//opoznienie, dla inicjalizacji urzadzen
	this_thread::sleep_for(chrono::milliseconds(500));
}

//tutaj nastepuje odczytanie stanu czujnikow
//oraz wykonanie ustawionych polecen wedlug regul
void Program::_CheckAndExecute()
{
	try
	{
		lock_guard<mutex> lck(_program_mutex);

		//TYMCZASOWE WYLACZENIE
		this_thread::sleep_for(chrono::milliseconds(10));

		/*_bus->CheckAll();
		_ruleManager.ExecuteRules();*/
	}
	catch (exception& ex)
	{
		_io->ErrorOutput(string(ex.what()));
	}
}

//zapisanie konfiguracji do plikow
void Program::_SaveAll()
{
	try
	{
		_deviceManager.SaveDevices(_devsFile);
		_ruleManager.SaveRules(_rulesFile);
		_ruleManager.SaveEvents(_eventsFile);
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
	_coreQuit = true;
}

//metoda tworzy nowe zdarzenie(trigger albo akcje)
//nowe urzadzenie,
//przy wyjsciu z programu zostanie zapisane do pliku
//konfiguracyjnego i przy ponownym uruchomieniu bedzie dzialac
string Program::Add(string& line)
{
	string reply;
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
	return reply;
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
			Rule* rule = _creator.CreateRule(line);
			if (rule != nullptr)
			{
				lock_guard<mutex> lck(_program_mutex);
				_ruleManager.AddRule(rule);
				reply = "Created!";
			}
			else
			{
				reply = "Syntax error";
			}
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
		_ruleManager.ClearRules();
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
		_ruleManager.ClearEvents();
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
		_ruleManager.ClearRules();
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
	return _ruleManager.PrintRules();
}

string Program::GetEventsString()
{
	lock_guard<mutex> lck(_program_mutex);
	return _ruleManager.PrintEvents();
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
