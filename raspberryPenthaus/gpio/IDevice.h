#ifndef IDEVICE_H
#define IDEVICE_H
#include <fstream>

#include "../prototype/PrototypeBase.h"

namespace gpio
{
	class IDevice : public prototype::PrototypeBase
	{
	public:
		virtual ~IDevice() { }

		virtual void Check() = 0;

		virtual void ReadDefault() = 0;
		virtual void WriteDefault() = 0;
		virtual void Setup() = 0;

		virtual int GetID() = 0;
		virtual unsigned int GetCheckInterval() = 0;

		virtual std::string Execute(std::string&) = 0;
	};
}

#endif // !IDEVICE_H