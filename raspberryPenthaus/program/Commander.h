#ifndef COMMANDER_H
#define COMMANDER_H
#include <string>

namespace program
{
	class Commander
	{
	public:
		Commander();
		~Commander();

		std::string ExecuteCommand(std::string&);
	};
}

#endif // !COMMANDER_H
