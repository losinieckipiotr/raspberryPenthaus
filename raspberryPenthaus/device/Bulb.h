#ifndef BULB_H
#define BULB_H

#include "DeviceBase.h"
#include "IWriteable.h"

namespace device
{
	typedef std::chrono::time_point<std::chrono::system_clock> timePoint;

	class BulbWriteVal : public IWriteVal
	{
	public:
		BulbWriteVal() = delete;
		BulbWriteVal(bool v) : val(v) { }
		virtual ~BulbWriteVal() { }

		operator bool() { return val; }

		bool val;
	};

	class Bulb : public DeviceBase, public IWriteable
	{
	public:
		Bulb(int, unsigned int);
		virtual ~Bulb();

		virtual std::string ToString() const;

		virtual void SaveToTree(boost::property_tree::ptree&, const std::string&) const;
		virtual bool LoadFromTree(boost::property_tree::ptree::value_type&);

		virtual bool Write(IWriteVal*);

		virtual std::string Execute(std::string&);

		void On();
		void Off();

		void LockOn();
		void LockOff();
		void Unlock();
		void ChangeDelay(int);

		bool IsOn() const { return (_state >= 1); }
		bool IsLocked() const { return _isLocked; }
		std::chrono::seconds GetDelay() const { return _delay; }
		void SetDelay(std::chrono::seconds& delay) { _delay = delay; }

		static const std::string name;

	protected:
		virtual void _Write(bool) = 0;

		std::chrono::seconds _delay;
		int _state;
		bool _isLocked;

		timePoint _lightingTime;
	};
}

#endif // !BULB_H

