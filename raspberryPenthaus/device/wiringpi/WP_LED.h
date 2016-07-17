#ifndef WP_LED_H
#define WP_LED_H
#include "../gpio/LED.h"
#include "../wp.h"

namespace gpio
{
	namespace wiringpi
	{
		class WP_LED : public gpio::LED
		{
		public:
			WP_LED(int, int, int, bool = true);
			virtual ~WP_LED();

			virtual prototype::IPrototype* Clone() const;

			virtual void ReadDefault();
			virtual void WriteDefault();
			virtual void Setup();

			static const WP_LED prototype;

		protected:
			virtual void _Write(int);

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