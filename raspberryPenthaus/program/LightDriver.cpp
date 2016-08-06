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
	devMan_(devMan),
	work_(service_),
	timer_(service_)
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
	//wylaczenie watka timerow
	service_.stop();
}

//zapis do drzewa
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

//wczytanie z drzewa
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

//obsluga eventow
void LightDriver::HandleEvent(event::eventPtr evPtr)
{
	eventHanled = false;

	if (isItMyDevEvent(evPtr->GetDeviceID()))
	{
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
	}

	if (!eventHanled)
		IEventHandler::HandleEvent(evPtr);
}

//obsluga eventow w dzien
void LightDriver::Day_(event::eventPtr evPtr)
{
	MotionEventDay_(evPtr);
	if (eventHanled)
		return;

	LightEventDay_(evPtr);
	if (eventHanled)
		return;

	Default_(evPtr);
}

//oblsuga eventow w nocy
void LightDriver::Night_(event::eventPtr evPtr)
{
	MotionEventNight_(evPtr);
	if (eventHanled)
		return;

	LightEventNight_(evPtr);
	if (eventHanled)
		return;

	Default_(evPtr);
}

//eventy niezalezne od pory dnia
void LightDriver::Default_(event::eventPtr evPtr)
{
	LEDExpiredHandler_(evPtr);
	if (eventHanled)
		return;

	//mozliwe nastepne eventy...
}

//ingoruj
void LightDriver::MotionEventDay_(event::eventPtr evPtr)
{
	MotionDetected* motionDet = dynamic_cast<MotionDetected*>(evPtr.get());
	if (motionDet)
	{
		#ifdef LOG
		io::StdIO::StandardOutput(lightDet->ToString() + "- ignored");
		#endif
		eventHanled = true;
	}
}

//zapal swiatla
void LightDriver::MotionEventNight_(event::eventPtr evPtr)
{
	MotionDetected* motionDet = dynamic_cast<MotionDetected*>(evPtr.get());
	if (motionDet)
	{
		#ifdef LOG
		io::StdIO::StandardOutput(motionDet->ToString());
		#endif

		for (auto &led : leds_)
		{
			led.second->On();
			LEDExpiredCreator(led.second);
		}
		eventHanled = true;
	}
}

//sprawdz czy jest juz noc
void LightDriver::LightEventDay_(event::eventPtr evPtr)
{
	LightDetected* lightDet = dynamic_cast<LightDetected*>(evPtr.get());
	if (lightDet)
	{
		#ifdef LOG
		io::StdIO::StandardOutput(lightDet->ToString());
		#endif

		auto lightRead = lightDet->GetLightReadVal();
		//val <= threshold ?
		if (!lightRead())
		{
			state_ = State::Night;

			#ifdef LOG
			io::StdIO::StandardOutput("=====__NIGHT__=====");
			#endif
		}
		eventHanled = true;
	}
}

//sprawdz czy jest juz dzien
void LightDriver::LightEventNight_(event::eventPtr evPtr)
{
	LightDetected* lightDet = dynamic_cast<LightDetected*>(evPtr.get());
	if (lightDet)
	{
		#ifdef LOG
		io::StdIO::StandardOutput(lightDet->ToString());
		#endif

		auto lightRead = lightDet->GetLightReadVal();
		//val > threshold ?
		if (lightRead())
		{
			state_ = State::Day;

			#ifdef LOG
			io::StdIO::StandardOutput("=====''DAY''=====");
			#endif
		}
		eventHanled = true;
	}
}

//wylacz swiatla
void LightDriver::LEDExpiredHandler_(event::eventPtr evPtr)
{
	LEDExpired* ledExp = dynamic_cast<LEDExpired*>(evPtr.get());
	if (ledExp)
	{
		#ifdef LOG
		io::StdIO::StandardOutput(ledExp->ToString());
		#endif

		for (auto &led : leds_)
		{
			led.second->Off();
		}
		eventHanled = true;
	}
}

//ustaw timer wylaczenia swiatla
void LightDriver::LEDExpiredCreator(LED *led)
{
	auto handler = [this, led](const boost::system::error_code& ec)
	{
		if (!ec)
		{
			auto exp = make_shared<LEDExpired>(*led);
			eventPool_.Push(exp);
		}
		else if (ec != ba::error::operation_aborted)
		{
			io::StdIO::ErrorOutput(ec.message());
		}
	};
	unsigned int delay = static_cast<unsigned long>
		(led->GetDelay().count());
	timer_.expires_from_now(boost::posix_time::seconds(delay));
	timer_.async_wait(handler);
}

//dodawanie urzadzen
void LightDriver::AddDev_(IDevice *dev)
{
	LED *led = dynamic_cast<LED*>(dev);
	if (led)
	{
		leds_[led->GetID()] = led;
		myDevs_[led->GetID()] = led;
		return;
	}

	MotionSensor *motSens = dynamic_cast<MotionSensor*>(dev);
	if (motSens)
	{
		motionSensors_[motSens->GetID()] = motSens;
		myDevs_[motSens->GetID()] = motSens;
		return;
	}

	LightSensor *lighSens = dynamic_cast<LightSensor*>(dev);
	if (lighSens)
	{
		lightSensor_ = lighSens;
		myDevs_[lighSens->GetID()] = lighSens;
		return;
	}

	throw std::logic_error("Try to add unknow device");
}

