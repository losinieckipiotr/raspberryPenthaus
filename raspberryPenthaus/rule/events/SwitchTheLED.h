#ifndef SWITCH_THE_LED_H
#define SWITCH_THE_LED_H
#include <string>
#include "../IAction.h"
#include "../../device/LED.h"

namespace rule
{
	namespace events
	{
		class SwitchTheLED : public IAction
		{
		public:
			SwitchTheLED();
			virtual ~SwitchTheLED();

			virtual std::string ToString() const;

			virtual void Save(std::ostream&) const;
			virtual bool Load(std::string&);

			virtual prototype::IPrototype* Clone() const;

			virtual bool AttachDevice(device::IDevice*);
			virtual int GetDeviceID(){ return _devID; }

			virtual void Execute();

			static const std::string name;
			static const SwitchTheLED prototype;

		private:
			bool _onOff;
			int _devID;
			device::LED* _led;
		};
	}
}

#endif // !SWITCH_THE_LED_H
