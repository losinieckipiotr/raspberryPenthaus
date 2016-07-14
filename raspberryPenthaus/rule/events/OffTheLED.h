#ifndef OFF_THE_LED_H
#define OFF_THE_LED_H
#include "../IAction.h"
#include "../../gpio/LED.h"

namespace rule
{
	namespace events
	{
		class OffTheLED : public IAction
		{
		public:
			OffTheLED();
			virtual ~OffTheLED();

			virtual std::string ToString() const;
			
			virtual void Save(std::ostream&) const;
			virtual bool Load(std::string&);

			virtual prototype::IPrototype* Clone() const;

			virtual bool AttachDevice(gpio::IDevice*);
			virtual int GetDeviceID(){ return _devID; }

			virtual void Execute();

			static const std::string name;
			static const OffTheLED prototype;

		private:
			int _devID;
			gpio::LED* _led;
		};
	}
}

#endif // !OFF_THE_LED_H
