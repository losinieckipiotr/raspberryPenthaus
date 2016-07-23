#include <chrono>
#include <string>
#include <sstream>
#include "MotionSensor.h"

using namespace device;
using namespace std;

const string MotionSensor::name = "Motion_Sensor";

MotionSensor::MotionSensor(int id, int pin, bool logic)
	: DeviceBase(id), _pin(pin), _logic(logic)
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

	_isInit = true;
	return true;
}

IReadVal& MotionSensor::Read()
{
	_myVal = (_state == static_cast<int>(_logic));
	return _myVal;
}

string MotionSensor::Execute(string& s)
{
	return "Syntax error";
}

//bool MotionSensor::IsMotionDetected()
//{
//	if (_state == static_cast<int>(_logic))
//		return true;
//	else
//		return false;
//}