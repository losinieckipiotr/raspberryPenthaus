#include <cmath>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <iomanip>//setprecision

#include "../../gpio/GPIO.h"
#include "WP_TSL2561.h"

using namespace device;
using namespace wiringpi;
using namespace prototype;
using namespace std;

#ifdef WP
#include <wiringPiI2C.h>
#else
default_random_engine WP_LightSensorTSL2561::gen(
	(unsigned)chrono::system_clock()
	.now().time_since_epoch().count());
binomial_distribution<int> WP_LightSensorTSL2561::d0(0xFFFF, 0.01);
binomial_distribution<int> WP_LightSensorTSL2561::d1(0xFFFF, 0.001);
#endif // WP

const WP_LightSensorTSL2561 WP_LightSensorTSL2561::prototype(-1);

WP_LightSensorTSL2561::WP_LightSensorTSL2561(int id)
	: LightSensor(id)
{
	_fd = -1;
	_lux = 0.0;
	_ch0 = 0.0;
	_ch1 = 0.0;
	_gain = Gain::X16;
	_intTiming = IntegrationTiming::MS_402;
}

WP_LightSensorTSL2561::~WP_LightSensorTSL2561()
{
	PowerOff();
}

IPrototype* WP_LightSensorTSL2561::Clone() const
{
	return new WP_LightSensorTSL2561(prototype);
}

string WP_LightSensorTSL2561::ToString() const
{
	stringstream ss;
	ss << fixed << setprecision(2);
	ss << name << "\tID " << _id;
	ss << "\tI2C";
	if (_lux == numeric_limits<double>::max())
	{
		ss << "\tLUX Out of range!";
	}
	else
	{
		ss << "\tLUX " << _lux;
	}
	ss << "\tVIS+IR " << _ch0;
	ss << "\tIR " << _ch1;
	return ss.str();
}

double WP_LightSensorTSL2561::_Read()
{
	_ch0 = wiringPiI2CReadReg16(_fd, COMMAND_BIT | CHAN0_LOW_REGISTER);
	if (_ch0 < 0)
		throw runtime_error("Error in writting I2C");
	_ch1 = wiringPiI2CReadReg16(_fd, COMMAND_BIT | CHAN1_LOW_REGISTER);
	if (_ch1 < 0)
		throw runtime_error("Error in writting I2C");

	double ratio;

	if ((_ch0 == 0xFFFF) || (_ch1 == 0xFFFF))//przekroczenie zakresu pomiarowego
	{
		return _lux = numeric_limits<double>::max();
	}

	ratio = _ch1 / _ch0;

	// Normalize for integration time
	if (_intTiming == IntegrationTiming::MS_13)
	{
		_ch0 *= (402.0 / 13.7);
		_ch1 *= (402.0 / 13.7);
	}
	else if (_intTiming == IntegrationTiming::MS_101)
	{
		_ch0 *= (402.0 / 101);
		_ch1 *= (402.0 / 101);
	}

	// Normalize for gain
	if (_gain == Gain::X1)
	{
		_ch0 *= 16;
		_ch1 *= 16;
	}

	// Determine lux per datasheet equations:
	if (ratio < 0.5)
	{
		_lux = 0.0304 * _ch0 - 0.062 * _ch0 * pow(ratio, 1.4);
	}
	else if (ratio < 0.61)
	{
		_lux = 0.0224 * _ch0 - 0.031 * _ch1;
	}
	else if (ratio < 0.80)
	{
		_lux = 0.0128 * _ch0 - 0.0153 * _ch1;
	}
	else if (ratio < 1.30)
	{
		_lux = 0.00146 * _ch0 - 0.00112 * _ch1;
	}
	else// if (ratio > 1.30)
	{
		_lux = 0.0;
	}

	return _lux;
}

//void WP_LightSensorTSL2561::ReadDefault()
//{
//	Check();
//}

//void WP_LightSensorTSL2561::WriteDefault()
//{
//
//}

void WP_LightSensorTSL2561::Setup()
{
	_fd = wiringPiI2CSetup(ADDR_FLOAT);
	if (_fd == -1)
		runtime_error("Cannot Setup I2C");
	PowerOn();
	SetGainAndTiming(_gain, _intTiming);
	_Read();
}

void WP_LightSensorTSL2561::PowerOn()
{
	int code = wiringPiI2CWriteReg8(_fd,
		COMMAND_BIT | CONTROL_REGISTER, CONTROL_POWERON);
	if (code < 0)
		throw runtime_error("Error in writting I2C");
}

void WP_LightSensorTSL2561::PowerOff()
{
	int code = wiringPiI2CWriteReg8(_fd,
		COMMAND_BIT | CONTROL_REGISTER, CONTROL_POWEROFF);
	if (code < 0)
		throw runtime_error("Error in writting I2C");
}

void WP_LightSensorTSL2561::SetGainAndTiming(Gain g, IntegrationTiming it)
{
	int gain, timing;

	switch (g)
	{
	case Gain::X1:
		gain = GAIN_1X;
		break;
	case Gain::X16:
		gain = GAIN_16X;
		break;
	default:
		break;
	}
	switch (it)
	{
	case IntegrationTiming::MS_13:
		timing = INTEGRATIONTIME_13MS;
		break;
	case IntegrationTiming::MS_101:
		timing = INTEGRATIONTIME_101MS;
		break;
	case IntegrationTiming::MS_402:
		timing = INTEGRATIONTIME_402MS;
		break;
	default:
		break;
	}

	int code = wiringPiI2CWriteReg8(_fd, COMMAND_BIT | TIMING_REGISTER, gain | timing);
	if (code < 0)
		throw runtime_error("Error in writting I2C");
}
