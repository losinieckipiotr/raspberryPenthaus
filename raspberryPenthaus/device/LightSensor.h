#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
#include <string>

#include "DeviceBase.h"
#include "IReadable.h"

namespace device
{
	struct LightSensorReadVal
	{
	public:
		LightSensorReadVal(
			double val,
			double threshold,
			unsigned int deviceID)
				: val(val), threshold(threshold), deviceID(deviceID)
		{ }

		bool operator()()
		{
			return val > threshold;
		}

		double val;
		double threshold;
		unsigned int deviceID;
	};

	class LightSensor : public DeviceBase, public IReadable
	{
	public:
		LightSensor(int, double);
		virtual ~LightSensor() { }

		virtual std::string ToString() const;

		virtual void SaveToTree(boost::property_tree::ptree&, const std::string&) const;
		virtual bool LoadFromTree(boost::property_tree::ptree::value_type&);

		virtual event::eventPtr Read();

		virtual std::string Execute(std::string&);

		static const std::string name;

	protected:
		virtual double _Read() = 0;

		double _threshold;
		LightSensorReadVal _myVal;
	};
}

#endif // !LIGHT_SENSOR_H