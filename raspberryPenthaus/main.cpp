#include <thread>

#include "program/Program.h"

using namespace std;
using namespace program;

int main()
{
	try
	{
		//tworzenie instancji glownej klasy
		Program* prog = Program::Instance();

		thread th([&prog]()
		{
			//inicjalizacja (czytanie plikow konfiguracyjnych,
			//inicjalizacja sprzetu)
			if (prog->Init())
			{
				//wlaczanie modulu serwera
				prog->IO();
				//glowna petla programu
				prog->CoreLoop();
			}
		});
		cout << "Running...";
		cin.get();
		prog->StopAll();
		cout << "Stopping program...";
		cin.get();

		//usuwanie instancji glownej klasy
		prog->ExitProgram();
		th.join();

		return 0;
	}
	catch (exception&)
	{
		return 1;
	}
}
