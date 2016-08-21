#include <stdexcept>

#include "LightDriver.h"
#include "Program.h"

#include "../event/LightDetected.hpp"
#include "../event/MotionDetected.hpp"
#include "../event/LightExpired.hpp"

#include "../io/StdIO.h"

#include <boost/date_time/posix_time/posix_time.hpp>

using namespace program;
using namespace event;
using namespace device;
using namespace std;

namespace ba = boost::asio;
namespace pt = boost::property_tree;

LightDriver::LightDriver(device::DeviceManager& devMan)
	:	 id_(0),
		delay_(2),
		state_(State::Night),
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
	driverNode.put("id", id_);
	driverNode.put("delay", static_cast<unsigned int>(delay_.total_seconds()));
	myPath = "devices";
	driverNode.put(myPath, "");
	myPath.append(1, '.');
	for (auto &led : leds_)
	{
		led.second->SaveToTree(driverNode, myPath);
	}
	for (auto &bulb : bulbs_)
	{
		bulb.second->SaveToTree(driverNode, myPath);
	}
	for (auto &motSen : motionSensors_)
	{
		motSen.second->SaveToTree(driverNode, myPath);
	}
	if (lightSensor_)
	{
		lightSensor_->SaveToTree(driverNode, myPath);
	}
}

//wczytanie z drzewa
bool LightDriver::LoadFromTree(pt::ptree::value_type &v)
{
	try
	{
		IDevice* dev = nullptr;
		id_ = v.second.get<int>("id");
		delay_ = boost::posix_time::seconds(v.second.get<unsigned int>("delay"));
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
	eventHanled_ = false;

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

	if (!eventHanled_)
		IEventHandler::HandleEvent(evPtr);
}

//obsluga eventow w dzien
void LightDriver::Day_(event::eventPtr evPtr)
{
	MotionEventDay_(evPtr);
	if (eventHanled_)
		return;

	LightEventDay_(evPtr);
	if (eventHanled_)
		return;

	Default_(evPtr);
}

//oblsuga eventow w nocy
void LightDriver::Night_(event::eventPtr evPtr)
{
	MotionEventNight_(evPtr);
	if (eventHanled_)
		return;

	LightEventNight_(evPtr);
	if (eventHanled_)
		return;

	Default_(evPtr);
}

//eventy niezalezne od pory dnia
void LightDriver::Default_(event::eventPtr evPtr)
{
	ExpiredHandler_(evPtr);
	if (eventHanled_)
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
		io::StdIO::StandardOutput(motionDet->ToString() + " - IGNORED");
		#endif

		eventHanled_ = true;
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

		for (auto &bulb : bulbs_)
		{
			bulb.second->On();
		}
		for (auto &led : leds_)
		{
			led.second->On();
		}

		if (bulbs_.size() != 0)
		{
			ExpiredCreator(bulbs_.begin()->second);
		}
		else if (leds_.size() != 0)
		{
			ExpiredCreator(leds_.begin()->second);
		}

		eventHanled_ = true;
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
		eventHanled_ = true;
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
		eventHanled_ = true;
	}
}

//wylacz swiatla
void LightDriver::ExpiredHandler_(event::eventPtr evPtr)
{
	LightExpired* lightExp = dynamic_cast<LightExpired*>(evPtr.get());
	if (lightExp)
	{
		#ifdef LOG
		io::StdIO::StandardOutput(lightExp->ToString());
		#endif

		for (auto &bulb : bulbs_)
		{
			bulb.second->Off();
		}
		for (auto &led : leds_)
		{
			led.second->Off();
		}
		eventHanled_ = true;
	}
}

//ustaw timer wylaczenia swiatla
void LightDriver::ExpiredCreator(IDevice* dev)
{
	auto handler = [this, dev](const boost::system::error_code& ec)
	{
		if (!ec)
		{
			auto exp = make_shared<LightExpired>(*dev, *this);
			eventPool_.Push(exp);
		}
		else if (ec != ba::error::operation_aborted)
		{
			io::StdIO::ErrorOutput(ec.message());
		}
	};
	timer_.expires_from_now(delay_);
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

	Bulb *bulb = dynamic_cast<Bulb*>(dev);
	if (bulb)
	{
		bulbs_[bulb->GetID()] = bulb;
		myDevs_[bulb->GetID()] = bulb;
		return;
	}

	throw std::logic_error("Try to add unknow device");
}

