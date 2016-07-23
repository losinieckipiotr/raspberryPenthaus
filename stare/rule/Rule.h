#ifndef RULE_H
#define RULE_H
#include <list>
#include <string>
#include "../prototype/PrototypeBase.h"

#include "ITrigger.h"
#include "IAction.h"

namespace rule
{
	class Rule : public prototype::PrototypeBase
	{
	public:
		Rule(std::string, int, bool = true);
		~Rule();

		virtual std::string ToString() const;

		virtual void Save(std::ostream&) const;
		virtual bool Load(std::string&);

		virtual prototype::IPrototype* Clone() const;

		std::string& Return_name() { return _name; }
		int Return_priority() { return _priority; }

		void AddEvent(IEvent*);
		void CheckAndExecute();
		void SaveEvents(std::ofstream&);
		void ClearEvents();
		std::string PrintEvents();

		static const Rule prototype;
		static const std::string name;

	private:
		bool _mode;
		int _priority;
		std::string _name;

		std::list<ITrigger*> _triggers;
		std::list<IAction*> _actions;
	};
}

#endif // !RULE_G
