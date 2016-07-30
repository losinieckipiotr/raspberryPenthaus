#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "Creator.h"
#include "LightDriver.h"

using namespace program;
using namespace device;
using namespace prototype;
using namespace std;

namespace pt = boost::property_tree;

Creator::Creator(DeviceManager &devMan)
	: _devManager(devMan)
{

}

Creator::~Creator()
{

}

list<IDriver*> Creator::DriversFromFile(const string &filename)
{
	list<IDriver*> drivers;
	pt::ptree tree;
	pt::read_xml(filename, tree);

	auto driversNode = tree.get_child("serialize.drivers");
	for (pt::ptree::value_type &v : driversNode)
	{
		if (v.first == "lightdriver")
		{
			IDriver *ld = new LightDriver(_devManager);
			if (ld->LoadFromTree(v))
				drivers.push_back(ld);
			else
				delete ld;
		}
	}

	return drivers;
}
