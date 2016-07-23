#include <sstream>
#include <fstream>

#include "Program.h"
#include "../gpio/GPIO.h"

using namespace program;
using namespace device;
using namespace prototype;
using namespace std;

Creator::Creator(DeviceManager &devMan)
	: _devManager(devMan)
{

}

Creator::~Creator()
{

}

void Creator::DevicesFromFile(string& filename)
{
	list<string> lines = _ReadLines(filename);

	IDevice* dev = nullptr;
	for (auto& line : lines)
	{
		dev = CreateDevice(line);
		if (dev != nullptr)
			_devManager.AddDevice(dev);
	}
}

//void Creator::RulesFromFile(string& filename)
//{
//	list<string> lines = _ReadLines(filename);
//
//	for (auto& line : lines)
//	{
//		Rule* rule = CreateRule(line);
//		if (rule != nullptr)
//			_ruleManager.AddRule(rule);
//	}
//}

//void Creator::EventsFromFile(string& filename)
//{
//	list<string> lines = _ReadLines(filename);
//
//	for (auto& line : lines)
//	{
//		CreateEvents(line);
//	}
//}

IDevice* Creator::CreateDevice(string& line)
{
	string buffer;
	IDevice* dev = nullptr;
	stringstream str(line);

	str >> buffer;
	if (buffer != "create")
		return nullptr;
	getline(str, buffer);

	dev = dynamic_cast<IDevice*>(
		_prototypeManager.CreatePrototype(buffer));

	return dev;
}

//Rule* Creator::CreateRule(string& line)
//{
//	string buffer;
//	Rule* rule = nullptr;
//	stringstream str(line);
//
//	str >> buffer;
//	if (buffer != "create")
//		return nullptr;
//	getline(str, buffer);
//
//	rule = dynamic_cast<Rule*>(
//		_prototypeManager.CreatePrototype(buffer));
//
//	return rule;
//}

//string Creator::CreateEvents(string& line)
//{
//	string buffer;
//	stringstream str(line);
//
//	str >> buffer;
//	if (buffer != "add")
//	{
//		return "Syntax error";
//	}
//	str >> buffer;
//	if (buffer == "trigger" || buffer == "action")
//	{
//		str >> buffer;
//		if (buffer != "to")
//		{
//			return "Syntax error";
//		}
//		str >> buffer;
//		return _CreateEvent(buffer, str);
//	}
//	else
//	{
//		return "Syntax error";
//	}
//}

list<string> Creator::_ReadLines(string& filename)
{
	ifstream file(filename, ifstream::in);
	if (!file)
		throw runtime_error("Cannot open file: " + filename);
	file.seekg(0, file.end);
	int length = (int)file.tellg();
	file.seekg(0, file.beg);
	char * buffer = new char[length];
	file.read(buffer, length);
	file.close();

	stringstream ss(buffer);
	delete buffer;
	string buffer2;
	list<string> lines;
	while (true)
	{
		getline(ss, buffer2);
		if (ss.good())
		{
			if (buffer2 != "")
				lines.push_back(buffer2);
		}
		else
			break;
	}

	return lines;
}

//string Creator::_CreateEvent(string& ruleName, istream& str)
//{
//	string buffer;
//
//	Rule* rule = _ruleManager.ReturnRule(ruleName);
//	if (rule == nullptr)
//	{
//		return "Syntax error: Rule not found!";
//	}
//	str >> buffer;
//	if (buffer != "name")
//	{
//		return "Syntax error";
//	}
//	getline(str, buffer);
//	IEvent* ev = dynamic_cast<IEvent*>(
//		_prototypeManager.CreatePrototype(buffer));
//	if (ev == nullptr)
//	{
//		return "Syntax error.";
//	}
//	int id = ev->GetDeviceID();
//	IDevice* dev = _devManager.GetDevice(id);
//	if (dev == nullptr)
//	{
//		delete ev;
//		return "Syntax error: Device not found!";
//	}
//	if (!ev->AttachDevice(dev))
//	{
//		delete ev;
//		return "Syntax error.";
//	}
//
//	rule->AddEvent(ev);
//
//	return "Added!";
//}