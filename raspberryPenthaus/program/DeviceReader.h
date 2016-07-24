#ifndef DEVICE_READER_H
#define DEVICE_READER_H
#include <map>
#include <vector>
#include <thread>

#include "../device/DeviceManager.h"

namespace program
{
	class DeviceReader
	{
	public:
		DeviceReader() = delete;
		DeviceReader(const DeviceReader&) = delete;
		DeviceReader(DeviceReader&&) = delete;
		DeviceReader(device::DeviceManager& devMan);
		virtual ~DeviceReader();

		void BiuldDeviceMap();
		//void ReadAll();
		void StartRead();
		void StopRead();

	private:
		void ReadLoop(unsigned int interval, std::vector<device::IReadable*>& devs);
		void ReadDev(device::IReadable *dev);

		const unsigned int baseInterval;
		bool readFlag;
		//unsigned int readCounter;

		device::DeviceManager& devMan_;
		std::map<const unsigned int, std::vector<device::IReadable*>> intrvMap_;
		std::vector<std::thread> readThreads;
	};
}

#endif // !DEVICE_READER_H
