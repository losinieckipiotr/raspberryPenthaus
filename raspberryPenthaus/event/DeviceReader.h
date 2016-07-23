#ifndef DEVICE_READER_H
#define DEVICE_READER_H

namespace device
{
	class IDevice;
	//TO DO: DeviceManager

	class DeviceReader
	{
	public:
		DeviceReader();
		DeviceReader(const DeviceReader&) = delete;
		DeviceReader(DeviceReader&&) = delete;
		virtual ~DeviceReader();

		void AddDevice(IDevice &dev);
		void CheckAll();

	private:

	};
}

#endif // !DEVICE_READER_H
