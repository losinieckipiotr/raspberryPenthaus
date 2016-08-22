#include <chrono>
#include <string>
#include <sstream>
#include <future>
#include <thread>
#include <exception>

#include "LED.h"
#include "../io/StdIO.h"
#include "../config.h"

using namespace device;
using namespace std;
using namespace chrono;

namespace pt = boost::property_tree;

const string LED::name = "led";

LED::LED(int id, int pin, bool logic, unsigned int delay)
	:	DeviceBase(id), _pin(pin),
		_logic(logic), _delay(delay),
		_defaultValue(!static_cast<int>(logic)),
		_isLocked(false), _state(-1)
{

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

void LED::SaveToTree(pt::ptree& tree, const string& path) const
{
	pt::ptree &myNode = tree.add(path + name, "");
	myNode.put("id", _id);
	myNode.put("pin", _pin);
	myNode.put("logic", _logic);
}

bool LED::LoadFromTree(pt::ptree::value_type &val)
{
	if (_isInit)
		return false;
	try
	{
		_id = val.second.get<int>("id");
		_pin = val.second.get<int>("pin");
		_logic = val.second.get<bool>("logic");
		_defaultValue = !static_cast<int>(_logic);
	}
	catch (exception& e)
	{
		string err = "LED.LoadFromTree() returned false! ";
		err += "device ID: " + to_string(_id);
		err += "what(): ";
		err += e.what();
		io::StdIO::ErrorOutput(err);
		return false;
	}
	_isInit = true;
	return true;
}

bool device::LED::Write(IWriteVal *val)
{
	LEDWriteVal *vPtr = dynamic_cast<LEDWriteVal*>(val);
	if (vPtr) { _Write(vPtr->val); return true; }
	else { return false; }
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
	/*else if (buffer == "Change_Delay")
	{
		ss >> buffer;
		if (buffer != "delay")
			return "Syntax error";
		int delay;
		ss >> delay;
		this->ChangeDelay(delay);
	}*/
	else
		return "Syntax error";

	return "Executed!";
}

void LED::On()
{
	#ifdef LOG
	io::StdIO::StandardOutput(
		print::TimeToString(system_clock::now()) +
		" LED.On()" + " device ID: " + to_string(_id));
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
	#ifdef LOG
	string s = print::TimeToString(system_clock::now()) +
		" LED.Off()" + " device ID: " + to_string(_id);
	#endif

	if (_isLocked)
		return;

	if (IsOn())
	{
		auto now = system_clock::now();
		if (now >= _lightingTime)
		{
			_Write(!static_cast<int>(_logic));
			_state = !static_cast<int>(_logic);
		}
		#ifdef LOG
		else
			s += " - IGNORED";
		#endif
	}
	#ifdef LOG
	io::StdIO::StandardOutput(s);
	#endif
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

