#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>

#include "LightSensor.h"
#include "../event/LightDetected.hpp"

using namespace device;
using namespace std;

const string LightSensor::name = "Light_Sensor";

LightSensor::LightSensor(int id, double threshold)
	:	 DeviceBase(id),
		_threshold(threshold),
		_myVal(-1.0, threshold, id)
{

}

string LightSensor::ToString() const
{
	stringstream ss;
	ss << fixed << setprecision(2);
	ss << name << "\tID " << _id;
	ss << "\tI2C";
	if (_myVal.val == numeric_limits<double>::max())
	{
		ss << "\tLUX Out of range!";
	}
	else
	{
		ss << "\tLUX " << _myVal.val;
	}
	return ss.str();
}

void LightSensor::Save(ostream& str) const
{
	string s(name
		+ " id " + to_string(_id)
		+ " threshold " + to_string(_threshold));
	str << s;
}

bool LightSensor::Load(string& s)
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
	ss >> buffer >> _threshold;
	if (buffer != "threshold")
		return false;

	_myVal.deviceID = _id;
	_myVal.threshold = _threshold;

	_isInit = true;
	return true;
}

shared_ptr<event::IEvent> LightSensor::Read()
{
	_myVal.val = _Read();
	_myVal.threshold = _threshold;
	return make_shared<event::LightDetected>(_myVal);
}

string LightSensor::Execute(string& s)
{
	return "Syntax error";
}
