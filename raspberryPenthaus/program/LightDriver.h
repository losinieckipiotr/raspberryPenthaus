#ifndef LIGHT_DRIVER_H
#define LIGHT_DRIVER_H
#include <memory>

#include "../event/IEvent.h"
#include "../ItemsPool.hpp"
#include"../device/DeviceManager.h"

namespace program
{
	class LightDriver
	{
		enum class State
		{
			Day,
			Night
		};

	public:
		typedef std::shared_ptr<event::IEvent> eventPtr;

		LightDriver(ItemsPool<eventPtr>& pool, device::DeviceManager& devMan);
		virtual ~LightDriver();

		void HandleEvent(eventPtr eventPtr);

	private:
		void Day_(eventPtr eventPtr);
		void Night_(eventPtr eventPtr);
		void Default_(eventPtr eventPtr);


		bool eventHanled;
		State state_;
		ItemsPool<eventPtr>& eventPool_;
		device::DeviceManager& devMan_;
	};
}

#endif // !LIGHT_DRIVER_H
