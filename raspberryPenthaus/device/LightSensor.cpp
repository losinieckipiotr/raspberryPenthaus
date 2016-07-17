#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include "LightSensor.h"

using namespace gpio;
using namespace std;

const string LightSensor::name = "Light_Sensor";

LightSensor::LightSensor(int id) : DeviceBase(id)
{

}

string LightSensor::ToString() const
{
	stringstream ss;
	ss << fixed << setprecision(2);
	ss << name << "\tID " << _id;
	ss << "\tI2C";
	if (_lux == numeric_limits<double>::max())
	{
		ss << "\tLUX Out of range!";
	}
	else
	{
		ss << "\tLUX " << _lux;
	}
	ss << "\tVIS+IR " << _ch0;
	ss << "\tIR " << _ch1;
	return ss.str();
}


void LightSensor::Save(ostream& str) const
{
	string s(name
		+ " id " + to_string(_id));
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

	_isInit = true;
	return true;
}

string LightSensor::Execute(string& s)
{
	return "Syntax error";
}