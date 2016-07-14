#ifndef IEVENT_H
#define IEVENT_H
#include <fstream>
#include "../prototype/PrototypeBase.h"
#include "../gpio/IDevice.h"

namespace rule
{
	class IEvent : public prototype::PrototypeBase
	{
	public:
		virtual ~IEvent() { }

		virtual bool AttachDevice(gpio::IDevice*) = 0;
		virtual int GetDeviceID() = 0;
	};
}

#endif // !IEVENT_H
