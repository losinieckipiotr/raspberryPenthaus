#include <algorithm>
#include <fstream>
#include <stdexcept>
#include "RuleManager.h"

using namespace rule;
using namespace std;

RuleManager::RuleManager()
{

}

RuleManager::~RuleManager()
{
	for (auto& rule : _rules)
	{
		delete rule;
	}
}

void RuleManager::AddRule(Rule* rule)
{
	if (rule == nullptr)
		throw runtime_error("Attempt to add nullptr to RuleManager");
	Rule* temp = _FindByName(rule->Return_name());
	if (temp != nullptr)
		throw runtime_error("Rule already exists");
	_rules.push_back(rule);
	_rules.sort(_comp);
}

Rule* RuleManager::ReturnRule(std::string& name)
{
	return _FindByName(name);
}

void RuleManager::ExecuteRules()
{
	for (auto & rule : _rules)
	{
		rule->CheckAndExecute();
	}
}

void RuleManager::SaveRules(string& filename)
{
	ofstream file(filename, ofstream::trunc);
	for (auto& rule : _rules)
	{
		file << string("create Rule name ");
		rule->Save(file);
		file << endl;
	}
	file.close();
}

void RuleManager::SaveEvents(string& filename)
{
	ofstream file(filename, ofstream::trunc);
	for (auto& rule : _rules)
	{
		rule->SaveEvents(file);
	}
	file.close();
}

void RuleManager::ClearRules()
{
	for (auto& rule : _rules)
	{
		delete rule;
	}
	_rules.clear();
}

void RuleManager::ClearEvents()
{
	for (auto& rule : _rules)
	{
		rule->ClearEvents();
	}
}

string RuleManager::PrintRules()
{
	string s("Rules\n\n");
	for (auto& rule : _rules)
	{
		s += rule->ToString() + "\n";
	}
	s += "\n";
	return s;
}

string RuleManager::PrintEvents()
{
	string s("Events\n\n");
	for (auto& rule : _rules)
	{
		s += rule->Return_name() + ":\n";
		s += rule->PrintEvents() + "\n";
	}
	return s;
}

Rule* RuleManager::_FindByName(std::string& name)
{
	auto it = find_if(_rules.begin(), _rules.end(),
		[&name](Rule* r)->bool
	{
		if (r->Return_name() == name)
			return true;
		else
			return false;
	});
	if (it == _rules.end())
		return nullptr;
	else
		return *it;
}