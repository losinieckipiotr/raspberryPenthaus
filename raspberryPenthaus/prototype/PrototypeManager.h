#ifndef PROTOTYPE_MANAGER
#define PROTOTYPE_MANAGER
#include <map>
#include <string>

#include "IPrototype.h"

namespace prototype
{
	class PrototypeManager
	{
	public:
		PrototypeManager();
		~PrototypeManager();

		IPrototype* CreatePrototype(std::string&);

	private:
		std::map<const std::string, const IPrototype*> _prototypes;
	};
}

#endif // !PROTOTYPE_MANAGER
