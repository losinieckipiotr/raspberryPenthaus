#ifndef BULB_EXPIRED_H
#define BULB_EXPIRED_H

#include <sstream>

#include "IEvent.h"
#include "../device/Bulb.h"
#include "../program/LightDriver.h"

namespace event
{
	class LightExpired : public IEvent
	{
	public:
		LightExpired(device::IDevice& dev, program::LightDriver& driver)
			: dev_(dev), driver_(driver), time_(std::chrono::system_clock::now())
		{

		}

		virtual ~LightExpired()
		{

		}

		virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << print::TimeToString(time_);
			ss << " LightExpired";
			ss << " device ID: " << dev_.GetID();
			ss << " LightDriver ID: " << driver_.GetID();
			return ss.str();
		}

		virtual unsigned int GetDeviceID()
		{
			return dev_.GetID();
		}

		virtual timePoint GetTime()
		{
			return time_;
		}

	private:
		device::IDevice& dev_;
		program::LightDriver& driver_;
		const timePoint time_;
	};
}

#endif // !BULB_EXPIRED_H

