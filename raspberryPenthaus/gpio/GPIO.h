#ifndef GPIO_H
#define GPIO_H
#include <map>
#include <vector>
#include <mutex>
#include <string>

#include "../config.h"

namespace device
{
	class DeviceManager;
}

namespace gpio
{
	class GPIO
	{
	public:
		static GPIO* Instance(device::DeviceManager &man);
		GPIO(const GPIO&) = delete;
		GPIO(GPIO&&) = delete;

		void TurnOff();

		void SetupGPIO();
		void WriteDefaultAll();

	private:
		GPIO(device::DeviceManager &man);
		~GPIO();

		#ifndef WP
		void wiringPiSetup()
		{

		}
		#endif

		static GPIO* _instance;

		device::DeviceManager& man;

		//unsigned int _checkCounter;

		//std::vector<std::pair<int, device::IDevice*>> _checkTable;
	};
}

#endif // !GPIO_H