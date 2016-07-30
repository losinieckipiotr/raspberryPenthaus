#ifndef PROTOTYPE_MANAGER
#define PROTOTYPE_MANAGER
#include <map>
#include <string>

#include <boost/property_tree/xml_parser.hpp>

#include "IPrototype.h"

namespace prototype
{
	class PrototypeManager
	{
	public:
		PrototypeManager();
		~PrototypeManager();

		IPrototype* CreatePrototype(std::string&);
		IPrototype* CreatePrototype(boost::property_tree::ptree::value_type &v);

	private:
		std::map<const std::string, const IPrototype*> _prototypes;
	};
}

#endif // !PROTOTYPE_MANAGER
