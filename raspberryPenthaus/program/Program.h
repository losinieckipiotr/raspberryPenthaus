#ifndef PROGRAM_H
#define PROGRAM_H
#include <chrono>
#include <mutex>
#include <string>

#include "Creator.h"
#include "../rule/RuleManager.h"
#include "../gpio/GPIO.h"
#include "Commander.h"
#include "../io/IO.h"
#include "../wp.h"

namespace program
{
	/// <summary>
	/// Glowna klasa aplikacji, implementuje wzorzec Singleton.
	/// Metoda IO inicjuje serwer(asynchronicznie). W metodzie CoreLoop jest
	/// glowna petla programu sterujacego(metoda blokujaca az do wyjscia  z programu).
	/// </summary>
	class Program
	{
	public:
		static Program* Instance();
		Program(const Program&) { }
		Program(const Program&&) { }

		void ExitProgram();

		bool Init();
		void CoreLoop();
		void IO();
		void StopAll();

		std::string Add(std::string&);
		std::string Create(std::string&);
		std::string Execute(std::string&);
		std::string ClearAll();
		std::string ClearEvents();
		std::string ClearRules();

		std::string GetDevsString();
		std::string GetRulesString();
		std::string GetEventsString();

	private:
		Program();
		~Program();

		static Program* _instance;

		std::string _devsFile;
		std::string _rulesFile;
		std::string _eventsFile;

		rule::RuleManager _ruleManager;
		Creator* _creator;
		gpio::GPIO* _bus;

		Commander _commander;
		io::IO* _io;

		void _Setup();
		void _CheckAndExecute();
		void _SaveAll();
		void _CheckDelay(std::chrono::time_point
			< std::chrono::system_clock,
			std::chrono::system_clock::duration > );

		bool _coreQuit;
		std::mutex _program_mutex;
	};
}

#endif // !PROGRAM_H