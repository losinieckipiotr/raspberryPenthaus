#include <exception>
#include <algorithm>
#include <fstream>
#include <functional>

#include "GPIO.h"
#include "../device/IDevice.h"
#include "../device/DeviceManager.h"
#include "../device/IWriteable.h"

using namespace gpio;
using namespace std;

GPIO* GPIO::_instance = nullptr;

GPIO* GPIO::Instance(device::DeviceManager &man)
{
	if (_instance == nullptr)
	{
		_instance = new GPIO(man);
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

GPIO::GPIO(device::DeviceManager &man) : man(man)
{
	//_checkCounter = 0;
}

GPIO::~GPIO()
{
	/*WriteDefaultAll();

	for (auto& dev : _devices)
	{
		delete dev.second;
	}*/
}

//void GPIO::AddDevice(IDevice* dev)
//{
//	if (dev == nullptr)
//		throw runtime_error("Attempt to add nullptr to GPIO");
//	auto temp = _devices.find(dev->GetID());
//	if (temp != _devices.end())
//		throw runtime_error("Attemp to add device with non-unique ID");
//	_devices[dev->GetID()] = dev;
//}

//void GPIO::AddNewDevice(IDevice* dev)
//{
//	if (dev == nullptr)
//		throw runtime_error("Attempt to add nullptr to GPIO");
//	auto temp = _devices.find(dev->GetID());
//	if (temp != _devices.end())
//		throw runtime_error("Attemp to add device with non-unique ID");
//	temp = _newDevices.find(dev->GetID());
//	if (temp != _newDevices.end())
//		throw runtime_error("Attemp to add device with non-unique ID");
//	_newDevices[dev->GetID()] = dev;
//}
//
//IDevice* GPIO::GetDevice(int id)
//{
//	auto temp = _devices.find(id);
//	if (temp == _devices.end())
//	{
//		temp = _newDevices.find(id);
//		if (temp == _newDevices.end())
//			return nullptr;
//	}
//	return (*temp).second;
//}

void GPIO::SetupGPIO()
{
	wiringPiSetup();

	auto devs =  man.GetAllDevices();
	for (auto &dev : devs)
	{
		dev->Setup();
	}

	//	/*if (interval > 0)
	//	{
	//		_checkTable.push_back(
	//			std::make_pair(interval, dev.second));
	//	}*/
	//}
}

//void GPIO::CheckAll()
//{
//	for (auto& pair : _checkTable)
//	{
//		if (_checkCounter % pair.first == 0)
//		{
//			pair.second->Check();
//		}
//	}
//	++_checkCounter;
//}
//
//void GPIO::ReadDefaultAll()
//{
//	for (auto& dev : _devices)
//	{
//		dev.second->ReadDefault();
//	}
//}

void GPIO::WriteDefaultAll()
{
	auto writeDevs = man.GetWriteDevices();
	for (auto &dev : writeDevs)
	{
		dev->WriteDefault();
	}
}