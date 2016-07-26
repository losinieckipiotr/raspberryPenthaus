#include <stdexcept>
#include <chrono>
#include <future>

#include "LightDriver.h"
#include "../device/LED.h"
#include "../device/LightSensor.h"
#include "../device/MotionSensor.h"
#include "../event/LightDetected.hpp"
#include "../event/MotionDetected.hpp"
#include "../event/LEDExpired.hpp"
#include "../io/StdIO.h"

using namespace program;
using namespace event;
using namespace device;
using namespace std;

LightDriver::LightDriver(ItemsPool<eventPtr>& eventPool, device::DeviceManager& devMan)
	: state_(State::Day), eventPool_(eventPool), devMan_(devMan)
{

}

LightDriver::~LightDriver()
{

}

void program::LightDriver::HandleEvent(eventPtr eventPtr)
{
	eventHanled = false;

	switch (state_)
	{
	case program::LightDriver::State::Day:
		Day(eventPtr);
		break;
	case program::LightDriver::State::Night:
		Night(eventPtr);
		break;
	default:
		break;
	}
	//przekazac wyzej/dalej
	if (!eventHanled)
		io::StdIO::StandardOutput("Unhandled event: " + eventPtr->ToString());
}

void program::LightDriver::Day(eventPtr eventPtr)
{
	MotionDetected* motionPtr = dynamic_cast<MotionDetected*>(eventPtr.get());
	if (motionPtr)
	{
		//io::StdIO::StandardOutput( motionPtr->ToString());
		eventHanled = true;
		return;
	}
	LightDetected* lightPtr = dynamic_cast<LightDetected*>(eventPtr.get());
	if (lightPtr)
	{
		auto lightRead = lightPtr->GetLightReadVal();
		if (!lightRead())
		{
			state_ = State::Night;
			io::StdIO::StandardOutput("Day -> Night");
		}
		io::StdIO::StandardOutput(lightPtr->ToString());
		eventHanled = true;
		return;
	}

	Default(eventPtr);
}

void program::LightDriver::Night(eventPtr eventPtr)
{
	MotionDetected* motionPtr = dynamic_cast<MotionDetected*>(eventPtr.get());
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
	LightDetected* lightPtr = dynamic_cast<LightDetected*>(eventPtr.get());
	if (lightPtr)
	{
		auto lightRead = lightPtr->GetLightReadVal();
		if (lightRead())
		{
			state_ = State::Day;
			io::StdIO::StandardOutput("Night -> Day");
		}
		io::StdIO::StandardOutput(lightPtr->ToString());
		eventHanled = true;
		return;
	}

	Default(eventPtr);
}

void program::LightDriver::Default(eventPtr eventPtr)
{
	LEDExpired* ledPtr = dynamic_cast<LEDExpired*>(eventPtr.get());
	if (ledPtr)
	{ 
		auto dev = dynamic_cast<LED*>(devMan_.GetDevice(ledPtr->GetDeviceID()));
		if (dev)
		{
			dev->Off();
			io::StdIO::StandardOutput(ledPtr->ToString());
			eventHanled = true;
		}
	}
}
