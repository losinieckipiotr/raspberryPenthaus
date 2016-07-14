#ifndef GPIO_H
#define GPIO_H
#include <map>
#include <vector>
#include <mutex>
#include <string>
#include "IDevice.h"
#include "../wp.h"

namespace gpio
{
	class GPIO
	{
	public:
		static GPIO* Instance();
		GPIO(const GPIO&) { }
		GPIO(const GPIO&&) { }

		void TurnOff();

		void AddDevice(IDevice*);
		void AddNewDevice(IDevice*);
		IDevice* GetDevice(int id);

		void SetupGPIO();
		void CheckAll();

		void ReadDefaultAll();
		void WriteDefaultAll();
		std::string PrintDevices();
		void SaveDevices(std::string& filename);

	private:
		GPIO();
		~GPIO();

		static bool _DeviceComp(IDevice* a, IDevice* b)
		{
			return a->GetID() < b->GetID();
		}

		static GPIO* _instance;
		unsigned int _checkCounter;

		std::map<int, IDevice*, std::less<int>> _devices;
		std::map<int, IDevice*, std::less<int>> _newDevices;
		std::vector<std::pair<int, IDevice*>> _checkTable;

		#ifndef WP
		void wiringPiSetup()
		{

		}
		#endif
	};
}

#endif // !GPIO_H