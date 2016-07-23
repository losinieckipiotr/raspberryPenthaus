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
		LEDWriteVal(bool v) : val(val) { }
		LEDWriteVal(int v) : val(val) { }
		virtual ~LEDWriteVal() { }

		bool val;
	};

	class LED : public DeviceBase, public IWriteable
	{
	public:
		LED(int, int, int, bool = true);
		virtual ~LED() { }

		virtual std::string ToString() const;

		virtual void Save(std::ostream&) const;
		virtual bool Load(std::string&);

		//virtual bool IsReadable() { return false; }

		virtual bool Write(IWriteVal* val);

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
		virtual void _Write(bool);

		bool _IsOn() const { return _state == static_cast<int>(_logic); }

		
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
