#include <cstdlib>
#include <ctime>

#include "Menu.h"

int main()
{
	srand(time(NULL));
	Menu menu;
	menu.settings();
	menu.start();

	//multipleGamesTest(1000);

	return 0;
}