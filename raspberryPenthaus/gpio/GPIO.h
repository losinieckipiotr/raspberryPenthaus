#ifndef GPIO_H
#define GPIO_H
#include <map>
#include <vector>
#include <mutex>
#include <string>

#include "../wp.h"

namespace device
{
	class DeviceManager;
}

namespace gpio
{
	//Klasa singleton. Po utworzeniu instancji automatycznie, konfiguruje GPIO
	//oraz urzadzenia ktore znajduja sie  w DeviceManager, tak wiec
	//1. Utworzenie urzadzen (klasa DeviceManager)
	//2. Opoznione utworzenie instancji GPIO (w konstruktorze program dac nullptr)
	//3. Automatyczna konfiguracja wszystkich urzadzen
	
	//Zas zmiany

	class GPIO
	{
	public:
		static GPIO* Instance(device::DeviceManager &man);
		GPIO(const GPIO&) = delete;
		GPIO(GPIO&&) = delete;

		void TurnOff();

		/*void AddDevice(device::IDevice*);
		void AddNewDevice(device::IDevice*);
		device::IDevice* GetDevice(int id);*/

		//void CheckAll();

		//czy to jest potzebne ?
		//void ReadDefaultAll();
		////////////////////////

		void SetupGPIO();
		void WriteDefaultAll();

		/*std::string PrintDevices();
		void SaveDevices(std::string& filename);*/

	private:
		GPIO(device::DeviceManager &man);
		~GPIO();

		/*static bool _DeviceComp(device::IDevice* a, device::IDevice* b)
		{
			return a->GetID() < b->GetID();
		}*/

		#ifndef WP
		void wiringPiSetup()
		{

		}
		#endif

		static GPIO* _instance;

		device::DeviceManager& man;

		//unsigned int _checkCounter;

		/*std::map<int, device::IDevice*, std::less<int>> _devices;
		std::map<int, device::IDevice*, std::less<int>> _newDevices;*/

		//std::vector<std::pair<int, device::IDevice*>> _checkTable;
	};
}

#endif // !GPIO_H