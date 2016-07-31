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

		virtual void SaveToTree(boost::property_tree::ptree&, const std::string&) const;
		virtual bool LoadFromTree(boost::property_tree::ptree::value_type&);

		virtual event::eventPtr Read();

		virtual std::string Execute(std::string&);

		static const std::string name;

	protected:
		virtual bool _Read() = 0;

		//TO DO: WYWALIC STATE I LOGIC I ZOSTWIC
		//TYLKO TO

		int _pin;
		bool _logic;
		int _state;

		MotionSensorReadVal _myVal;
	};
}

#endif // !MOTION_SENSOR_H
