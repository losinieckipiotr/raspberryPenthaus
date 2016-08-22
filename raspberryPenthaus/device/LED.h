#ifndef LED_H
#define LED_H
#include <string>
#include <chrono>

#include "DeviceBase.h"
#include "IWriteable.h"

namespace device
{
	typedef std::chrono::time_point<std::chrono::system_clock> timePoint;

	class LEDWriteVal : public IWriteVal
	{
	public:
		LEDWriteVal() = delete;
		LEDWriteVal(bool v) : val(v) { }
		virtual ~LEDWriteVal() { }

		operator bool() { return val; }

		bool val;
	};

	class LED : public DeviceBase, public IWriteable
	{
	public:
		LED(int, int, bool = true, unsigned int = 0);
		virtual ~LED() { }

		virtual std::string ToString() const;

		virtual void SaveToTree(boost::property_tree::ptree&, const std::string&) const;
		virtual bool LoadFromTree(boost::property_tree::ptree::value_type&);

		virtual bool Write(IWriteVal* val);

		virtual std::string Execute(std::string&);

		void On();
		void Off();

		void LockOn();
		void LockOff();
		void Unlock();

		bool IsOn() const { return _state == static_cast<int>(_logic); }
		bool IsLocked() const { return _isLocked; }
		std::chrono::seconds GetDelay() const { return _delay; }
		void SetDelay(std::chrono::seconds& delay) { _delay = delay; }

		static const std::string name;

	protected:
		virtual void _Write(bool val) = 0;

		int _pin;
		bool _logic;
		std::chrono::seconds _delay;
		int _defaultValue;
		bool _isLocked;
		int _state;

		timePoint _lightingTime;
	};
}

#endif // !LED_H
