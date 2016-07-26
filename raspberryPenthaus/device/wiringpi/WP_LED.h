#ifndef WP_LED_H
#define WP_LED_H
#include "../LED.h"
#include "../../wp.h"

namespace device
{
	namespace wiringpi
	{
		class WP_LED : public device::LED
		{
		public:
			WP_LED(int, int, int, bool = true);
			virtual ~WP_LED();

			virtual prototype::IPrototype* Clone() const;

			virtual void WriteDefault();

			virtual void Setup();

			static const WP_LED prototype;

		protected:
			virtual void _Write(bool val);

		private:
			#ifndef WP
			void digitalWrite(int i, int k)
			{

			}

			void pinMode(int i, int j)
			{

			}
			#endif
		};
	}
}

#endif // !WP_LED_H