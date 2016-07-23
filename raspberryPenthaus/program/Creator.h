#ifndef CREATOR_H
#define CREATOR_H
#include <iostream>
#include <list>
#include <string>

#include "../device/IDevice.h"
#include "../rule/Rule.h"
#include "../device/DeviceManager.h"
#include "../rule/RuleManager.h"
#include "../prototype/PrototypeManager.h"

namespace program
{
	class Creator
	{
	public:
		Creator(device::DeviceManager&, rule::RuleManager&);
		Creator(const Creator& copy) = delete;
		Creator(Creator&&) = delete;
		~Creator();

		//Creator& operator=(const Creator& copy);

		void DevicesFromFile(std::string&);
		void RulesFromFile(std::string&);
		void EventsFromFile(std::string&);

		device::IDevice* CreateDevice(std::string&);
		rule::Rule* CreateRule(std::string&);
		std::string CreateEvents(std::string&);

	private:
		std::list<std::string> _ReadLines(std::string&);
		std::string _CreateEvent(std::string&, std::istream&);

		device::DeviceManager& _devManager;
		rule::RuleManager& _ruleManager;
		prototype::PrototypeManager _prototypeManager;
	};
}
#endif // !CREATOR_H
