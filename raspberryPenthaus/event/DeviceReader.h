#ifndef DEVICE_READER_H
#define DEVICE_READER_H

namespace gpio
{
	class IDevice;
}

namespace events
{
	//TO DO: DeviceManager

	class DeviceReader
	{
	public:
		DeviceReader();
		virtual ~DeviceReader();

		void AddDevice(gpio::IDevice* dev);
		void CheckAll();

	private:

	};
}

#endif // !DEVICE_READER_H
