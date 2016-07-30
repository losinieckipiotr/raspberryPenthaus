#ifndef IREADABLE_H
#define IREADABLE_H
#include "../event/IEvent.h"

namespace device
{
	class IReadable
	{
	public:
		virtual ~IReadable() { }

		virtual event::eventPtr Read() = 0;
		virtual unsigned int GetReadInterval() = 0;
	};
}

#endif // !IREADABLE_H
