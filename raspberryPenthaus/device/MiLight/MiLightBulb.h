#ifndef MI_LIGHT_BULB
#define MI_LIGHT_BULB
#include "../Bulb.h"
#include "MiLightPilot.h"

namespace device
{
	namespace milight
	{
		class MiLightBulb : public device::Bulb
		{
		public:
			MiLightBulb(int, unsigned int);
			virtual ~MiLightBulb();

			virtual prototype::IPrototype* Clone() const;

			virtual void WriteDefault();

			virtual void Setup();

			static const MiLightBulb prototype;

		protected:
			virtual void _Write(bool val);

			static unsigned int _resends;
			static unsigned int _seqLength;
			static MiLightPilot _pilot;
		};
	}
}

#endif // !MI_LIGHT_BULB

