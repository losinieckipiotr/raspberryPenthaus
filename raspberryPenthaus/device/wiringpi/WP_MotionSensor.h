#ifndef WP_MOTION_SENSOR_H
#define WP_MOTION_SENSOR_H

#include "../MotionSensor.h"

namespace device
{
	namespace wiringpi
	{
		class WP_MotionSensor : public device::MotionSensor
		{
		public:
			WP_MotionSensor(int, int, bool = true);
			virtual ~WP_MotionSensor();

			virtual prototype::IPrototype* Clone() const;

			virtual unsigned int GetReadInterval();

			virtual void Setup();

			static const WP_MotionSensor prototype;

		protected:
			virtual bool _Read();
		};
	}
}

#endif // !WP_MOTION_SENSOR_H
