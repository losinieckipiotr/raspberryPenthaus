#include <chrono>

#include "WP_MotionSensor.h"
#include "../../config.h"

#ifdef SYMULATOR
#define	INPUT 0
#include <random>
static std::default_random_engine gen =
    std::default_random_engine((unsigned)std::chrono::system_clock()
        .now().time_since_epoch().count());
static std::bernoulli_distribution dist =
    std::bernoulli_distribution(0.4);

static void pinMode(int i, int j)
{

}

static int digitalRead(int pin)
{
    return dist(gen);
}
#else
#include <wiringPi.h>
#endif //!SYMULATOR

using namespace device;
using namespace wiringpi;
using namespace prototype;
using namespace std;

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

unsigned int WP_MotionSensor::GetReadInterval()
{
    return MOTION_INTERVAL;
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
