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

		//zmienic Chceck na Read
		//virtual void Check() = 0;

		//unsigned int GetCheckInterval(); ???

		//virtual void ReadDefault() = 0;
		//virtual void WriteDefault() = 0;

		virtual void Setup() = 0;
		virtual const int GetID() = 0;

		//virtual bool IsReadable() = 0;

		virtual std::string Execute(std::string&) = 0;
	};
}

#endif // !IDEVICE_H