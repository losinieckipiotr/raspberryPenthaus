#include <string>
#include <sstream>
#include <stdexcept>

#include "Session.h"
#include "../StdIO.h"

using namespace io;
using namespace network;
using namespace transport;
using namespace boost;
using namespace asio;
using namespace ip;
using namespace std;

Session::Session(Server& server,
	tcp::socket socket,
	StdIO& io_module) :
	_server(server),
	_socket(move(socket)),
	_io_module(io_module),
	_peer(_socket.remote_endpoint())
{
	tcp::no_delay option(true);
	//wylacz algorytm Nagle (unika opoznienia przy wysylaniu pojedynczych ramek)
	_socket.set_option(option);
	string str = "New connection form " +
		_peer.address().to_string() + ':' + to_string(_peer.port());
	//wyswietl informacje o nowym polaczeniu
	_io_module.StandardOutput(str);
}

Session::~Session()
{

}

void Session::Start()
{
	//odbierz ramke
	_RecieveFrame([this]()
	{
		//zdekoduj ramke
		string message = _frame.GetMsg();
		//czy to ramka powitania?
		if (message == "<HELLO>")
		{
			//pobranie wiadomosci na powitanie
			_buffer = _io_module.GetHello();
			//wyslanie powitania
			_Send();
		}
		else
		{
			//client nie przywital sie, rozlacz
			_WasError("hello protocol error - ");
		}
	});
}

void Session::_Send()
{
	//jezeli dane w buforze to przesylaj po 1 linii
	if (!_buffer.empty())
	{
		//pobranie linii
		string line = _buffer.front();
		//usuniecie z bufora
		_buffer.pop_front();
		//usuniecie pustych linii i zastapienie ich znacznikami
		if (line == "")
			line = "<NEWLINE>";
		//kodowanie i wyslanie ramki
		_SendLineFrame(line);
	}
	else
	{
		//wyslij ramke stopu
		_SendStopFrame();
	}
}

void Session::_SendLineFrame(const string& line)
{
	//kodowanie ramki
	if (!_frame.LoadMessage(line))
	{
		//jezeli wystapil blad, wyswietlenie komunikatu i zamkniecie sesji
		_WasError("encode frame error - ");
		return;
	}
	//wyslanie ramki
	async_write(_socket, buffer(_frame.BeginData(),
		MyFrame::frameSize),
		//handler uruchamiany po wyslaniu ramki
		[this](error_code ec, size_t size)
	{
		//czy wystapil blad?
		if (!ec)
		{
			//nie, wyslij nastepna ramke UWAGA: To nie jest rekurencja !
			_Send();
		}
		else
		{
			//tak, wyswietlnie bledu i zamkniecie sesji
			_AsyncError("send frame error - ", ec);
		}
	});
}

void Session::_SendStopFrame()
{
	//bufor pusty, wyslij ramke konca transmisji
	string line = "<STOP>";
	//kodowanie ramki
	if (!_frame.LoadMessage(line))
	{
		//jezeli wystapil blad, wyswietlenie komunikatu i zamkniecie sesji
		_WasError("encode frame error - ");
		return;
	}
	//wyslanie ramki
	async_write(_socket, buffer(_frame.BeginData(),
		MyFrame::frameSize),
		//handler uruchamiany po wyslaniu ramki
		[this](error_code ec, size_t size)
	{
		//czy wystapil blad?
		if (!ec)
		{
			//nie, odbierz ramke
			_RecieveFrame([this]()
			{
				//przetwarzaj ramke
				_HandleFrame();
			});
		}
		else
		{
			//tak, wyswietlnie bledu i zamkniecie sesji
			_AsyncError("send frame error - ", ec);
		}
	});
}

template<class handler>
void Session::_RecieveFrame(handler frameHandler)
{
	//odebranie ramki
	_socket.async_read_some(buffer(_frame.BeginData(),
		MyFrame::frameSize),
		//handler uruchamiany po odebraniu ramki
		[this, frameHandler](error_code ec, size_t size)
	{
		//czy wystapil blad?
		if (!ec)
		{
			//nie, przetwarzaj odebrana ramke
			frameHandler();
		}
		else if (ec == error::eof)
		{
			//klient zakonczyl polaczenie, zakonczenie sesji
			_WasError("client disconnected - ");
		}
		else
		{
			//tak, wyswietlnie bledu i zamkniecie sesji
			_AsyncError("receive frame error - ", ec);
		}
	});
}

void Session::_HandleFrame()
{
	try
	{
		//dekodowanie, ramki
		string message = _frame.GetMsg();
		//jezeli pusta wiadomosc wystapil blad dekodowania
		if (message == "")
			throw std::logic_error("decode frame error");
		//wykonanie polecenia i pobranie odpowiedzi
		_buffer = _io_module.Input(message, *this);
		//jezeli pusty bufor wystapil blad w wykonaniu polecenia
		if (_buffer.empty())
			throw std::runtime_error("input error");
		//jezeli serwer zostaje wylaczony zakoncz
		else if (_buffer.front() == "shutdown")
		{
			_socket.close();
			//usuniecie sesji
			_server.DeleteSession(this);
			return;
		}
		//start przesylania odpowiedzi
		_Send();
	}
	catch (std::exception& ex)
	{
		//jezeli wystapil blad, wyswietlenie komunikatu i zamkniecie sesji
		_WasError(ex.what() + string(" - "));
	}
}

void Session::_AsyncError(const string& message, const error_code& ec)
{
	//komunikat o bledzie operacji asynchronicznej
	string s = message
		+ to_string(ec.value()) + ". "
		+ ec.message() + ". ";
	//wyswietlenie i zamkniecie sesji
	_WasError(s);
}

void Session::_WasError(const string& message)
{
	try
	{
		//wyswietlenie komunikatu o zamkniecu sesji z danym peerem
		string s = message;
		s += "connection with "
			+ _peer.address().to_string() + ':' + to_string(_peer.port())
			+ " will be terminated";
		_io_module.StandardOutput(s);
		//zamkniecie socketa powoduje anulowanie wszystkich operacji
		//asynchronicznych
		_socket.close();
		//usuniecie sesji
		_server.DeleteSession(this);
	}
	catch (std::exception& ex)
	{
		//wyswietlenie bledu,
		//jezeli tutaj pojawil sie blad jest to cos powaznego
		_io_module.ErrorOutput(ex.what());
	}
}
