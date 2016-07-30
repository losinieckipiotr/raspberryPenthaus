#include <stdexcept>
#include <chrono>
#include <future>

#include "LightDriver.h"
#include "Program.h"
#include "../event/LightDetected.hpp"
#include "../event/MotionDetected.hpp"
#include "../event/LEDExpired.hpp"
#include "../io/StdIO.h"

using namespace program;
using namespace event;
using namespace device;
using namespace std;

namespace pt = boost::property_tree;

LightDriver::LightDriver(device::DeviceManager& devMan)
	: state_(State::Day), eventPool_(program::Program::GetEventPool()), devMan_(devMan)
{

}

LightDriver::~LightDriver()
{

}

void LightDriver::SaveToTree(pt::ptree& tree, const string& path) const
{
	/*pt::ptree myTree;
	string myPath = "serialize.drivers";
	myTree.put(myPath, "");

	myPath = "serialize.drivers.lightdriver";
	pt::ptree &driverNode = myTree.add(myPath, "");
	driverNode.put("devices", "");

	auto dev = devMan_.GetDevice(0);
	dev->SaveToTree(driverNode, "devices.");

	dev = devMan_.GetDevice(1);
	dev->SaveToTree(driverNode, "devices.");

	dev = devMan_.GetDevice(2);
	dev->SaveToTree(driverNode, "devices.");

	pt::ptree &driverNode2 = myTree.add(myPath, "");
	driverNode2.put("devices", "");

	dev = devMan_.GetDevice(0);
	dev->SaveToTree(driverNode2, "devices.");

	dev = devMan_.GetDevice(1);
	dev->SaveToTree(driverNode2, "devices.");

	dev = devMan_.GetDevice(2);
	dev->SaveToTree(driverNode2, "devices.");

	pt::write_xml("serialize4.xml", myTree);*/

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
	IDevice* dev = nullptr;
	pt::ptree &devsNode = v.second.get_child("devices");
	for (pt::ptree::value_type &v : devsNode)
	{
		dev = dynamic_cast<IDevice*>
			(protoMan_.CreatePrototype(v));
		if (dev)
		{
			//TO DO: try catch block
			devMan_.AddDevice(dev);
			AddDev_(dev);
		}
	}
	return true;
}

void LightDriver::HandleEvent(event::eventPtr evPtr)
{
	eventHanled = false;

	//castowanie dac tutaj?
	//a unikalne dzialanie wyzej?
	//jak zrobic aby castowanie eventow bylo tylku raz
	//w jednym miejscu?

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
		//io::StdIO::StandardOutput( motionPtr->ToString());
		eventHanled = true;
		return;
	}
	LightDetected* lightPtr = dynamic_cast<LightDetected*>(evPtr.get());
	if (lightPtr)
	{
		auto lightRead = lightPtr->GetLightReadVal();
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

		//test
		auto dev = dynamic_cast<LED*>(devMan_.GetDevice(0));
		if (dev)
		{
			dev->On();
			io::StdIO::StandardOutput("LightON(), waiting 3s");
			auto throwExpiredEvent = [this, dev]()
			{
				this_thread::sleep_for(chrono::seconds(3));
				auto exp = make_shared<LEDExpired>(*dev);
				eventPool_.Push(exp);

				//io::StdIO::StandardOutput("Time expired event");
			};
			async(launch::async, throwExpiredEvent);
			eventHanled = true;
		}
		//koniec testu
		
		return;
	}
	LightDetected* lightPtr = dynamic_cast<LightDetected*>(evPtr.get());
	if (lightPtr)
	{
		auto lightRead = lightPtr->GetLightReadVal();
		if (lightRead())
		{
			state_ = State::Day;
			io::StdIO::StandardOutput("=====''DAY''=====");
		}
		io::StdIO::StandardOutput(lightPtr->ToString());
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
		auto dev = dynamic_cast<LED*>(devMan_.GetDevice(ledPtr->GetDeviceID()));
		if (dev)
		{
			dev->Off();
			io::StdIO::StandardOutput("LightOff()");
			io::StdIO::StandardOutput(ledPtr->ToString());
			eventHanled = true;
		}
	}
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
	//TO DO: throw exception
}

