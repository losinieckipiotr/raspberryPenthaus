#ifndef DUSK_DETECTED_H
#define DUSK_DETECTED_H
#include "../ITrigger.h"
#include "../../device/LightSensor.h"

namespace rule
{
	namespace events
	{
		class DuskDetected : public ITrigger
		{
		public:
			DuskDetected();
			virtual ~DuskDetected();

			virtual std::string ToString() const;
			
			virtual void Save(std::ostream&) const;
			virtual bool Load(std::string&);

			virtual prototype::IPrototype* Clone() const;

			virtual bool AttachDevice(device::IDevice*);
			virtual int GetDeviceID(){ return _devID; }

			virtual bool IsActivated();

			static const std::string name;
			static const DuskDetected prototype;

		private:
			bool _logic;
			int _devID;
			double _threshold;
			device::LightSensor* _sensor;
		};
	}
}

#endif // !DUSK_DETECTED_H
