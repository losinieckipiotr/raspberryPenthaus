#include <sstream>

#include "PrototypeManager.h"

#include "../device/wiringpi/WP_LED.h"
#include "../device/wiringpi/WP_MotionSensor.h"
#include "../device/wiringpi/WP_TSL2561.h"

using namespace prototype;
using namespace device;
using namespace wiringpi;
using namespace std;

namespace pt = boost::property_tree;

PrototypeManager::PrototypeManager()
{
	_prototypes[WP_LED::name] = &WP_LED::prototype;
	_prototypes[WP_MotionSensor::name] = &WP_MotionSensor::prototype;
	_prototypes[WP_LightSensorTSL2561::name] = &WP_LightSensorTSL2561::prototype;
}

PrototypeManager::~PrototypeManager()
{

}

IPrototype* PrototypeManager::CreatePrototype(pt::ptree::value_type &v)
{
	IPrototype* prot = nullptr;
	string buffer = v.first;
	auto it = _prototypes.find(buffer);
	if (it != _prototypes.end())
	{
		prot = (*it).second->Clone();
		if (!prot->LoadFromTree(v))
		{
			delete prot;
			prot = nullptr;
		}

	}
	return prot;
}
