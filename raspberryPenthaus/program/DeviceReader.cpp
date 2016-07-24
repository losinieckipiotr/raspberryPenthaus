#include <iostream>
#include <stdexcept>
#include <sstream>
#include <chrono>

#include "../wp.h"
#include "DeviceReader.h"
#include "../device/IDevice.h"

#include "../event/LightDetected.hpp"
#include "../event/MotionDetected.hpp"

#include "../io/StdIO.h"

using namespace program;
using namespace device;
using namespace event;
using namespace std;
using namespace chrono;

DeviceReader::DeviceReader(DeviceManager &devMan)
	:	devMan_(devMan), baseInterval(BASE_INTERVAL), readFlag(false)
{

}

DeviceReader::~DeviceReader()
{

}

void DeviceReader::BiuldDeviceMap()
{
	StopRead();

	intrvMap_.clear();
	auto readDevs = devMan_.GetReadDevices();
	for (auto &dev : readDevs)
	{
		unsigned int i = dev->GetReadInterval();
		if (i < baseInterval)
			throw logic_error("Device read shorter smaller than base interval");
		if (intrvMap_.find(i) != intrvMap_.end())
		{
			intrvMap_[i].push_back(dev);
		}
		else
		{
			vector<IReadable*> sameIntervalDevs;
			sameIntervalDevs.push_back(dev);
			intrvMap_[i] = sameIntervalDevs;
		}
	}
}

//void DeviceReader::ReadAll()
//{
//	for (auto &elm : intrvMap_)
//	{
//		unsigned int i = elm.first;
//		if (readCounter % i == 0)
//		{
//			auto &devs = elm.second;
//			for (auto &dev : devs)
//			{
//				ReadDev(dev);
//			}
//		}
//	}
//	++readCounter;
//}

void DeviceReader::StartRead()
{
	readFlag = true;
	for (auto &elm : intrvMap_)
	{
		auto loop = [this, &elm]() { ReadLoop(elm.first, elm.second); };
		readThreads.push_back(thread(loop));
	}
}

void DeviceReader::StopRead()
{
	readFlag = false;
	for (auto &th : readThreads)
	{
		th.detach();
	}
	readThreads.clear();
}

void program::DeviceReader::ReadLoop
	(unsigned int interval,
	std::vector<device::IReadable*>& devs)
{
	const auto tick = milliseconds(interval);
	time_point<system_clock> now;
	time_point<system_clock> nextTime = system_clock::now() + tick;
	while (readFlag)
	{
		for (auto &dev : devs)
		{
			ReadDev(dev);
		}

		time_point<system_clock> now = system_clock::now();
		if (nextTime > now)
			this_thread::sleep_for(nextTime - now);
		nextTime = system_clock::now() + tick;
	}
}

void program::DeviceReader::ReadDev(device::IReadable *dev)
{
	//TYMCZASOWO WYPISYWANIE WARTOSCI NA EKRAN

	auto readEvent = dev->Read();
	if (readEvent == nullptr)
		return;

	MotionDetected* motionPtr = dynamic_cast<MotionDetected*>(readEvent.get());
	if (motionPtr)
	{
		io::StdIO::StandardOutput(motionPtr->ToString());
		return;
	}

	LightDetected* lightPtr = dynamic_cast<LightDetected*>(readEvent.get());
	if (lightPtr)
	{
		io::StdIO::StandardOutput(lightPtr->ToString());
		return;
	}

	throw runtime_error("Unexpected exception");


	//MotionSensor* motSens = dynamic_cast<MotionSensor*>(dev);
	//if (motSens != nullptr)
	//{
	//	MotionSensorReadVal& rVal = 
	//		static_cast<MotionSensorReadVal&>(motSens->Read());
	//	stringstream ss;
	//	ss << boolalpha << rVal;
	//	io::StdIO::StandardOutput(ss.str());
	//	return;
	//}
	//
	//LightSensor* lightSens = dynamic_cast<LightSensor*>(dev);
	//if (lightSens != nullptr)
	//{
	//	LightSensorReadVal& rVal =
	//		static_cast<LightSensorReadVal&>(lightSens->Read());
	//	stringstream ss;
	//	ss << rVal;
	//	io::StdIO::StandardOutput(ss.str());
	//	return;
	//}
}



