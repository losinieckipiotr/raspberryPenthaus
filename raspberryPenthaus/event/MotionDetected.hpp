#ifndef MOTION_DETECTED_H
#define MOTION_DETECTED_H
#include <stdexcept>
#include <sstream>

#include "IEvent.h"
#include "../device/MotionSensor.h"

namespace event
{
	class MotionDetected : public IEvent
	{
	public:
		MotionDetected(device::MotionSensorReadVal readVal)
			: readVal_(readVal), time_(std::chrono::system_clock::now())
		{
			if (readVal.val == false)
				throw std::logic_error("MotionSensorReadVal.val == false");
		}

		virtual ~MotionDetected()
		{

		}

		virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << print::TimeToString(time_);
			ss << " MotionDetected ";
			ss << "device ID: " << readVal_.deviceID;
			ss << " val: " << print::BoolToString(readVal_.val);
			
			return ss.str();
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
		const device::MotionSensorReadVal readVal_;
		const timePoint time_;
	};
}

#endif // !MOTION_DETECTED_H