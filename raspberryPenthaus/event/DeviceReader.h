#ifndef DEVICE_READER_H
#define DEVICE_READER_H

namespace gpio
{
	class IDevice;
}

namespace events
{

	class DeviceReader
	{
	public:
		DeviceReader();
		virtual ~DeviceReader();

		void AddDevice(gpio::IDevice*);

	private:

	};
}

#endif // !DEVICE_READER_H
