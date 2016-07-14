#include <stdexcept>

#include "../StdIO.h"
#include "Server.h"
#include "Session.h"

using namespace io;
using namespace network;
using namespace transport;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;

Server::Server(unsigned short port, StdIO& io)
	: _port(port),
	  _io_module(io),
	  _acceptor(_io_service, ip::tcp::endpoint(tcp::v4(), port), true),
	  _socket(_io_service)
{
	//jezeli gniazdo nie jest otwarte to rzuc blad
	if (!_acceptor.is_open())
		throw runtime_error("Server is not started correctly!");
	//rozpocznij akceptowanie polaczen
	_Accept();
}

Server::~Server()
{
	//usun wszystkie sesje
	for (auto& session : _active_sessions)
	{
		delete session;
	}
}

std::future<void> Server::Start()
{
	//jezeli serwer zostal juz wlaczony rzuc blad
	if (_isStarted != false)
		throw runtime_error("Attempt to start server more than once!");
	//uruchomienie operacji asynchronicznych serwera na innym watku
	future<void> serverFuture = async(launch::async, [this]()
	{
		_isStarted = true;
		_io_service.run();

	});
	return serverFuture;
}

void Server::Stop()
{
	//jezeli serwer jest uruchominy
	if (_isStarted)
	{
		//oznacz jako wylaczony
		_isStarted = false;
		//wylacz operacje asynchroniczne
		_io_service.stop();
	}
}

void Server::_Accept()
{
	//asynchroniczne akceptuj polaczenia przychodzace
	_acceptor.async_accept(_socket,
		[this](error_code er)
	{
		_HandleAccept(er);
	});
}

void Server::_HandleAccept(error_code error)
{
	//jezeli blad
	if (error)
	{
		string s("accept_handler error - ");
		s += error.message();
		//wyswietl informacje o bledzie
		_io_module.StandardOutput(s);
		//stworz nowy akceptor na tym porcie
		_acceptor =
			tcp::acceptor(_io_service, ip::tcp::endpoint(tcp::v4(), _port), true);
	}
	else
	{
		// jezeli nie ma bledu, stworz nowa sejse
		_StartNewSession();
	}
	//akceptuj nastepne polaczenia
	_Accept();
}

void Server::_StartNewSession()
{
	//tworzenie nowej sesji
	Session* s = new Session(
		*this,
		move(_socket),
		_io_module);
	_socket = ip::tcp::socket(_io_service);
	//dodaj do listy aktywnych sesji
	_active_sessions.insert(s);
	//wystartuj sesje
	s->Start();
}

void Server::DeleteSession(Session* session_ptr)
{
	//usun z listy sesji
	_active_sessions.erase(session_ptr);
	//zwolnij zasoby sesji
	delete session_ptr;
}
