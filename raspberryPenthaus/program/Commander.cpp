#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include "Commander.h"
#include "../gpio/GPIO.h"
#include "../gpio/LED.h"
#include "../gpio/MotionSensor.h"
#include "../gpio/LightSensor.h"

using namespace program;
using namespace gpio;
using namespace std;

Commander::Commander()
{

}

Commander::~Commander()
{

}

string Commander::ExecuteCommand(string& line)
{
	string buffer;
	int dev_id;
	IDevice* dev = nullptr;
	stringstream str(line);

	str >> buffer >> buffer >> buffer >> dev_id;
	if (buffer != "id")
		return "Synatax error";
	try
	{
		dev = GPIO::Instance()->GetDevice(dev_id);
	}
	catch (runtime_error& ex)
	{
		string s("Syntax error: ");
		s += ex.what();
		return s;
	}

	return dev->Execute(line);
}