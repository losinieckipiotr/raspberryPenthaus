#ifndef WP_LIGHT_SENSOR_TSL2561_H
#define WP_LIGHT_SENSOR_TSL2561_H
#include <random>
#include <chrono>

#include "../LightSensor.h"
#include "../../config.h"

namespace device
{
	namespace wiringpi
	{
		enum class IntegrationTiming
		{
			MS_13,
			MS_101,
			MS_402
		};

		enum class Gain
		{
			X1,
			X16
		};

		class WP_LightSensorTSL2561 : public device::LightSensor
		{
		public:
			WP_LightSensorTSL2561(int, double);
			virtual ~WP_LightSensorTSL2561();

			virtual prototype::IPrototype* Clone() const;

			virtual unsigned int GetReadInterval() { return LIGHT_INTERVAL; };

			virtual void Setup();

			virtual std::string ToString() const;

			void PowerOn();
			void PowerOff();
			void SetGainAndTiming(Gain, IntegrationTiming);

			static const WP_LightSensorTSL2561 prototype;

		protected:
			virtual double _Read();

			int _fd;
			double _ch0;
			double _ch1;
			double _lux;
			Gain _gain;
			IntegrationTiming _intTiming;

		private:
			static bool _isOn;

			enum Constants
			{
				ADDR_LOW = 0x29,//GND ADDR
				ADDR_FLOAT = 0x39,//Default I2C  ADDR
				ADDR_HIGH = 0x49,//VCC ADDR

				//////////////COMMAND REGISTER no adress//////////////////////

				COMMAND_BIT = 0x80,//Must be 1
				CLEAR_BIT = 0x40,//Interrupt clear 1 to clear
				WORD_BIT = 0x20,//SMB Write/Read Word Protocol
				BLOCK_BIT = 0x10,//Block Write/Read Protocol

				//////////////CONTROL REGISTER 0x00///////////////////////////

				CONTROL_POWERON = 0x03,
				CONTROL_POWEROFF = 0x00,

				//////////////TIMING REGISTER 0x01////////////////////////////

				GAIN_1X = 0x00,
				GAIN_16X = 0x10,
				MANUAL_GAIN = 0x08,
				INTEGRATIONTIME_13MS = 0x00,// 13.7ms
				INTEGRATIONTIME_101MS = 0x01,// 101ms
				INTEGRATIONTIME_402MS = 0x02,// 402ms

				/////////////REGISTER ADRESSES////////////////////////////////

				CONTROL_REGISTER = 0x00,
				TIMING_REGISTER = 0x01,

				/////////////INTERRUPTS///////////////////////////////////////

				THRESHLOWLOW_REGISTER = 0x02,
				THRESHLOWHIGH_REGISTER = 0x03,
				THRESHHIGHLOW_REGISTER = 0x04,
				THRESHHIGHHIGH_REGISTER = 0x05,

				INTERRUPT_REGISTER = 0x06,

				////////////OTHER ADDRESSES///////////////////////////////////

				ID_REGISTER = 0x0A,

				CHAN0_LOW_REGISTER = 0x0C,
				CHAN0_HIGH_REGISTER = 0x0D,
				CHAN1_LOW_REGISTER = 0x0E,
				CHAN1_HIGH_REGISTER = 0x0F
			};

			#ifndef WP
			static std::default_random_engine gen;
			static std::binomial_distribution<int> d0;
			static std::binomial_distribution<int> d1;

			int wiringPiI2CWriteReg8(int i, int j, int k)
			{
				return 1;
			}

			int wiringPiI2CReadReg16(int i, int j)
			{
				if (j == (COMMAND_BIT | CHAN0_LOW_REGISTER))
					return d0(gen);
				if (j == (COMMAND_BIT | CHAN1_LOW_REGISTER))
					return d1(gen);
				return 666;
			}

			int wiringPiI2CSetup(int i)
			{
				return 0;
			}
			#endif
		};
	}
}

#endif // !WP_LIGHT_SENSOR_TSL2561_H
