#ifndef IREADABLE_H
#define IREADABLE_H
#include <memory>

namespace event
{
	class IEvent;
}

namespace device
{
	class IReadable
	{
	public:
		virtual ~IReadable() { }

		virtual std::shared_ptr<event::IEvent> Read() = 0;
		virtual unsigned int GetReadInterval() = 0;
	};
}

#endif // !IREADABLE_H
