#include "program\Program.h"

using namespace std;
using namespace program;

int main()
{
	try
	{
		//creating instance of main program classs
		Program* prog = Program::Instance();
		//initalization (reading config files, setting up hardware)
		if (prog->Init())
		{
			//starting server module
			prog->IO();
			//main program loop
			prog->CoreLoop();
		}
		//deleting instance of program class
		prog->ExitProgram();
		return 0;
	}
	catch (exception&)
	{
		return 1;
	}
}
