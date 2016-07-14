#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
#include <string>
#include "DeviceBase.h"

namespace gpio
{
	class LightSensor : public DeviceBase
	{
	public:
		LightSensor(int);
		virtual ~LightSensor() { }

		virtual std::string ToString() const;

		virtual void Save(std::ostream&) const;
		virtual bool Load(std::string&);

		virtual std::string Execute(std::string&);

		double GetLight() { return _lux; }

		static const std::string name;

	protected:
		
		double _ch0;
		double _ch1;
		double _lux;
	};
}

#endif // !LIGHT_SENSOR_H