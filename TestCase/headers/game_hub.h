#pragma once
#include <stdio.h>
#include <stdlib.h>


class party;
class areas;
class shop;
class inn;
class home;
class questHall;

class game_hub
{
	party *Team;
	
	areas **Dungeons;
	areas *rand_dungeon;
	int num_areas;
	int active_areas;	

	shop **Shops;
	int num_shops;
	int active_shops;

	inn *Inn;	
	home *Home;
	questHall *Hall;

	public:
		game_hub();
		~game_hub();
		void startGame();
		void newGame();
		void loadGame();

		void saveGame();
		void gameLoop();//May change return type to int for more info		
		void hubMenu();
		void areaMenu();
		void shopMenu();

		void printAreas();
		void printShops();
		void incrementAreas();
		void incrementShops();
};
