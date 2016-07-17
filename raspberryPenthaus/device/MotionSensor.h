#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H
#include <string>
#include "DeviceBase.h"

namespace gpio
{
	class MotionSensor : public DeviceBase
	{
	public:
		MotionSensor(int, int, bool = true);
		virtual ~MotionSensor() { }

		virtual std::string ToString() const;

		virtual void Save(std::ostream&) const;
		virtual bool Load(std::string&);

		virtual std::string Execute(std::string&);

		bool IsMotionDetected();

		static const std::string name;

	protected:
		
		int _pin;
		bool _logic;
		int _state;
	};
}

#endif // !MOTION_SENSOR_H
