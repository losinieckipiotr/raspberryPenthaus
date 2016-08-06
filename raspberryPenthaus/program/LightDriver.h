#ifndef LIGHT_DRIVER_H
#define LIGHT_DRIVER_H
#include <memory>
#include <map>
#include <thread>

#include "IDriver.h"
#include "../ItemsPool.hpp"
#include "../device/DeviceManager.h"
#include "../prototype/PrototypeManager.h"

#include "../device/LED.h"
#include "../device/LightSensor.h"
#include "../device/MotionSensor.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/asio.hpp>

namespace program
{
	class LightDriver : public IDriver
	{
		enum class State
		{
			Day,
			Night
		};

	public:
		LightDriver(device::DeviceManager& devMan);
		virtual ~LightDriver();

		virtual void HandleEvent(event::eventPtr evPtr);

		virtual void SaveToTree(boost::property_tree::ptree& tree, const std::string&) const;
		virtual bool LoadFromTree(boost::property_tree::ptree::value_type &v);

	private:
		void Day_(event::eventPtr evPtr);
		void Night_(event::eventPtr evPtr);
		void Default_(event::eventPtr evPtr);

		void AddDev_(device::IDevice* dev);

		void LEDExpiredCreator(device::LED* led);

		bool eventHanled;
		State state_;
		ItemsPool<event::eventPtr>& eventPool_;
		device::DeviceManager& devMan_;
		prototype::PrototypeManager protoMan_;

		std::map<int, device::LED*, std::less<int>> leds_;
		std::map<int, device::MotionSensor*, std::less<int>> motionSensors_;
		device::LightSensor* lightSensor_;

		boost::asio::io_service service_;
		boost::asio::io_service::work work_;

		boost::asio::deadline_timer timer_;
	};
}

#endif // !LIGHT_DRIVER_H
