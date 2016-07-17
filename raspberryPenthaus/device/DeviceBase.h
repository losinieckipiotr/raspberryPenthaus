#ifndef DEVICE_BASE_H
#define DEVICE_BASE_H
#include "IDevice.h"

namespace gpio
{
	class DeviceBase : public IDevice
	{
	public:
		DeviceBase(int id) : _id(id) { }
		virtual ~DeviceBase() { }

		virtual int GetID() { return _id; }
		virtual unsigned int GetCheckInterval() { return 0; }

	protected:
		int _id;
	};
}

#endif // !DEVICE_BASE_H
