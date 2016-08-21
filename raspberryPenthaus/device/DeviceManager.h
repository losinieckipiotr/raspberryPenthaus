#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#include <string>
#include <map>
#include <vector>

#include "IDevice.h"
#include "IReadable.h"
#include "IWriteable.h"

namespace device
{
	class DeviceManager
	{
	public:
		DeviceManager();
		DeviceManager(const DeviceManager&) = delete;
		~DeviceManager();

		void AddDevice(IDevice *dev);
		IDevice* GetDevice(const int id) const;
		std::vector<IDevice*> GetAllDevices() const;
		void DeleteAllDevices();

		std::vector<IReadable*> GetReadDevices() const;
		std::vector<IWriteable*> GetWriteDevices() const;

		std::string PrintDevices() const;

	private:
		struct devComp_
		{
			bool operator()(IDevice* a, IDevice* b)
			{
				return a->GetID() < b->GetID();
			}
		};

		std::map<const int, IDevice*, std::less<const int>> devices_;
	};
}

#endif // !DEVICE_MANAGER_H
