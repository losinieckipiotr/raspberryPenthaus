#include <chrono>

#include "WP_LED.h"
#include "../../config.h"

#ifdef SYMULATOR
#define	OUTPUT 1
static void digitalWrite(int i, int k)
{

}

static void pinMode(int i, int j)
{

}
#else
#include <wiringPi.h>
#endif

using namespace device;
using namespace wiringpi;
using namespace prototype;
using namespace std;

const WP_LED WP_LED::prototype(-1, -1, false, 0);

WP_LED::WP_LED(int id, int pin, bool logic, unsigned int delay)
	: LED(id, pin, logic, delay)
{

}

WP_LED::~WP_LED()
{

}

IPrototype* WP_LED::Clone() const
{
	return new WP_LED(prototype);
}

void WP_LED::WriteDefault()
{
	_state = _defaultValue;
	bool temp = (_defaultValue > 0);
	_Write(temp);
}

void WP_LED::Setup()
{
	pinMode(_pin, OUTPUT);
	WriteDefault();
}


void WP_LED::_Write(bool val)
{
	int v = static_cast<int>(val);
	digitalWrite(_pin, v);
}
