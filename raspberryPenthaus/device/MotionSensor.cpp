#include <chrono>
#include <string>
#include <sstream>

#include "MotionSensor.h"
#include "../event/MotionDetected.hpp"

using namespace device;
using namespace std;

namespace pt = boost::property_tree;

const string MotionSensor::name = "motionsensor";

MotionSensor::MotionSensor(int id, int pin, bool logic)
	: DeviceBase(id), _pin(pin), _logic(logic), _myVal(false, id)
{
	_state = -1;
}

string MotionSensor::ToString() const
{
	string s = name + "\tID " + to_string(_id)
		+ "\tpin " + to_string(_pin);
	if (_state == -1)
		s += "\tUndef";
	else if (_state == static_cast<int>(_logic))
		s += "\tMotion detected";
	else
		s += "\tNo motion";

	return s;
}

void MotionSensor::Save(ostream& str) const
{
	string s(name
		+ " id " + to_string(_id)
		+ " pin " + to_string(_pin)
		+ " logic " + print::BoolToString(_logic));
	str << s;
}

void MotionSensor::SaveToTree(boost::property_tree::ptree& tree, const string& path) const
{
	pt::ptree &sensNode = tree.add(path + "motionsensor", "");
	sensNode.put("id", _id);
	sensNode.put("pin", _pin);
	sensNode.put("logic", _logic);
}

bool MotionSensor::Load(string& s)
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
	if (buffer != "logic")
		return false;
	ss >> buffer;
	if (buffer == "true")
		_logic = true;
	else if (buffer == "false")
		_logic = false;
	else
		return false;

	_myVal.deviceID = _id;


	_isInit = true;
	return true;
}

bool MotionSensor::LoadFromTree(pt::ptree::value_type &val)
{
	if (_isInit)
		return false;
	try
	{
		_id = val.second.get<int>("id");
		_pin = val.second.get<int>("pin");
		_logic = val.second.get<bool>("logic");
	}
	catch (exception&)
	{
		return false;
	}

	_isInit = true;
	return true;
}

event::eventPtr MotionSensor::Read()
{
	_myVal.val = (_Read() == static_cast<int>(_logic));
	if (_myVal.val)
		return make_shared<event::MotionDetected>(_myVal);
	else
		return nullptr;
}

string MotionSensor::Execute(string& s)
{
	return "Syntax error";
}