#ifndef PROGRAM_H
#define PROGRAM_H
#include <chrono>
#include <mutex>
#include <string>
#include <memory>
#include <list>

#include "../ItemsPool.hpp"
#include "../event/IEvent.h"
#include "DeviceReader.h"
#include "../device/DeviceManager.h"
#include "Commander.h"
#include "Creator.h"
#include "../gpio/GPIO.h"
#include "../io/IO.h"
#include "IDriver.h"

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
		Program(const Program&) = delete;
		Program(const Program&&) = delete;

		static ItemsPool<event::eventPtr>& GetEventPool();

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

		void _Setup();
		void _SaveAll();
		void _CheckDelay(std::chrono::time_point
			< std::chrono::system_clock,
			std::chrono::system_clock::duration >);

		static Program* _instance;
		static ItemsPool<event::eventPtr> _eventPool;

		std::string _serializeFile;

		device::DeviceManager _deviceManager;
		DeviceReader _deviceReader;
		Commander _commander;
		Creator _creator;
		gpio::GPIO* _bus;
		io::IO* _io;

		std::list<IDriver*> _drivers;

		bool _coreQuit;
		std::mutex _program_mutex;
	};
}

#endif // !PROGRAM_H
