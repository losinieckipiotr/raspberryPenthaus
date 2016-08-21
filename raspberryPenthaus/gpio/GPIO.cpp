#include <exception>
#include <algorithm>
#include <fstream>
#include <functional>

#include "GPIO.h"
#include "../device/IDevice.h"
#include "../device/DeviceManager.h"
#include "../device/IWriteable.h"

#include "../config.h"
#ifndef WP
void wiringPiSetup()
{

}
#else
#include <wiringPi.h>
#endif // WP

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

}

GPIO::~GPIO()
{

}

void GPIO::SetupGPIO()
{
	wiringPiSetup();

	auto devs =  man.GetAllDevices();
	for (auto &dev : devs)
	{
		dev->Setup();
	}
}

void GPIO::WriteDefaultAll()
{
	auto writeDevs = man.GetWriteDevices();
	for (auto &dev : writeDevs)
	{
		dev->WriteDefault();
	}
}
