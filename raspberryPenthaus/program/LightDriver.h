#ifndef LIGHT_DRIVER_H
#define LIGHT_DRIVER_H
#include <memory>
#include <unordered_map>
#include <thread>

#include "IDriver.h"
#include "../ItemsPool.hpp"
#include "../device/DeviceManager.h"
#include "../prototype/PrototypeManager.h"

#include "../device/LED.h"
#include "../device/LightSensor.h"
#include "../device/MotionSensor.h"

#include "../device/Bulb.h"

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

		inline int GetID() { return id_; }

		virtual void HandleEvent(event::eventPtr evPtr);

		virtual void SaveToTree(boost::property_tree::ptree& tree, const std::string&) const;
		virtual bool LoadFromTree(boost::property_tree::ptree::value_type &v);

	private:
		inline bool isItMyDevEvent(int id)
		{
			return (myDevs_.find(id) != myDevs_.end());
		}

		void Day_(event::eventPtr evPtr);
		void Night_(event::eventPtr evPtr);
		void Default_(event::eventPtr evPtr);

		void MotionEventDay_(event::eventPtr evPtr);
		void MotionEventNight_(event::eventPtr evPtr);
		void LightEventDay_(event::eventPtr evPtr);
		void LightEventNight_(event::eventPtr evPtr);
		void ExpiredHandler_(event::eventPtr evPtr);

		void ExpiredCreator(device::IDevice* dev);

		void AddDev_(device::IDevice* dev);

		int id_;
		boost::posix_time::seconds delay_;
		bool eventHanled_;
		State state_;
		ItemsPool<event::eventPtr>& eventPool_;
		device::DeviceManager& devMan_;
		prototype::PrototypeManager protoMan_;

		std::unordered_map<int, device::IDevice*> myDevs_;
		std::unordered_map<int, device::LED*> leds_;
		std::unordered_map<int, device::MotionSensor*> motionSensors_;
		std::unordered_map<int, device::Bulb*> bulbs_;
		device::LightSensor* lightSensor_;

		boost::asio::io_service service_;
		boost::asio::io_service::work work_;

		boost::asio::deadline_timer timer_;
	};
}

#endif // !LIGHT_DRIVER_H
