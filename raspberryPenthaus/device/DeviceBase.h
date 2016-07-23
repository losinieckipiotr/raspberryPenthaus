#ifndef DEVICE_BASE_H
#define DEVICE_BASE_H
#include "IDevice.h"

namespace device
{
	class DeviceBase : public IDevice
	{
	public:
		DeviceBase(int id) : _id(id) { }
		virtual ~DeviceBase() { }

		virtual const int GetID() { return _id; }

	protected:
		int _id;
	};
}

#endif // !DEVICE_BASE_H
