#include <sstream>
#include "SwitchTheLED.h"

using namespace rule;
using namespace events;
using namespace prototype;
using namespace std;

const string SwitchTheLED::name = "Switch_The_LED";
const SwitchTheLED SwitchTheLED::prototype;

SwitchTheLED::SwitchTheLED()
	: _onOff(false), _devID(-1),
	_led(nullptr)
{

}

SwitchTheLED::~SwitchTheLED()
{

}

string SwitchTheLED::ToString() const
{
	string s(name
		+ " dev_id " + to_string(_devID)
		+ " logic " + print::BoolToString(_onOff));
	return s;
}

void SwitchTheLED::Save(ostream& str) const
{
	str << ToString();
}

bool SwitchTheLED::Load(string& s)
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
		_onOff = true;
	else if (buffer == "false")
		_onOff = false;
	else
		return false;

	_isInit = true;
	return true;
}

IPrototype* SwitchTheLED::Clone() const
{
	return new SwitchTheLED(prototype);
}

bool SwitchTheLED::AttachDevice(device::IDevice* dev)
{
	_led = nullptr;
	_led = dynamic_cast<device::LED*>(dev);
	if (_led)
		return true;
	else
		return false;
}

void SwitchTheLED::Execute()
{
	if (_onOff)
		_led->On();
	else
		_led->Off();
}