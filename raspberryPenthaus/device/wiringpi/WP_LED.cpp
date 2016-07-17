#include <chrono>

#include "../gpio/GPIO.h"
#include "WP_LED.h"

using namespace gpio;
using namespace wiringpi;
using namespace prototype;
using namespace std;

#ifndef WP
#define	OUTPUT 1
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

void WP_LED::ReadDefault()
{

}

void WP_LED::WriteDefault()
{
	_state = _defaultValue;
	_Write(_defaultValue);
}

void WP_LED::Setup()
{
	pinMode(_pin, OUTPUT);
	WriteDefault();
}


void WP_LED::_Write(int val)
{
	digitalWrite(_pin, val);
}