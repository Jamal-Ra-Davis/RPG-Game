#pragma once
#include <stdio.h>
#include <stdlib.h>

class player;
class enemy;
class item;
class party;

class battle
{
	enemy **enm_lst;
	int num_enms;
	player **ply_lst;
	int num_plys;
	item **inv;
	int num_itms;

	party *characters;
//	int enm_lvl;
	int turn_cnt;

    int flee_chance;
	bool flee_success;

	public:
//		battle(enemy*, int, player**, int, item**, int);
		battle(enemy**, int, party*);
//		void set_enmLvl();
		int battleLoop();
			int checkVictory();
			int Attack(int, int);
			int playerPhase();
                int player_Attack(int);
                int player_Defend(int);
                int player_Skill(int);
                int player_Item(int);
                int player_Flee(int);
			int enemyPhase();
                int enemy_Attack(int);
                int enemy_Defend(int);
                int enemy_Skill(int);
			void resetPlayers();
			void resetEnemies();
		void battleAwards();
			void saveStats();
		void checkQuest();
};
