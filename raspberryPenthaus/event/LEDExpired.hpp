#ifndef LED_EXPIRED_H
#define LED_EXPIRED_H
#include <sstream>

#include "IEvent.h"
#include "../device/LED.h"

namespace event
{
	class LEDExpired : public IEvent
	{
	public:
		LEDExpired(device::LED& led)
			: led_(led), time_(std::chrono::system_clock::now())
		{

		}

		virtual ~LEDExpired()
		{

		}

		virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << print::TimeToString(time_);
			ss << " LEDExpired";
			ss << " device ID: " << led_.GetID();

			return ss.str();
		}

		virtual unsigned int GetDeviceID()
		{
			return led_.GetID();
		}

		virtual timePoint GetTime()
		{
			return time_;
		}

	private:
		device::LED& led_;
		const timePoint time_;
	};
}

#endif // LED_EXPIRED_H
