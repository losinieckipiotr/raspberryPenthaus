#ifndef LED_H
#define LED_H
#include <string>
#include <chrono>
#include "DeviceBase.h"

namespace gpio
{
	typedef std::chrono::time_point<std::chrono::system_clock> timePoint;

	class LED : public DeviceBase
	{
	public:
		LED(int, int, int, bool = true);
		virtual ~LED() { }

		virtual std::string ToString() const;

		virtual void Save(std::ostream&) const;
		virtual bool Load(std::string&);

		virtual void Check() { }

		virtual std::string Execute(std::string&);

		void On();
		void Off();
		void HardOff();

		void LockOn();
		void LockOff();
		void Unlock();
		void ChangeDelay(int);

		static const std::string name;

	protected:
		virtual void _Write(int) = 0;

		bool _IsOn() const
		{ return _state == static_cast<int>(_logic); }

		
		int _pin;
		std::chrono::duration<int> _delay;
		bool _logic;
		int _defaultValue;
		bool _isLocked;
		int _state;
		timePoint _lightingTime;
	};
}

#endif // !LED_H
