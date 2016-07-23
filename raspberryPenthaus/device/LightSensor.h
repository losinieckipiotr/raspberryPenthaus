#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
#include <string>

#include "DeviceBase.h"
#include "IReadable.h"

namespace device
{
	class LightSensorReadVal : public IReadVal
	{
	public:
		LightSensorReadVal() { }
		LightSensorReadVal(double val) : val(val) { }
		virtual ~LightSensorReadVal() { }

		operator double() { return val; }

		double val;
	};

	class LightSensor : public DeviceBase, public IReadable
	{
	public:
		LightSensor(int);
		virtual ~LightSensor() { }

		//virtual std::string ToString() const;

		virtual void Save(std::ostream&) const;
		virtual bool Load(std::string&);

		//virtual bool IsReadable() { return true; };

		virtual IReadVal& Read();

		virtual std::string Execute(std::string&);

		//double GetLight() const { return _lux; }

		static const std::string name;

	protected:
		virtual double _Read() = 0;

		LightSensorReadVal _myVal;

		/*double _ch0;
		double _ch1;
		double _lux;*/
	};
}

#endif // !LIGHT_SENSOR_H