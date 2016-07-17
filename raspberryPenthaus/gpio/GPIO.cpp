#include <exception>
#include <algorithm>
#include <fstream>
#include <functional>
#include "GPIO.h"

using namespace gpio;
using namespace std;

GPIO* GPIO::_instance = nullptr;

GPIO* GPIO::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new GPIO;
	}
	return _instance;
}

void GPIO::TurnOff()
{
	if (_instance != nullptr)
	{
		delete this;
		_instance = nullptr;
	}
}

GPIO::GPIO()
{
	_checkCounter = 0;
}

GPIO::~GPIO()
{
	WriteDefaultAll();
	for (auto& dev : _devices)
	{
		delete dev.second;
	}
}

void GPIO::AddDevice(IDevice* dev)
{
	if (dev == nullptr)
		throw runtime_error("Attempt to add nullptr to GPIO");
	auto temp = _devices.find(dev->GetID());
	if (temp != _devices.end())
		throw runtime_error("Attemp to add device with non-unique ID");
	_devices[dev->GetID()] = dev;
}

void GPIO::AddNewDevice(IDevice* dev)
{
	if (dev == nullptr)
		throw runtime_error("Attempt to add nullptr to GPIO");
	auto temp = _devices.find(dev->GetID());
	if (temp != _devices.end())
		throw runtime_error("Attemp to add device with non-unique ID");
	temp = _newDevices.find(dev->GetID());
	if (temp != _newDevices.end())
		throw runtime_error("Attemp to add device with non-unique ID");
	_newDevices[dev->GetID()] = dev;
}

IDevice* GPIO::GetDevice(int id)
{
	auto temp = _devices.find(id);
	if (temp == _devices.end())
	{
		temp = _newDevices.find(id);
		if (temp == _newDevices.end())
			return nullptr;
	}
	return (*temp).second;
}

void GPIO::SetupGPIO()
{
	//konfiguracja GPIO
	wiringPiSetup();

	for (auto& dev : _devices)
	{
		dev.second->Setup();
		unsigned int interval = dev.second->GetCheckInterval();
		if (interval > 0)
		{
			_checkTable.push_back(
				std::make_pair(interval, dev.second));
		}
	}
}

void GPIO::CheckAll()
{
	for (auto& pair : _checkTable)
	{
		if (_checkCounter % pair.first == 0)
		{
			pair.second->Check();
		}
	}
	++_checkCounter;
}

void GPIO::ReadDefaultAll()
{
	for (auto& dev : _devices)
	{
		dev.second->ReadDefault();
	}
}

void GPIO::WriteDefaultAll()
{
	for (auto& dev : _devices)
	{
		dev.second->WriteDefault();
	}
}

string GPIO::PrintDevices()
{
	string s("Plugged devices:\n\n");
	for (auto& dev : _devices)
	{
		s += dev.second->ToString() + "\n";
	}
	if (_newDevices.size() > 0)
	{
		s += "\nNew devices:\n";
		for (auto& dev : _newDevices)
		{
			s += dev.second->ToString() + "\n";
		}
		
	}
	s += "\n";
	return s;
}

void GPIO::SaveDevices(string& filename)
{
	map<int, IDevice*>
		devicesToSave(_devices);
	for (auto& dev : _newDevices)
	{
		devicesToSave.insert(dev);
	}

	ofstream file(filename, ofstream::trunc);
	for (auto& dev : devicesToSave)
	{
		file << "create ";
		dev.second->Save(file);
		file << endl;
	}
	file.close();
}