#include <vector>

#include "MiLightBulb.h"

using namespace std;

using namespace device;
using namespace milight;
using namespace prototype;

const MiLightBulb MiLightBulb::prototype(-1, 0);

unsigned int MiLightBulb::_resends = 1;
unsigned int MiLightBulb::_seqLength = 16;

MiLightPilot MiLightBulb::_pilot;

MiLightBulb::MiLightBulb(int id, unsigned int delay)
	: Bulb(id ,delay)
{

}

MiLightBulb::~MiLightBulb()
{

}

IPrototype* MiLightBulb::Clone() const
{
	return new MiLightBulb(prototype);
}

void MiLightBulb::WriteDefault()
{
	_state = 0;
	_Write(false);
}

void MiLightBulb::Setup()
{
	_pilot.Init();
	vector<uint8_t> id = { 0xB0, 0xAB, 0x46 };
	_pilot.SetID(id);
	_pilot.SetColor(0x00);
	_pilot.SetLightInPerc(100);
	_pilot.SetSeq(0x00);
	WriteDefault();
}

void MiLightBulb::_Write(bool val)
{
	if (val)
		_pilot.SendCH1_ON(false, _resends, _seqLength);
	else
		_pilot.SendCH1_OFF(false, _resends, _seqLength);
}
