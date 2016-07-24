#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H
#include <string>
#include "DeviceBase.h"
#include "IReadable.h"

namespace device
{
	struct MotionSensorReadVal
	{
	public:
		MotionSensorReadVal(
			bool val,
			unsigned int deviceID)
				: val(val), deviceID(deviceID)
		{ }

		bool val;
		unsigned int deviceID;
	};

	class MotionSensor : public DeviceBase, public IReadable
	{
	public:
		MotionSensor(int, int, bool = true);
		virtual ~MotionSensor() { }

		virtual std::string ToString() const;

		virtual void Save(std::ostream&) const;
		virtual bool Load(std::string&);

		virtual std::shared_ptr<event::IEvent> Read();

		virtual std::string Execute(std::string&);

		static const std::string name;

	protected:
		virtual bool _Read() = 0;

		//TO DO: WYWALIC STATE I LOGIC I ZOSTWIC
		//TYLKO TO
		MotionSensorReadVal _myVal;

		int _pin;
		bool _logic;
		int _state;
	};
}

#endif // !MOTION_SENSOR_H
