#ifndef COMMANDER_H
#define COMMANDER_H
#include <string>

namespace device
{
	class DeviceManager;
}

namespace program
{
	class Commander
	{
	public:
		Commander() = delete;
		Commander(const Commander&) = delete;
		Commander(device::DeviceManager &man);
		~Commander();

		std::string ExecuteCommand(std::string&);

	private:
		device::DeviceManager& man;
	};
}

#endif // !COMMANDER_H
