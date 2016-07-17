#include "program\Program.h"

using namespace std;
using namespace program;

int main()
{
	try
	{
		//tworzenie instancji glownej klasy
		Program* prog = Program::Instance();
		//inicjalizacja (czytanie plikow konfiguracyjnych,
		//inicjalizacja sprzetu)
		if (prog->Init())
		{
			//wlaczanie modulu serwera
			prog->IO();
			//glowna petla programu
			prog->CoreLoop();
		}
		//usuwanie instancji glownej klasy
		prog->ExitProgram();
		return 0;
	}
	catch (exception&)
	{
		return 1;
	}
}
