#ifndef WP_LED_H
#define WP_LED_H
#include "../LED.h"

namespace device
{
	namespace wiringpi
	{
		class WP_LED : public device::LED
		{
		public:
			WP_LED(int, int, bool = true, unsigned int = 0);
			virtual ~WP_LED();

			virtual prototype::IPrototype* Clone() const;

			virtual void WriteDefault();

			virtual void Setup();

			static const WP_LED prototype;

		protected:
			virtual void _Write(bool val);
		};
	}
}

#endif // !WP_LED_H