#ifndef IEVENT_H
#define IEVENT_H
#include <chrono>
#include <memory>

#include"../print/IToString.h"

namespace event
{
	typedef std::chrono::time_point<std::chrono::system_clock> timePoint;

	class IEvent : public print::IToString
	{
	public:
		virtual ~IEvent() { }

		virtual unsigned int GetDeviceID() = 0;
		virtual timePoint GetTime() = 0;
	};
	typedef std::shared_ptr<IEvent> eventPtr;
}

#endif // !IEVENT_H