#pragma once
#include <stdio.h>
#include <stdlib.h>


class enemy;
class player;
class item;
class party;

class areas 
{
	char name[32];
	int id;//Currently is not being used
	int start_col, start_row, boss_col, boss_row;
	int loc_col, loc_row;
//	enemy monster_list[3];
	int *area_monsters;
	int num_monsters;
	int monster_lvl;
	int lvl_range;	

	bool **movement;//[5][5];
	int SIZE;///////////
	bool **treasure;//[5][5];
	int enc_rate;
	int treas_rate;
	bool boss_defeated;
	
	int num_enm;
	enemy *enms_fought;
	int enm_level;

	enemy *boss;
	int boss_level;

/*
	player **ply_lst;
	int num_plys;
	item **inventory;
	int num_itms;
*/
	party *characters;	

	public:
		areas(){};
		areas (char*, int, party*);
		~areas ();
		void setArea(char*, int, party*);
		void map_disp();
		void treasure_disp();
		void monster_disp();
		void info_disp();

		int getDifficulty();

		void enter_area();
		int change_loc(); 
		void examine_room();//Need to set it up so it actually gives you real items, not just a message
		bool enemy_encounter();
		void dungeon_loop();
		void random_dungeon();
		void random_dungeon2();
			int step_through(int,int);
		void file_dungeon();
		void battle_init();
		void battle_conclude();

		char* getName()
			{return name;}
};
