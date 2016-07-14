#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <set>
#include <future>
#include <boost/asio.hpp>

namespace io
{
	class StdIO;

	namespace network
	{
		class Session;

		class Server
		{
			typedef const boost::system::error_code& error_code;

		public:
			Server(unsigned short port, StdIO& io);
			~Server();

			//wlaczenie serwera asynchronicznie,
			//zwraca obiekt future reprezentujacy watek dzialania serwera
			std::future<void> Start();
			//wylaczenie operacji asynchronicznych serwera,
			//powinno skutkowac zakonczeniem sie watka serwera
			void Stop();
			//zwraca czy serwer zostal wlaczony
			inline bool IsStarted() { return _isStarted; }
			//usuwa sesje z listy aktywnych sesji
			void DeleteSession(Session* session_ptr);

		private:
			//asynchroniczna operacja akceptujaca przychodzace polaczenia
			void _Accept();
			//metoda tworzy nowa sesje jezeli przyjdzie polaczenie
			void _StartNewSession();
			//handler akceptacji polaczenia
			void _HandleAccept(error_code error);

			boost::asio::io_service _io_service;
			const unsigned short _port;
			StdIO& _io_module;
			
			boost::asio::ip::tcp::acceptor _acceptor;
			boost::asio::ip::tcp::socket _socket;

			bool _isStarted = false;

			std::set<Session*> _active_sessions;
		};
	}
}

#endif // !SERVER_H
