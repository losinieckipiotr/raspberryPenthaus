#ifndef IO_H
#define IO_H
#include "StdIO.h"
#include "network/Server.h"

namespace program
{
	class Program;
}

namespace io
{
	class IO : public StdIO
	{
	public:
		static IO* Instance();

		void StopIO();
		void StartIO();

		virtual std::list<std::string> Input(std::string& message, network::Session& session);

	private:
		IO();
		virtual ~IO();

		static IO* _instance;

		void _ClearScreen();
		void _StartServer();
		void _StopServer();

		network::Server* _server;

		std::future<void> _serverFuture;
	};
}

#endif // !IO_H
