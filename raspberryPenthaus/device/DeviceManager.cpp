#include <stdexcept>
#include <fstream>
#include <string>

#include "DeviceManager.h"

using namespace device;
using namespace std;

DeviceManager::DeviceManager()
{

}

DeviceManager::~DeviceManager()
{
	for (auto& dev : devices_)
	{
		delete dev.second;
	}
}

void DeviceManager::AddDevice(IDevice *dev)
{
	if (dev == nullptr)
		throw runtime_error("Attempt to add nullptr to GPIO");
	auto temp = devices_.find(dev->GetID());
	if (temp != devices_.end())
		throw runtime_error("Attemp to add device with non-unique ID");
	devices_[dev->GetID()] = dev;
}

//Niebezpieczna metoda! Zawsze dodaæ block try catch
IDevice* DeviceManager::GetDevice(const int id) const
{
	auto temp = devices_.find(id);
	if (temp == devices_.end())
		throw runtime_error("Invalid device id");
	return (*temp).second;
}

std::vector<IDevice*> DeviceManager::GetAllDevices() const
{
	//return std::vector<IDevice*>(devices_.begin(), devices_.end());
	std::vector<IDevice*> devs;
	devs.reserve(devices_.size());
	for (auto &dev : devices_)
	{
		devs.push_back(dev.second);
	}
	return devs;
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

//TO DO: Zapisywanie urzadzen w jakims formacie
//XML?
//JSON?
void DeviceManager::SaveDevices(std::string &filename) const
{
	ofstream file(filename, ofstream::trunc);
	for (auto& dev : devices_)
	{
		file << "create ";
		dev.second->Save(file);
		file << endl;
	}
	file.close();
}
