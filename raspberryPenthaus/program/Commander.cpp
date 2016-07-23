#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include "Commander.h"
#include "../device/DeviceManager.h"
#include "../device/LED.h"
#include "../device/MotionSensor.h"
#include "../device/LightSensor.h"

using namespace program;
using namespace device;
using namespace std;

Commander::Commander(device::DeviceManager &man) : man(man)
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
		dev = man.GetDevice(dev_id);
	}
	catch (runtime_error& ex)
	{
		string s("Syntax error: ");
		s += ex.what();
		return s;
	}

	return dev->Execute(line);
}