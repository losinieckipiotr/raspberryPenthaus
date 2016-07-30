#ifndef IEVENT_HANDLER_H
#define IEVENT_HANDLER_H
#include <memory>
#include "IEvent.h"
#include "../io/StdIO.h"

namespace event {
	class IEventHandler
	{
	public:
		IEventHandler() : nextHandler(nullptr) { }
		virtual ~IEventHandler() { }

		virtual void HandleEvent(event::eventPtr evPtr)
		{
			if (nextHandler != nullptr)
				nextHandler->HandleEvent(evPtr);
			else
				io::StdIO::StandardOutput("Unhandled event: " + evPtr->ToString());
		}

		virtual void AddHandler(IEventHandler *handler)
		{
			nextHandler = handler;
		}

	private:
		IEventHandler* nextHandler;
	};
}

#endif // !IEVENT_HANDLER_H
