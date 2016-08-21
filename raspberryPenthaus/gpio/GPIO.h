#ifndef GPIO_H
#define GPIO_H
#include <map>
#include <vector>
#include <mutex>
#include <string>

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

		static GPIO* _instance;

		device::DeviceManager& man;
	};
}

#endif // !GPIO_H
