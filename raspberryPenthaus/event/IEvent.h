#ifndef IEVENT_H
#define IEVENT_H
#include <chrono>

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
}

#endif // !IEVENT_H