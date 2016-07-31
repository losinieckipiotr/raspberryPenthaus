#ifndef DEVICE_READER_H
#define DEVICE_READER_H
#include <map>
#include <vector>
#include <thread>
#include <memory>

#include "../ItemsPool.hpp"
#include "../event/IEvent.h"
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
		void StartRead();
		void StopRead();

	private:
		void ReadLoop(unsigned int interval, std::vector<device::IReadable*>& devs);
		void ReadDev(device::IReadable *dev);

        bool readFlag;
        const unsigned int baseInterval;

		device::DeviceManager& devMan_;
		ItemsPool<event::eventPtr>& eventPool_;

		std::map<const unsigned int, std::vector<device::IReadable*>> intrvMap_;
		std::vector<std::thread> readThreads;
	};
}

#endif // !DEVICE_READER_H
