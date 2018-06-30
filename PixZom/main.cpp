// PixZom.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <SDL.h>
#include "PixZomApp.h"


int main(int argc, char* argv[])
{
	CPixZomApp app;

	const std::string windowName = "PixMon";
	int width = 800;
	int height = 600;

	if (app.Initialise(windowName, width, height)) 
	{
		app.Run();
	}

	//system("PAUSE");
    return 0;
}
