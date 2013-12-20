#pragma once
#include <stdio.h>
#include <stdlib.h>

class home;
class player;
class Inventory;
class party;

class home
{
	player** ply_lst;
	int num_plys;
	int Max_members;

	Inventory* storage;	
	party* player_party;

	public:
		home();
		home(int, int);
//		home(FILE*);
		void setHome();
		void setHome(int, int);
		~home();

		int getPartySize()
			{return num_plys;}
		int getMaxPartySize()
			{return Max_members;}

		player* recievePlayer(player*);
		void compressParty();
		player* removePlayer(int);
		void deletePlayer(int);

		bool inventoryFull();
		bool partyFull();
		void printPlayers();

		void enterHome(party*);
			void depositItem();
			void withdrawItem();
			void depositPlayer();
			void withdrawPlayer();

		void saveHome();
		void loadHome();
};
