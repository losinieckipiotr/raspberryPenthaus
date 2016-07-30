#ifndef CREATOR_H
#define CREATOR_H
#include <iostream>
#include <list>
#include <string>
#include <list>

#include "IDriver.h"
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

		std::list<IDriver*> DriversFromFile(const std::string&);
		void DevicesFromFile(std::string&);
		void DevicesFromXML(const std::string&);

		device::IDevice* CreateDevice(std::string&);

	private:
		std::list<std::string> _ReadLines(std::string&);

		device::DeviceManager& _devManager;
		prototype::PrototypeManager _prototypeManager;
	};
}
#endif // !CREATOR_H
