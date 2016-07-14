#ifndef RULE_MANAGER_H
#define RULE_MANAGER_H
#include <list>
#include <string>
#include <functional>
#include "Rule.h"


namespace rule
{
	class RuleManager
	{
	public:
		RuleManager();
		~RuleManager();

		void AddRule(Rule*);
		Rule* ReturnRule(std::string&);
		void ExecuteRules();
		void SaveRules(std::string&);
		void SaveEvents(std::string&);
		void ClearRules();
		void ClearEvents();
		std::string PrintRules();
		std::string PrintEvents();

	private:
		static bool _comp(Rule* r1, Rule* r2)
		{
			int a = r1->Return_priority();
			int b = r2->Return_priority();
			return a < b;
		}
		Rule* _FindByName(std::string&);

		std::list<Rule*> _rules;
	};
}



#endif // !RULE_MANAGER_H
