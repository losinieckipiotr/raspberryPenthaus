#include "WP_MotionSensor.h"

using namespace device;
using namespace wiringpi;
using namespace prototype;
using namespace std;

#ifndef WP
#define	INPUT 0

default_random_engine WP_MotionSensor::gen(
	(unsigned)chrono::system_clock()
	.now().time_since_epoch().count());
bernoulli_distribution WP_MotionSensor::dist(0.1);

#endif // !WP

const WP_MotionSensor WP_MotionSensor::prototype(-1, -1, false);

WP_MotionSensor::WP_MotionSensor(int id, int pin,bool logic)
	: MotionSensor(id, pin, logic)
{

}

WP_MotionSensor::~WP_MotionSensor()
{

}

IPrototype* WP_MotionSensor::Clone() const
{
	return new WP_MotionSensor(prototype);
}

bool WP_MotionSensor::_Read()
{
	_state = digitalRead(_pin);
	return (_state > 0);
}

void WP_MotionSensor::Setup()
{
	pinMode(_pin, INPUT);
	MotionSensor::Read();
}