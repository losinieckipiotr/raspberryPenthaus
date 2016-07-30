#ifndef CREATOR_H
#define CREATOR_H
#include <list>
#include <string>

#include "IDriver.h"
#include "../device/DeviceManager.h"

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

	private:
		device::DeviceManager& _devManager;
	};
}
#endif // !CREATOR_H
