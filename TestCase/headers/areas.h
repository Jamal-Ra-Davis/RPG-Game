#pragma once
#include <stdio.h>
#include <stdlib.h>


class enemy;
class player;
class item;
class party;
class areas;

areas* getArea(int id, party *player_in);

class areas 
{
    //Basic Area Info
	char name[32];
	int id;//Currently is not being used

    //Location variables
	int start_col, start_row, boss_col, boss_row;
	int loc_col, loc_row;

    //Dungeon Monster Info (What monster are in the area and what level they can be) 
	int *area_monsters;
	int num_monsters;
	int monster_lvl;
	int lvl_range;	

    //Area Info (Size of area, the movement and treasure maps, enemy encouter rate, likelyhood of treasure spawns)
    int SIZE;
	bool **movement;
	bool **treasure;
	int enc_rate;
	int treas_rate;
	bool boss_defeated;
	
    //Battle Info (The enemies you fight in the battle, how many are fought, and their levels)
	int num_enm;
//	enemy *enms_fought;
    enemy **random_encounter;
	int enm_level;//Not being used apparently

    //Boss information
//	enemy **boss;
	int boss_level;
    int boss_encounter_size;
    enemy **boss_encounter;
    int *boss_IDs; 

    //Variable to keep track of player party
	party *characters;	



	public:
		areas(){};
		areas(char*, int, party*);
        areas(char*,party*);
		~areas();
        void nullPointers();
		void setArea(char*, int, party*);
        void loadArea(char*, party*);
		void map_disp();
		void treasure_disp();
		void monster_disp();
		void info_disp();

		int getDifficulty();

		void enter_area();
        void leave_area();
		int change_loc(); 
		void examine_room();//Need to set it up so it actually gives you real items, not just a message
		bool enemy_encounter();
        void resetBoss();
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
