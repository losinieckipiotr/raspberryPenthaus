#ifndef WP_MOTION_SENSOR_H
#define WP_MOTION_SENSOR_H
#include <random>
#include <chrono>

#include "../MotionSensor.h"
#include "../../wp.h"

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

			//virtual void Check();

			//virtual unsigned int GetCheckInterval() { return 1; };

			virtual unsigned int GetReadInterval() { return MOTION_INTERVAL; };

			//virtual void ReadDefault();
			//virtual void WriteDefault();

			virtual void Setup();

			static const WP_MotionSensor prototype;

		protected:
			virtual bool _Read();

		private:
			#ifndef WP
			void pinMode(int i, int j)
			{

			}

			static std::default_random_engine gen;
			static std::bernoulli_distribution dist;
			
			int digitalRead(int pin)
			{
				return dist(gen);
			}
			#endif
		};
	}
}

#endif // !WP_MOTION_SENSOR_H
