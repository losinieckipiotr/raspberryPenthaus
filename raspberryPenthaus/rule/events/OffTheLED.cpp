#include <sstream>
#include "OffTheLED.h"

using namespace rule;
using namespace events;
using namespace gpio;
using namespace prototype;
using namespace std;

const string OffTheLED::name = "Off_The_LED";
const OffTheLED OffTheLED::prototype;

OffTheLED::OffTheLED()
	: _devID(-1), _led(nullptr)
{

}

OffTheLED::~OffTheLED()
{

}

string OffTheLED::ToString() const
{
	string s(name
		+ " dev_id " + to_string(_devID));
	return s;
}

void OffTheLED::Save(ostream& str) const
{
	str << ToString();
}

bool OffTheLED::Load(string& s)
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
	
	_isInit = true;
	return true;
}

IPrototype* OffTheLED::Clone() const
{
	return new OffTheLED(prototype);
}

bool OffTheLED::AttachDevice(gpio::IDevice* dev)
{
	_led = nullptr;
	_led = dynamic_cast<LED*>(dev);
	if (_led)
		return true;
	else
		return false;
}

void OffTheLED::Execute()
{
	_led->HardOff();
}