#ifndef ITRIGGER_H
#define ITRIGGER_H
#include "IEvent.h"

namespace rule
{
	class ITrigger : public IEvent
	{
	public:
		virtual ~ITrigger() { }

		virtual bool IsActivated() = 0;
	};
}

#endif // !ITRIGGER_H
