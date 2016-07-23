#include <chrono>
#include <string>
#include <sstream>
#include "LED.h"

//using namespace gpio;
using namespace device;
using namespace std;
using namespace chrono;

const string LED::name = "LED";

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
	else if (_IsOn())
		s += "\tOn";
	else
		s += "\tOff";
	if (_isLocked == true)
	{
		s += "\tLocked";
	}
	else if (_IsOn())
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

void LED::Save(ostream& str) const
{
	string s(name
		+ " id " + to_string(_id)
		+ " pin " + to_string(_pin)
		+ " delay " + to_string(_delay.count())
		+ " logic " + print::BoolToString(_logic));
	str << s;
}

bool LED::Load(string& s)
{
	if (_isInit)
		return false;

	stringstream ss(s);
	string buffer;
	ss >> buffer;
	if (buffer != name)
		return false;
	ss >> buffer >> _id;
	if (buffer != "id")
		return false;
	ss >> buffer >> _pin;
	if (buffer != "pin")
		return false;
	ss >> buffer;
	int t;
	ss >> t;
	_delay = duration<int>(t);
	if (buffer != "delay")
		return false;
	ss >> buffer;
	if (buffer != "logic")
		return false;
	ss >> buffer;
	if (buffer == "true")
	{
		_logic = true;
		_defaultValue = 0;
	}
	else if (buffer == "false")
	{
		_logic = false;
		_defaultValue = 1;
	}
	else
		return false;

	_isInit = true;
	return true;
}

bool device::LED::Write(IWriteVal * val)
{
	LEDWriteVal * vPtr = nullptr;
	if (vPtr = dynamic_cast<LEDWriteVal*>(val))
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
	if (_isLocked)
		return;
	_lightingTime = system_clock::now();
	if (!_IsOn())
	{
		_lightingTime = system_clock::now();
		_Write(static_cast<int>(_logic));
		_state = static_cast<int>(_logic);
	}
}

void LED::Off()
{
	if (_isLocked)
		return;
	if (_IsOn())
	{
		timePoint now = system_clock::now();
		if (now > (_lightingTime + _delay))
		{
			_Write(!static_cast<int>(_logic));
			_state = !static_cast<int>(_logic);
		}
	}
}

void LED::HardOff()
{
	if (_isLocked)
		return;
	if (_IsOn())
	{
		_Write(!static_cast<int>(_logic));
		_state = !static_cast<int>(_logic);
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
	HardOff();
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

void device::LED::_Write(bool)
{

}
