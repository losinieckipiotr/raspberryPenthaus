#ifndef MOTION_DETECTED_H
#define MOTION_DETECTED_H
#include "../ITrigger.h"
#include "../../gpio/MotionSensor.h"

namespace rule
{
	namespace events
	{
		class MotionDetected : public ITrigger
		{
		public:
			MotionDetected();
			virtual ~MotionDetected();

			virtual std::string ToString() const;
			
			virtual void Save(std::ostream&) const;
			virtual bool Load(std::string&);

			virtual prototype::IPrototype* Clone() const;

			virtual bool AttachDevice(gpio::IDevice*);
			virtual int GetDeviceID(){ return _devID; }

			virtual bool IsActivated();

			static const std::string name;
			static const MotionDetected prototype;

		private:
			bool _logic;
			int _devID;
			gpio::MotionSensor* _sensor;

		};
	}
}

#endif // !MOTION_DETECTED_H
