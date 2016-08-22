#include <chrono>

#include "Bulb.h"
#include "../io/StdIO.h"
#include "../config.h"

using namespace device;

using namespace std;
using namespace chrono;

namespace pt = boost::property_tree;

const string Bulb::name = "bulb";

Bulb::Bulb(int id, unsigned int delay)
	: DeviceBase(id), _delay(delay), _state(-1), _isLocked(false)
{

}

Bulb::~Bulb()
{

}

string Bulb::ToString() const
{
	string s(name + "\t\tID " + to_string(_id));
	switch (_state)
	{
	case 1:
		s += "\tOn";
		break;
	case 0:
		s += "\tOff";
		break;
	default:
		s += "\tUndef";
		break;
	}
	if (_isLocked)
		s += "\tLocked";
	if (_state == 1)
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

void Bulb::SaveToTree(pt::ptree& tree, const string& path) const
{
	pt::ptree &myNode = tree.add(path + name, "");
	myNode.put("id", _id);
}

bool Bulb::LoadFromTree(pt::ptree::value_type& val)
{
	if (_isInit)
		return false;
	try
	{
		_id = val.second.get<int>("id");
	}
	catch (exception& e)
	{
		string err = "Bulb.LoadFromTree() returned false! ";
		err += "device ID: " + to_string(_id);
		err += "what(): ";
		err += e.what();
		io::StdIO::ErrorOutput(err);
		return false;
	}
	_isInit = true;
	return true;
}

bool Bulb::Write(IWriteVal* val)
{
	BulbWriteVal * vPtr = dynamic_cast<BulbWriteVal*>(val);
	if (vPtr) { _Write(vPtr->val); return true; }
	else { return false; }
}

string Bulb::Execute(string &)
{
	return string("No implementation!");
}

void device::Bulb::On()
{
	#ifdef LOG
	io::StdIO::StandardOutput(
		print::TimeToString(system_clock::now()) +
		" Bulb.On()" + " device ID: " + to_string(_id));
	#endif

	if (_isLocked)
		return;

	_lightingTime = system_clock::now() + _delay;

	if (!IsOn())
	{
		_Write(true);
		_state = 1;
	}
}

void device::Bulb::Off()
{
    #ifdef LOG
	string s = print::TimeToString(system_clock::now()) +
		" Bulb.Off()" + " device ID: " + to_string(_id);
    #endif

	if (_isLocked)
		return;

	if (IsOn())
	{
		auto now = system_clock::now();
		if (now >= _lightingTime)
		{
			_Write(false);
			_state = 0;
		}
		#ifdef LOG
		else
			s +=" - IGNORED";
		#endif
	}
	#ifdef LOG
	io::StdIO::StandardOutput(s);
	#endif
}

void device::Bulb::LockOn()
{
	Unlock();
	On();
	_isLocked = true;
}

void device::Bulb::LockOff()
{
	Unlock();
	Off();
	_isLocked = true;
}

void device::Bulb::Unlock()
{
	_isLocked = false;
}

void device::Bulb::ChangeDelay(int delay)
{
	duration<int> newDelay(delay);
	_delay = newDelay;
}
