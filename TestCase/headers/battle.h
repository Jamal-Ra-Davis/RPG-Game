#pragma once
#include <stdio.h>
#include <stdlib.h>

class player;
class enemy;
class item;
class party;

class battle
{
	enemy *enm_lst;
	int num_enms;
	player **ply_lst;
	int num_plys;
	item **inv;
	int num_itms;

	party *characters;
//	int enm_lvl;
	int turn_cnt;
	bool flee_success;

	public:
//		battle(enemy*, int, player**, int, item**, int);
		battle(enemy*, int, party*);
//		void set_enmLvl();
		int battleLoop();
			int checkVictory();
			int Attack(int, int);
			int playerPhase();
			int enemyPhase();
			void resetPlayers();
			void resetEnemies();
		void battleAwards();
			void saveStats();
		void checkQuest();
};
