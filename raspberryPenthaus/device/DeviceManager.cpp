#include <stdexcept>
#include <exception>
#include <fstream>
#include <string>

#include <boost/property_tree/xml_parser.hpp>

#include "DeviceManager.h"

using namespace device;
using namespace std;

namespace pt = boost::property_tree;

DeviceManager::DeviceManager()
{

}

DeviceManager::~DeviceManager()
{

}

void DeviceManager::AddDevice(IDevice *dev)
{
	if (dev == nullptr)
		throw runtime_error("Attempt to add nullptr to DeviceManager");
	auto temp = devices_.find(dev->GetID());
	if (temp != devices_.end())
		throw runtime_error("Attemp to add device with non-unique ID");
	devices_[dev->GetID()] = dev;
}

IDevice* DeviceManager::GetDevice(const int id) const
{
	auto temp = devices_.find(id);
	if (temp == devices_.end())
		return nullptr;
	return (*temp).second;
}

std::vector<IDevice*> DeviceManager::GetAllDevices() const
{
	std::vector<IDevice*> devs;
	devs.reserve(devices_.size());
	for (auto &dev : devices_)
	{
		devs.push_back(dev.second);
	}
	return devs;
}

void device::DeviceManager::DeleteAllDevices()
{
	for (auto& dev : devices_)
	{
		delete dev.second;
	}
	devices_.clear();
}

std::vector<IReadable*> DeviceManager::GetReadDevices() const
{
	std::vector<IReadable*> readDevs;
	for (auto &pair : devices_)
	{
		if (IReadable* dev = dynamic_cast<IReadable*>(pair.second))
			readDevs.push_back(dev);
	}
	return readDevs;
}

std::vector<IWriteable*> DeviceManager::GetWriteDevices() const
{
	std::vector<IWriteable*> readDevs;
	for (auto &pair : devices_)
	{
		if (IWriteable* dev = dynamic_cast<IWriteable*>(pair.second))
			readDevs.push_back(dev);
	}
	return readDevs;
}

//TO DO: Wypisywanie urzadzen w jakims formacie
//XML?
//JSON?
std::string DeviceManager::PrintDevices() const
{
	string s("Plugged devices:\n\n");
	for (auto& dev : devices_)
	{
		s += dev.second->ToString() + "\n";
	}
	s += "\n";
	return s;
}

