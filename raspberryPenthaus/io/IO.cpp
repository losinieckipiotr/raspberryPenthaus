#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <exception>
#include <chrono>
#include <future>

#include "IO.h"
#include "../program/Program.h"
#include "../wp.h"

using namespace program;
using namespace io;
using namespace std;
using namespace chrono;

IO* IO::_instance = nullptr;

IO* IO::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new IO;
	}
	return _instance;
}

IO::IO() : _server(nullptr)
{
	fstream file("files/header.txt", ifstream::in);
	if (!file)
		throw runtime_error("Cannot open file: files/header.txt");
	file.seekg(0, file.end);
	int length = (int)file.tellg();
	file.seekg(0, file.beg);
	char * buffer = new char[length];
	file.read(buffer, length);
	file.close();
	stringstream ss(buffer);
	delete[]buffer;
	string buffer2;
	list<string> lines;
	while (true)
	{
		getline(ss, buffer2);
		if (ss.good())
			lines.push_back(buffer2);
		else
			break;
		buffer2.clear();
	}

	_helloMsg.clear();
	_helloMsg.insert(_helloMsg.begin(), lines.begin(), lines.end());
	_helloMsg.push_back(string(""));
}

void IO::StopIO()
{
	if (_instance != nullptr)
	{
		_StopServer();
		delete this;
		_instance = nullptr;
	}
}

IO::~IO()
{

}

void IO::StartIO()
{
	_StartServer();
	StandardOutput("PENTHAUS PROJECT - Piotr Losiniecki (c)");
	StandardOutput("Server is running...");
}

list<string> IO::Input(string& line, network::Session& session)
{
	Program& prog = *Program::Instance();
	list<string> reply;
	stringstream ss(line);
	string buffer;
	ss >> buffer;

	if (buffer == "add")
	{
		reply.push_back(prog.Add(line));
	}
	else if (buffer == "create")
	{
		reply.push_back(prog.Create(line));
	}
	else if (buffer == "execute")
	{
		reply.push_back(prog.Execute(line));
	}
	else if (buffer == "help")
	{
		reply = _helloMsg;
	}
	else if (buffer == "shutdown")
	{
		_ClearScreen();
		StandardOutput("Program is shutting down...");
		reply.push_back("shutdown");
		prog.StopAll();
	}
	else if (buffer == "clear")
	{
		ss >> buffer;
		if (buffer == "all")
		{
			reply.push_back(prog.ClearAll());
		}
		else if (buffer == "events")
		{
			reply.push_back(prog.ClearEvents());
		}
		else if (buffer == "rules")
		{
			reply.push_back(prog.ClearRules());
		}
		else
		{
			reply.push_back("Syntax error");
		}
	}
	else if (buffer == "print")
	{
		ss >> buffer;
		if (buffer == "devs")
		{
			buffer = prog.GetDevsString();
			stringstream ss2(buffer);
			buffer.clear();
			list<string> lines;
			while (true)
			{
				getline(ss2, buffer);
				if (ss2.good())
					lines.push_back(buffer);
				else
					break;
				buffer.clear();
			}
			reply = lines;
		}
		else if (buffer == "events")
		{
			buffer = prog.GetEventsString();
			stringstream ss2(buffer);
			buffer.clear();
			list<string> lines;
			while (true)
			{
				getline(ss2, buffer);
				if (ss2.good())
					lines.push_back(buffer);
				else
					break;
				buffer.clear();
			}
			reply = lines;
		}
		else if (buffer == "rules")
		{
			buffer = prog.GetRulesString();
			stringstream ss2(buffer);
			buffer.clear();
			list<string> lines;
			while (true)
			{
				getline(ss2, buffer);
				if (ss2.good())
					lines.push_back(buffer);
				else
					break;
				buffer.clear();
			}
			reply = lines;
		}
		else
		{
			reply.push_back("Syntax error");
		}
	}
	else
	{
		reply.push_back("Syntax error");
	}

	return reply;
}

void IO::_ClearScreen()
{
	system(CLEAR_SCREEN);
}

void IO::_StartServer()
{
	if (_server == nullptr)
	{
		_server = new network::Server(1212, *this);
		_serverFuture = move(_server->Start());
	}
}

void IO::_StopServer()
{
	if (_server != nullptr)
	{
		_server->Stop();
		_serverFuture.wait();
		delete _server;
		_server = nullptr;
	}
}
