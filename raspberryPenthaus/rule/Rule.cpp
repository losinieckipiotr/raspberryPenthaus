#include <exception>
#include <sstream>
#include "Rule.h"

using namespace rule;
using namespace prototype;
using namespace std;

const Rule Rule::prototype("", -1, false);
const string Rule::name("Rule");

Rule::Rule(string name, int priority, bool mode)
{
	_name = name;
	_priority = priority;
	_mode = mode;
}

Rule::~Rule()
{
	for (auto& trigger : _triggers)
	{
		delete trigger;
	}

	for (auto& action : _actions)
	{
		delete action;
	}
}

string Rule::ToString() const
{
	string s(_name
		+ " priority " + to_string(_priority)
		+ " mode " + BoolToString(_mode));
	return s;
}

void Rule::Save(ostream& str) const
{
	str << ToString();
}

bool Rule::Load(string& s)
{
	if (_isInit)
		return false;

	stringstream ss(s);
	string buffer;
	ss >> buffer;
	if (buffer != name)
		return false;
	ss >> buffer >> _name;
	if (buffer != "name")
		return false;
	ss >> buffer >> _priority;
	if (buffer != "priority")
		return false;
	ss >> buffer;
	if (buffer != "mode")
		return false;
	ss >> buffer;
	if (buffer == "true")
		_mode = true;
	else if (buffer == "false")
		_mode = false;
	else
		return false;

	_isInit = true;
	return true;
}

IPrototype* Rule::Clone() const
{
	return new Rule(prototype);
}

void Rule::AddEvent(IEvent* ev)
{
	ITrigger* t = nullptr;
	t = dynamic_cast<ITrigger*>(ev);
	if (t)
		_triggers.push_back(t);
	else
		_actions.push_back(static_cast<IAction*>(ev));
}

void Rule::CheckAndExecute()
{
	//all
	if (_mode == true)
	{
		for (auto& trigger : _triggers)
		{
			if (trigger->IsActivated() == false)
				return;
		}

		for (auto& action : _actions)
		{
			action->Execute();
		}
	}
	//any
	else
	{
		for (auto& trigger : _triggers)
		{
			if (trigger->IsActivated() == true)
				break;
		}

		for (auto& action : _actions)
		{
			action->Execute();
		}
	}
}

void Rule::SaveEvents(ofstream& file)
{
	for (auto& trigger : _triggers)
	{
		file << "add trigger to " << _name << " name ";
		trigger->Save(file);
		file << endl;
	}
	for (auto& action : _actions)
	{
		file << "add action to " << _name << " name ";
		action->Save(file);
		file << endl;
	}
	file << endl;
}

void Rule::ClearEvents()
{
	for (auto& trigger : _triggers)
	{
		delete trigger;
	}
	_triggers.clear();
	for (auto& action : _actions)
	{
		delete action;
	}
	_actions.clear();
}

string Rule::PrintEvents()
{
	string s;
	for (auto& trigger : _triggers)
	{
		s += "Trigger " + _name + " name ";
		s += trigger->ToString() + "\n";
	}
	for (auto& action : _actions)
	{
		s += ("Action " + _name + " name ");
		s += action->ToString() + "\n";
	}
	return s;
}

