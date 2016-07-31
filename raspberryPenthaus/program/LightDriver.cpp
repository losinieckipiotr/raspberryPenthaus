#include <stdexcept>
#include <chrono>

#include "LightDriver.h"
#include "Program.h"
#include "../event/LightDetected.hpp"
#include "../event/MotionDetected.hpp"
#include "../event/LEDExpired.hpp"
#include "../io/StdIO.h"

#include <boost/date_time/posix_time/posix_time.hpp>

using namespace program;
using namespace event;
using namespace device;
using namespace std;

namespace ba = boost::asio;
namespace pt = boost::property_tree;

LightDriver::LightDriver(device::DeviceManager& devMan)
	: state_(State::Day),
	eventPool_(program::Program::GetEventPool()),
	devMan_(devMan), work_(service_)
{
	thread([this]()
	{
		while (true)
		{
			try
			{
				service_.run();
				break;//zamkniecie przez stop();
			}
			catch (const std::exception& ex)
			{
				io::StdIO::ErrorOutput(ex.what());
			}
		}
	}).detach();
}

LightDriver::~LightDriver()
{
	service_.stop();
}

void LightDriver::SaveToTree(pt::ptree& tree, const string& path) const
{
	string myPath = path + "lightdriver";
	pt::ptree &driverNode = tree.add(myPath, "");
	myPath = "devices";
	driverNode.put(myPath, "");
	myPath.append(1, '.');

	for (auto &led : leds_)
	{
		led.second->SaveToTree(driverNode, myPath);
	}
	for (auto &motSen : motionSensors_)
	{
		motSen.second->SaveToTree(driverNode, myPath);
	}
	lightSensor_->SaveToTree(driverNode, myPath);
}

bool LightDriver::LoadFromTree(pt::ptree::value_type &v)
{
	try
	{
		IDevice* dev = nullptr;
		pt::ptree &devsNode = v.second.get_child("devices");
		for (pt::ptree::value_type &v : devsNode)
		{
			dev = dynamic_cast<IDevice*>
				(protoMan_.CreatePrototype(v));
			if (dev)
			{
				devMan_.AddDevice(dev);
				AddDev_(dev);
			}
		}
	}
	catch (std::exception&)
	{
		return false;
	}
	return true;
}

void LightDriver::HandleEvent(event::eventPtr evPtr)
{
	eventHanled = false;

	switch (state_)
	{
	case program::LightDriver::State::Day:
		Day_(evPtr);
		break;
	case program::LightDriver::State::Night:
		Night_(evPtr);
		break;
	default:
		break;
	}

	if (!eventHanled)
		IEventHandler::HandleEvent(evPtr);
}

void LightDriver::Day_(event::eventPtr evPtr)
{
	MotionDetected* motionPtr = dynamic_cast<MotionDetected*>(evPtr.get());
	if (motionPtr)
	{
		eventHanled = true;
		return;
	}

	LightDetected* lightPtr = dynamic_cast<LightDetected*>(evPtr.get());
	if (lightPtr)
	{
		auto lightRead = lightPtr->GetLightReadVal();
		//lightRead() { return light > threshold; }
		if (!lightRead())
		{
			state_ = State::Night;
			io::StdIO::StandardOutput("=====__NIGHT__=====");
		}
		io::StdIO::StandardOutput(lightPtr->ToString());
		eventHanled = true;
		return;
	}

	Default_(evPtr);
}

void LightDriver::Night_(event::eventPtr evPtr)
{
	MotionDetected* motionPtr = dynamic_cast<MotionDetected*>(evPtr.get());
	if (motionPtr)
	{
		io::StdIO::StandardOutput(motionPtr->ToString());

		for (auto &led : leds_)
		{
			led.second->On();
			LEDExpiredCreator(led.second);
		}
		
		eventHanled = true;
		return;
	}

	LightDetected* lightPtr = dynamic_cast<LightDetected*>(evPtr.get());
	if (lightPtr)
	{
		io::StdIO::StandardOutput(lightPtr->ToString());

		auto lightRead = lightPtr->GetLightReadVal();
		if (lightRead())
		{
			state_ = State::Day;
			io::StdIO::StandardOutput("=====''DAY''=====");
		}
		
		eventHanled = true;
		return;
	}

	Default_(evPtr);
}

void LightDriver::Default_(event::eventPtr evPtr)
{
	LEDExpired* ledPtr = dynamic_cast<LEDExpired*>(evPtr.get());
	if (ledPtr)
	{ 
		io::StdIO::StandardOutput(ledPtr->ToString());

		LED* dev = leds_[ledPtr->GetDeviceID()];
		if (dev)
		{
			dev->Off();
			eventHanled = true;
			return;
		}
	}

	//mozliwe nastepne eventy...
}

void LightDriver::AddDev_(IDevice *dev)
{
	LED *led = dynamic_cast<LED*>(dev);
	if (led)
	{
		leds_[led->GetID()] = led;
		return;
	}

	MotionSensor *motSens = dynamic_cast<MotionSensor*>(dev);
	if (motSens)
	{
		motionSensors_[motSens->GetID()] = motSens;
		return;
	}

	LightSensor *lighSens = dynamic_cast<LightSensor*>(dev);
	if (lighSens)
	{
		lightSensor_ = lighSens;
		return;
	}

	throw std::logic_error("Try to add unknow device");
}

void LightDriver::LEDExpiredCreator(LED *led)
{
	unsigned long delay = static_cast<unsigned long>
		(led->GetDelay().count());
	timerPtr tPtr(
		new ba::deadline_timer(service_, boost::posix_time::seconds(delay)));
	timers_.insert(tPtr);
	tPtr->async_wait([this, led, tPtr](const boost::system::error_code& ec)
	{
		if (!ec)
		{
			auto exp = make_shared<LEDExpired>(*led);
			eventPool_.Push(exp);
		}
		else
		{
			throw runtime_error(ec.message());
		}
		timers_.erase(tPtr);
	});
}



/*
MotionDetected* LightDriver::TryCastMotionDetected_(event::eventPtr evPtr)
{
	return dynamic_cast<MotionDetected*>(evPtr.get());
}

LightDetected* LightDriver::TryCastLightDetected_(event::eventPtr evPtr)
{
	return dynamic_cast<LightDetected*>(evPtr.get());
}

LEDExpired* LightDriver::TryCastLEDExpired_(event::eventPtr evPtr)
{
	return dynamic_cast<LEDExpired*>(evPtr.get());
}
*/

