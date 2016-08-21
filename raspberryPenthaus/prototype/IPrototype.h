#ifndef IPROTOTYPE_H
#define IPROTOTYPE_H
#include <string>
#include <iostream>

#include <boost/property_tree/ptree.hpp>

namespace prototype
{
	class IPrototype
	{
	public:
		virtual ~IPrototype() { }

		//TO DO:: usunac is initialized
		virtual bool IsInitialized() const = 0;

		virtual void SaveToTree(boost::property_tree::ptree&, const std::string&) const = 0;
		virtual bool LoadFromTree(boost::property_tree::ptree::value_type&) = 0;

		virtual IPrototype* Clone() const = 0;
	};
}

#endif // !IPROTOTYPE_H