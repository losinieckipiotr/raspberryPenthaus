#ifndef IPROTOTYPE_H
#define IPROTOTYPE_H
#include <string>
#include <iostream>

namespace prototype
{
	class IPrototype
	{
	public:
		virtual ~IPrototype() { }

		virtual bool IsInitialized() const = 0;

		virtual void Save(std::ostream&) const = 0;
		virtual bool Load(std::string&) = 0;

		virtual IPrototype* Clone() const = 0;
	};
}

#endif // !IPROTOTYPE_H