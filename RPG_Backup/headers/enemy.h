#pragma once
#include <stdio.h>
#include <stdlib.h>

class enemy;
class skill;
class player;

int getEnemyRange();
enemy* getEnemy(int);

class enemy
{
		int id;
		char name[32];
		skill **skill_lst;
		int num_skills;
		//Base Stats
		int Max_Health_b;
		int Max_mp_b;
		int att_b, def_b, mag_b, mdef_b;
		//Regular Stats
		int Max_Health, Health, mp, Max_mp;
		int att, def, mag, mdef;

		int Level;
		bool dead;
		int exp;
		int gold_value;

		int att_btl, def_btl, mag_btl, mdef_btl;
		public:
			enemy(){};
			~enemy();
			enemy(int, char*, int, int, int, int, int, int, int);
			void setEnemy(int, char*, int, int, int, int, int, int, int);
			void setLevel(int LEVEL);
			void resetSkillLst();

			int getID()
				{return id;}
			char* getName()
				{return name;}
			int getNum_skills()
				{return num_skills;}
			int getHealth()
				{return Health;}
			int getMax_Health()
				{return Max_Health;}
			int getMP()
				{return mp;}
			int getMax_MP()
				{return Max_mp;}
			int getAtt()
				{return att;}
			int getAtt_btl()
				{return att_btl;}
			int getDef()
				{return def;}
			int getDef_btl()
				{return def_btl;}
			int getMag()
				{return mag;}
			int getMag_btl()
				{return mag_btl;}
			int getMdef()
				{return mdef;}
			int getMdef_btl()
				{return mdef_btl;}
			int getLevel()
				{return Level;}
			int getExp()
				{return exp;}
			int getGoldVal()
				{return gold_value;}

			bool isDead();//Check if health is < 0, sets and returns death status


			int takeDamage(int);//Returns 1 if you try to damage when dead, 0 if things work right
			int restoreHealth(int);//Returns 1 if you try to heal when dead, 0 if things work right
			int restoreMP(int);//Returns 1 if you try to restore mp when dead, 0 if successful
			int removeMP(int);//Returns 1 if you try to remove mp when dead, 0 if successful
			int revive(int);//Returns 1 if you try to revive player that's alive

			void Defend();
			void statusReset();
			void initSkill();
			void recieveSkill(int);

			void useSkillBtl(int skl_idx, player **ply_lst, int numP, enemy *enm_lst,
								  int numE, int caster, int targ);
			bool skillPass(int);

};
