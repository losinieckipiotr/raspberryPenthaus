#include <chrono>

#include "WP_LED.h"
#include "../../config.h"

using namespace device;
using namespace wiringpi;
using namespace prototype;
using namespace std;

#ifndef WP
#define	OUTPUT 1
void digitalWrite(int i, int k)
{

}

void pinMode(int i, int j)
{

}
#endif // !WP

const WP_LED WP_LED::prototype(-1, -1, 0, false);

WP_LED::WP_LED(int id, int pin, int delay, bool logic)
	: LED(id, pin, delay, logic)
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