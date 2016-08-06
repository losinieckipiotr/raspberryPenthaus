#include <thread>
#include <iostream>

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
		cout << "Running..." << endl;
		cin.get();
		prog->StopAll();
		cout << "Stopping program...";
		th.join();
		cin.get();
		//usuwanie instancji glownej klasy
		prog->ExitProgram();

		return 0;
	}
	catch (exception& ex)
	{
        cerr << ex.what() << endl;
		return 1;
	}
}
