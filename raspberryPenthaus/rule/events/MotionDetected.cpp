#include <sstream>
#include "MotionDetected.h"

using namespace rule;
using namespace events;
using namespace gpio;
using namespace prototype;
using namespace std;

const string MotionDetected::name = "Motion_Detected";
const MotionDetected MotionDetected::prototype;

MotionDetected::MotionDetected()
	: _logic(false), _devID(-1), _sensor(nullptr)
{

}

MotionDetected::~MotionDetected()
{

}

string MotionDetected::ToString() const
{
	string s(name
		+ " dev_id " + to_string(_devID)
		+ " logic " + BoolToString(_logic));
	return s;
}

void MotionDetected::Save(ostream& str) const
{
	str << ToString();
}

bool MotionDetected::Load(string& s)
{
	if (_isInit)
		return false;

	stringstream ss(s);
	string buffer;
	ss >> buffer;
	if (buffer != name)
		return false;
	ss >> buffer >> _devID;
	if (buffer != "dev_id")
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

IPrototype* MotionDetected::Clone() const
{
	return new MotionDetected(prototype);
}

bool MotionDetected::AttachDevice(IDevice* dev)
{
	_sensor = nullptr;
	_sensor = dynamic_cast<MotionSensor*>(dev);
	if (_sensor)
		return true;
	else
		return false;
}

bool MotionDetected::IsActivated()
{
	if (_logic)
		return _sensor->IsMotionDetected();
	else
		return !_sensor->IsMotionDetected();
}