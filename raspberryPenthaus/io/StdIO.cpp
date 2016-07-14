#include <iostream>
#include <future>

#include "StdIO.h"
#include "network/Server.h"
#include "network/Session.h"

using namespace io;
using namespace std;

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
	//brak implementacji, przeslonic metode
	list<std::string> reply;
	reply.push_back("<ERROR>");
	return reply;
}

void StdIO::StandardOutput(const string line)
{
	//nie blokujace wyslanie na standardowe wyjscie 
	std::async(launch::async, [this, line]()
	{
		lock_guard<mutex> lck(_std_out_mutex);
		cout << line << endl;
	});
}

void StdIO::ErrorOutput(const string line)
{
	//nie blokujace wyslanie na strumien bledow
	std::async(launch::async, [this, line]()
	{
		lock_guard<mutex> lck(_std_out_mutex);
		cerr << line << endl;
	});
}