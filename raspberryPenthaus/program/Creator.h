#ifndef CREATOR_H
#define CREATOR_H
#include <iostream>
#include <list>
#include <string>

#include "../gpio/IDevice.h"
#include "../rule/Rule.h"
#include "../rule/RuleManager.h"
#include "../prototype/PrototypeManager.h"

namespace program
{
	class Creator
	{
	public:
		Creator(rule::RuleManager&);
		~Creator();

		void DevicesFromFile(std::string&);
		void RulesFromFile(std::string&);
		void EventsFromFile(std::string&);

		gpio::IDevice* CreateDevice(std::string&);
		rule::Rule* CreateRule(std::string&);
		std::string CreateEvents(std::string&);

	private:
		std::string _CreateEvent(std::string&, std::istream&);
		std::list<std::string> _ReadLines(std::string&);

		rule::RuleManager& _ruleManager;
		prototype::PrototypeManager _prototypeManager;
	};
}
#endif // !CREATOR_H
