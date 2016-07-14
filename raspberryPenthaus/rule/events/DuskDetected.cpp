#include <sstream>
#include "DuskDetected.h"

using namespace rule;
using namespace events;
using namespace gpio;
using namespace prototype;
using namespace std;

const string DuskDetected::name = "Dusk_Detected";
const DuskDetected DuskDetected::prototype;

DuskDetected::DuskDetected()
	: _logic(false), _devID(-1),
	_threshold(-1), _sensor(nullptr)
{

}

DuskDetected::~DuskDetected()
{

}

string DuskDetected::ToString() const
{
	string s(name
		+ " dev_id " + to_string(_devID)
		+ " val " + to_string(_threshold)
		+ " logic " + BoolToString(_logic));
	return s;
}

void DuskDetected::Save(ostream& str) const
{
	str << ToString();
}

bool DuskDetected::Load(string& s)
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
	ss >> buffer >> _threshold;
	if (buffer != "val")
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

IPrototype* DuskDetected::Clone() const
{
	return new DuskDetected(prototype);
}

bool DuskDetected::AttachDevice(IDevice* dev)
{
	_sensor = nullptr;
	_sensor = dynamic_cast<LightSensor*>(dev);
	if (_sensor)
		return true;
	else
		return false;
}

bool DuskDetected::IsActivated()
{
	if (_logic)
	{
		if (_sensor->GetLight() < _threshold)
			return true;
		else
			return false;
	}
	else
	{
		if (_sensor->GetLight() > _threshold)
			return true;
		else
			return false;
	}
}