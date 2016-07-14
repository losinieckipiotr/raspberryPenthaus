#ifndef SESSION_H
#define SESSION_H
#include <string>
#include <list>
#include <boost/asio.hpp>

#include "Server.h"
#include "transport/MyFrame.hpp"

namespace io
{
	namespace network
	{
		class Session
		{
			typedef const boost::system::error_code& error_code;

		public:
			Session(Server& server,
				boost::asio::ip::tcp::socket socket,
				StdIO& io_module);
			~Session();

			//zwroc socket
			inline boost::asio::ip::tcp::socket& GetSocket() { return _socket; }
			//zwroc server (mozna wykorzystac do zamkniecia serwera)
			inline Server& GetServer() { return _server; }

			//uruchamia sesje
			void Start();

		private:
			//sprawdza czy w buforze pozostaly wiadomosci do przeslania i inicjalizuje wysylanie
			void _Send();
			//wysyla 1 ramke
			void _SendLineFrame(const std::string& line);
			//wysyla ramke stopu
			void _SendStopFrame();
			//odbiera ramke, sprawdza czy wystapil blad, wywoluje handler podany jako argument
			template<class handler>
			void _RecieveFrame(handler frameHandler);
			//dekoduje odebrana ramke
			void _HandleFrame();

			//tworzy komunikat o bledzie w operacji asynchronicznej
			void _AsyncError(const std::string& message, const error_code& ec);
			//wyswietla komunikaty o bledzie i zamyka sesje
			void _WasError(const std::string& message);

			Server& _server;
			boost::asio::ip::tcp::socket _socket;
			StdIO& _io_module;
			
			boost::asio::ip::tcp::endpoint _peer;
			transport::MyFrame _frame;
			std::list<std::string> _buffer;
		};
	}
}

#endif // !SESSION_H
