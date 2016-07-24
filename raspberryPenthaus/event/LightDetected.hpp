#ifndef LIGHT_DETECTED_H
#define LIGHT_DETECTED_H
#include <sstream>

#include "IEvent.h"
#include "../device/LightSensor.h"

namespace event
{
	class LightDetected : public IEvent
	{
	public:
		LightDetected(const device::LightSensorReadVal readVal)
			: readVal_(readVal), time_(std::chrono::system_clock::now())
		{

		}

		virtual ~LightDetected()
		{

		}

		virtual std::string ToString() const
		{
			std::stringstream ss;
			ss.precision(2);
			ss << "LightDetected ";
			ss << "device ID: " << readVal_.deviceID;
			ss << " lux: " << std::fixed << readVal_.val;
			ss << " threshold: " << (readVal_.threshold) << " ";
			ss << print::TimeToString(time_);
			return ss.str();

			/*std::string s("LightDetected ");
			s += "device ID: " + std::to_string(readVal_.deviceID);
			s += " lux: " + std::to_string(readVal_.val);
			s += " threshold: " + std::to_string(readVal_.threshold);
			s += " " + print::TimeToString(time_);
			return s;*/
		}

		virtual unsigned int GetDeviceID()
		{
			return readVal_.deviceID;
		}

		virtual timePoint GetTime()
		{
			return time_;
		}

	private:
		const device::LightSensorReadVal readVal_;
		const timePoint time_;
	};
}

#endif // !LIGHT_DETECTED_H
