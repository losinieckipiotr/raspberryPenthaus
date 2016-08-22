#include <iostream>
#include <future>

#include "StdIO.h"
#include "network/Server.h"
#include "network/Session.h"

using namespace io;
using namespace std;

mutex StdIO::_std_out_mutex;
mutex StdIO::_std_error_mutex;

StdIO::StdIO()
{
	//powitanie uzytkownika
	_helloMsg.push_back("<HELLO>");
}

StdIO::~StdIO()
{

}

list<string> StdIO::Input(string& message, network::Session& session)
{
	//brak implementacji, przeslonic metode (NO IMPLEMENTATION EXCEPTION)
	list<std::string> reply;
	reply.push_back("<ERROR>");
	return reply;
}

void StdIO::StandardOutput(const string& line)
{
	//nie blokujace wyslanie na standardowe wyjscie 
	std::async(launch::async, [&line]()
	{
		lock_guard<mutex> lck(_std_out_mutex);
		cout << line << endl;
	});
}

void StdIO::ErrorOutput(const string& line)
{
	//nie blokujace wyslanie na strumien bledow
	std::async(launch::async, [&line]()
	{
		lock_guard<mutex> lck(_std_out_mutex);
		cerr << line << endl;
	});
}