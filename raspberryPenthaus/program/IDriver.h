#ifndef IDRIVER_H
#define IDRIVER_H
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "../event/IEventHandler.h"



//TO DO: zrobic interfejs ISerializable z metodami jak nizej i przerobic
//IPrototype

namespace program
{
	class IDriver : public event::IEventHandler
	{
	public:
		virtual ~IDriver() { }

		virtual void SaveToTree(boost::property_tree::ptree&, const std::string&) const = 0;
		virtual bool LoadFromTree(boost::property_tree::ptree::value_type&) = 0;
	};
}

#endif // !IDRIVERS_H

