#include <chrono>
#include <string>
#include <sstream>
#include <future>
#include <thread>
#include <exception>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "LED.h"
#include "../program/Program.h"

using namespace device;
using namespace std;
using namespace chrono;

namespace pt = boost::property_tree;

const string LED::name = "led";

LED::LED(int id, int pin, int delay, bool logic)
	: DeviceBase(id), _pin(pin), _delay(delay),
	_logic(logic), _defaultValue(!static_cast<int>(logic))
{
	_state = -1;
	_isLocked = false;
}

string LED::ToString() const
{
	string s(name
		+ "\t\tID " + to_string(_id)
		+ "\tpin " + to_string(_pin));
	if (_state == -1)
		s += "\tUndef";
	else if (IsOn())
		s += "\tOn";
	else
		s += "\tOff";
	if (_isLocked == true)
	{
		s += "\tLocked";
	}
	else if (IsOn())
	{
		timePoint now = system_clock::now();
		if (now > _lightingTime)
		{
			system_clock::duration diff(_lightingTime + _delay - now);
			auto sec = duration_cast<seconds>(diff);
			long long count = sec.count();
			if (count >= 0)
			{
				s += "\t\tGoes off in " + to_string(count);
			}
		}
	}
	return s;
}

void LED::SaveToTree(boost::property_tree::ptree& tree, const string& path) const
{
	pt::ptree &ledNode = tree.add(path + "led", "");
	ledNode.put("id", _id);
	ledNode.put("pin", _pin);
	ledNode.put("delay", _delay.count());
	ledNode.put("logic", _logic);
}

bool LED::LoadFromTree(pt::ptree::value_type &val)
{
	if (_isInit)
		return false;

	try
	{
		_id = val.second.get<int>("id");
		_pin = val.second.get<int>("pin");
		_delay = seconds(val.second.get<int>("delay"));
		_logic = val.second.get<bool>("logic");
		_defaultValue = !static_cast<int>(_logic);
	}
	catch (exception&)
	{
		//TO DO: dodac logowanie bledu
		return false;
	}

	_isInit = true;
	return true;
}

bool device::LED::Write(IWriteVal *val)
{
	LEDWriteVal *vPtr = dynamic_cast<LEDWriteVal*>(val);
	if (vPtr)
	{
		_Write(vPtr->val);
		return true;
	}
	else
		return false;
}

string LED::Execute(string& s)
{
	string buffer;
	stringstream ss(s);

	ss >> buffer >> buffer;
	if (buffer != this->name)
		return "Invalid device ID";

	ss >> buffer >> buffer >> buffer;
	if (buffer == "Lock_On")
		this->LockOn();
	else if (buffer == "Lock_Off")
		this->LockOff();
	else if (buffer == "Unlock")
		this->Unlock();
	else if (buffer == "Change_Delay")
	{
		ss >> buffer;
		if (buffer != "delay")
			return "Syntax error";
		int delay;
		ss >> delay;
		this->ChangeDelay(delay);
	}
	else
		return "Syntax error";

	return "Executed!";
}

void LED::On()
{
	#ifdef LOG
	io::StdIO::StandardOutput(
		print::TimeToString(system_clock::now())
		+ " LED.On()");
	#endif

	if (_isLocked)
		return;
	_lightingTime = system_clock::now() + _delay;
	if (!IsOn())
	{
		_Write(static_cast<int>(_logic));
		_state = static_cast<int>(_logic);
	}
}

void LED::Off()
{
	if (_isLocked)
		return;
	if (IsOn())
	{
		auto now = system_clock::now();
		if (now >= _lightingTime)
		{
			_Write(!static_cast<int>(_logic));
			_state = !static_cast<int>(_logic);

			#ifdef LOG
			io::StdIO::StandardOutput(
				print::TimeToString(system_clock::now())
				+ " LED.Off()");
			#endif
		}
	}
}

void LED::LockOn()
{
	Unlock();
	On();
	_isLocked = true;
}

void LED::LockOff()
{
	Unlock();
	Off();
	_isLocked = true;
}

void LED::Unlock()
{
	_isLocked = false;
}

void LED::ChangeDelay(int delay)
{
	duration<int> newDelay(delay);
	_delay = newDelay;
}

