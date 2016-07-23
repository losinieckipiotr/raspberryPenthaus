#ifndef CREATOR_H
#define CREATOR_H
#include <iostream>
#include <list>
#include <string>

#include "../device/IDevice.h"
#include "../device/DeviceManager.h"
#include "../prototype/PrototypeManager.h"

namespace program
{
	class Creator
	{
	public:
		Creator(device::DeviceManager&);
		Creator(const Creator& copy) = delete;
		Creator(Creator&&) = delete;
		~Creator();

		void DevicesFromFile(std::string&);

		//void RulesFromFile(std::string&);
		//void EventsFromFile(std::string&);

		device::IDevice* CreateDevice(std::string&);

		//std::string CreateEvents(std::string&);

	private:
		std::list<std::string> _ReadLines(std::string&);

		//std::string _CreateEvent(std::string&, std::istream&);

		device::DeviceManager& _devManager;
		prototype::PrototypeManager _prototypeManager;
	};
}
#endif // !CREATOR_H
