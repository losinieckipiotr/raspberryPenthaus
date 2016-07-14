#include "program\Program.h"

using namespace std;
using namespace program;

int main()
{
	try
	{
		Program* prog = Program::Instance();
		if (prog->Init())
		{
			prog->IO();
			prog->CoreLoop();
		}
		prog->ExitProgram();
		return 0;
	}
	catch (exception&)
	{
		return 1;
	}
}
