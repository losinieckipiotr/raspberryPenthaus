#include <sstream>

#include "PrototypeManager.h"

#include "../device/wiringpi/WP_LED.h"
#include "../device/wiringpi/WP_MotionSensor.h"
#include "../device/wiringpi/WP_TSL2561.h"

#include "../device/milight/MiLightBulb.h"

using namespace std;

using namespace prototype;
using namespace device;

namespace pt = boost::property_tree;

PrototypeManager::PrototypeManager()
{
	_prototypes[wiringpi::WP_LED::name] = &wiringpi::WP_LED::prototype;
	_prototypes[wiringpi::WP_MotionSensor::name] = &wiringpi::WP_MotionSensor::prototype;
	_prototypes[wiringpi::WP_LightSensorTSL2561::name] = &wiringpi::WP_LightSensorTSL2561::prototype;

	_prototypes[milight::MiLightBulb::name] = &milight::MiLightBulb::prototype;
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
