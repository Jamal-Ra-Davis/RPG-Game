#pragma once
#include <stdio.h>
#include <stdlib.h>

class player;
class skill;
class enemy;
class item;

player* getNewPlayer();
player* getRandomPlayer();

class player
{
		char name[32];
		skill **skill_lst;
		int num_skills;
		int skill_cnt;
		
		skill *allSkills;
		int allSkill_num;

		item **Equipment;
		int equip_size;

		int class_type;
		char class_name[32];
		int Max_Health, Health, Max_mp, mp;
		int att, def, mag, mdef;

		int level;
		int exp;
		bool dead;
		int exp_req[99];

		int att_btl, def_btl, mag_btl, mdef_btl;
		int Max_Health_btl, Max_mp_btl;

		int eqp_Health, eqp_mp, eqp_att, eqp_def, eqp_mag, eqp_mdef;

		public:
			player();
			~player();
			player(char*, int);
			player(char*, int, int, int, int, int, int);
			void setPlayer(char*, int);
			void setPlayer(char*, int, int, int, int, int, int);
			void setPlayer(FILE*);
			void initSkill();
			int getClass();//determines class based off base stats

			char* getName()
				{return name;}
			int getLevel()
				{return level;}
			int getNum_skills()
				{return num_skills;}
			int getSkill_cnt()
				{return skill_cnt;}
			int getAllSkillNum()
				{return allSkill_num;}
			int getEquipSize()
				{return equip_size;}
			int getClassType()
				{return class_type;}//returns class type
			char *getClassName()
				{return class_name;}
			int getHealth()
				{return Health;}
			int getMax_Health()
				{return Max_Health;}
			int getMax_Health_btl()
				{return Max_Health_btl;}
			int getMP()
				{return mp;}
			int getMax_MP()
				{return Max_mp;}
			int getMax_MP_btl()
				{return Max_mp_btl;}
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


			bool isDead();//Check if health is < 0, sets and returns death status

			int takeDamage(int);//Returns 1 if you try to damage when dead, 0 if things work right
			int restoreHealth(int);//Returns 1 if you try to heal when dead, 0 if things work right
			int restoreMP(int);//Returns 1 if you try to restore mp when dead, 0 if successful
			int removeMP(int);//Returns 1 if you try to remove mp when dead, 0 if successful
			int revive(int);//Returns 1 if you try to revive player that's alive


			void Defend();
			void sumEquipment();
			void statusReset();

			void printStats();
			void printSkills();
			void printAllSkills();
			void useSkill(int skl_idx, player **ply_lst, int numP, int caster, int targ);
			void useSkillBtl(int skl_idx, player **ply_lst, int numP, enemy *enm_lst,
								  int numE, int caster, int targ);
			bool skillPass(int);
			int skillType(int);
			void viewSkillInfoIdx(int);
			void viewSkillInfo();

			int recieveExp(int);//Returns 1 if exp recieved when dead, 0 if successfully got exp
			void setExp();
			void levelUp();
			void statUp(int);

			void recieveSkill(int);
			void recieveSkill(skill*);

			bool skillRepeat(int);
			bool allSkillRepeat(int);

			void pushSkill(int);
			void pushSkill(skill*);
			skill *getIndexSkill(int);
			int getIndexSkillID(int);
			int findIndex(int);			
			void freeAllSkills();

			void equipSkill(int);							
			void unequipSkill(int);			
			void compressSkillList();

			void printEquipment();
			item* equipGear(item*);					
			item* unequipGear(int);			

			void saveInfo(FILE*);
};
