#ifndef PROTOTYPE_BASE_H
#define PROTOTYPE_BASE_H
#include "IPrototype.h"
#include "../print/IToString.h"

namespace prototype
{
	class PrototypeBase : public IPrototype, public print::IToString
	{
	public:
		PrototypeBase() : _isInit(false) { }
		virtual ~PrototypeBase() { }

		bool IsInitialized() const { return _isInit; }

	protected:
		bool _isInit;
	};
}

#endif // !PROTOTYPE_BASE_H
