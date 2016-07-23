#include <sstream>
#include "PrototypeManager.h"

#include "../device/wiringpi/WP_LED.h"
#include "../device/wiringpi/WP_MotionSensor.h"
#include "../device/wiringpi/WP_TSL2561.h"

#include "../rule/Rule.h"

#include "../rule/events/DuskDetected.h"
#include "../rule/events/MotionDetected.h"
#include "../rule/events/OffTheLED.h"
#include "../rule/events/SwitchTheLED.h"

using namespace prototype;
using namespace device;
using namespace wiringpi;
using namespace rule;
using namespace events;
using namespace std;

PrototypeManager::PrototypeManager()
{
	_prototypes[WP_LED::name] = &WP_LED::prototype;
	_prototypes[WP_MotionSensor::name] = &WP_MotionSensor::prototype;
	_prototypes[WP_LightSensorTSL2561::name] = &WP_LightSensorTSL2561::prototype;

	_prototypes[Rule::name] = &Rule::prototype;

	_prototypes[DuskDetected::name] = &DuskDetected::prototype;
	_prototypes[MotionDetected::name] = &MotionDetected::prototype;
	_prototypes[OffTheLED::name] = &OffTheLED::prototype;
	_prototypes[SwitchTheLED::name] = &SwitchTheLED::prototype;
}

PrototypeManager::~PrototypeManager()
{

}

IPrototype* PrototypeManager::CreatePrototype(std::string& line)
{
	IPrototype* prot = nullptr;
	string buffer;
	stringstream ss(line);
	ss >> buffer;
	auto it = _prototypes.find(buffer);
	if (it != _prototypes.end())
	{
		prot = (*it).second->Clone();
		if (!prot->Load(line))
		{
			delete prot;
			prot = nullptr;
		}
	}
	return prot;
}