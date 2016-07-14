#ifndef IACTION_H
#define IACTION_H
#include "IEvent.h"

namespace rule
{
	class IAction : public IEvent
	{ 
	public:
		virtual ~IAction() { }

		virtual void Execute() = 0;
	};
}

#endif // !IACTION_H
