#ifndef IDEVICE_H
#define IDEVICE_H
#include <fstream>

#include "../prototype/PrototypeBase.h"

namespace device
{
	class IDevice : public prototype::PrototypeBase
	{
	public:
		virtual ~IDevice() { }

		virtual void Setup() = 0;
		virtual const int GetID() = 0;

		virtual std::string Execute(std::string&) = 0;
	};
}

#endif // !IDEVICE_H