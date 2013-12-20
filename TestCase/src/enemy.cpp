#include "../headers/enemy.h"
#include "../headers/skill.h"
#include "../headers/FileFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <assert.h>
using namespace std;

int getEnemyRange()
{
	int cnt = lineCount("./files/Enemy_List.txt");
	return cnt;
}

enemy* getEnemy(int ID)
{
	int cnt;
	cnt = lineCount("./files/Enemy_List.txt");

	if ((ID > cnt)||(ID == 0))
		return NULL;
	
	enemy *temp;
	temp = new enemy;
	char *stats;	
	
	int id;
	char name[32];
	int M_hlt;
	int M_mp;
	int att;
	int def;
	int mag;
	int mdef;

	stats = getWholeLine("./files/Enemy_List.txt", ID);
	FILE *fp;


	fp = fopen("./files/tempEnemy.txt", "w");
	fprintf(fp, "%s \n", stats); 
	fclose(fp);
	fp = fopen("./files/tempEnemy.txt", "r");

	fscanf(fp, " %d '%[^']' %d %d %d %d %d %d ", &id, name, &M_hlt, &M_mp, &att,
          &def, &mag, &mdef);	

	
	free(stats);

	if (id != ID)
		printf("You dun goofed with the enemy fetching\n");

	int num_skills;
   fscanf(fp, " %d ", &num_skills);
	temp->setEnemy(id, name, M_hlt, M_mp, att, def, mag, mdef, num_skills);
   for (int i=0; i<num_skills; i++)
   {
      int skill;
      fscanf(fp, " %d ", &skill);
		temp->recieveSkill(skill);
   }
	fclose(fp);
	return temp;
}

enemy::enemy(int ID, char* NAME, int MH_B, int MP_B, int ATT_B, int DEF_B, 
				 int MAG_B, int MDEF_B, int NUM_SKL)
{
	/*
	sprintf(name, "%s", NAME);
	dead = false;
	Max_Health_b = MH_B;
	Max_mp_b  = MP_B;
	att_b = ATT_B;
	def_b = DEF_B;
	mag_b = MAG_B;
	mdef_b = MDEF_B;
	*/

	setEnemy(ID, NAME, MH_B, MP_B, ATT_B, DEF_B, MAG_B, MDEF_B, NUM_SKL);
}
enemy::~enemy()
{
	for (int i=0; i<num_skills; i++)
	{
		if (skill_lst[i] != NULL)
			delete skill_lst[i];
	}
	if (skill_lst != NULL)
		delete [] skill_lst;
}
void enemy::setEnemy(int ID, char* NAME, int MH_B, int MP_B, int ATT_B, int DEF_B, 
							int MAG_B, int MDEF_B, int NUM_SKL)
{
	id = ID;
	sprintf(name, "%s", NAME);
	dead = false;
	Max_Health_b = MH_B;
	Max_mp_b  = MP_B;
	att_b = ATT_B;
	def_b = DEF_B;
	mag_b = MAG_B;
	mdef_b = MDEF_B;

	num_skills = NUM_SKL;
	if (num_skills < 1)
		skill_lst = NULL;
	else
		skill_lst = new skill*[num_skills];
	initSkill();
}
void enemy::setLevel(int LEVEL)
{
	Level = LEVEL;
	//Scale stats according to level
	//Currently placeholder, put better equation in later
	att = (int)(att_b*(.25*Level));
	def = (int)(def_b*(.25*Level));
	mag = (int)(mag_b*(.25*Level));
	mdef = (int)(mdef_b*(.25*Level));

	statusReset();	

	Max_Health = (int)(Max_Health_b*(.75*Level));
	Max_mp = (int)(Max_mp_b*(.75*Level));

	Health = Max_Health;
	mp = Max_mp;
	exp = (Level*50) + (int)(Max_Health*.2) + (int)((att+def)*5);

	double temp = rand()%51 - 25;
	temp = temp + 100;
	temp = temp/100;

	double temp2 = (Max_Health/10.0) + (Max_mp/10.0) + att + def + mag + mdef;
	gold_value = (int)(temp*temp2);
}
void enemy::resetSkillLst()
{
	if (num_skills > 0)
	{
		skill **temp;
		temp = new skill*[num_skills];
//		int *skillIDs;
//		skillIDs = (int*)malloc(num_skills*sizeof(int));
		for (int i=0; i<num_skills; i++)
		{
			printf("In reset skills function\n");
			temp[i] = getSkill(skill_lst[i]->getID());
//			recieveSkill(skill_lst[i]->getID());
		}
		skill_lst = temp;
	}
}
bool enemy::isDead()
{
	if (Health <= 0)
	{
		Health = 0;
		dead = true;
	}
	return dead;
}
int enemy::takeDamage(int damage)
{
	if (dead)
		return 1;
	if (damage >= Health)
	{
		printf("Attack worth %d damage\n", damage);
		printf("%s took %d damage.\n%s is dead.\n", name, Health, name);
	}
	Health -= damage;
	isDead();
	if (Health > 0)
		printf("%s took %d damage.\n", name, damage);
	return 0;
}
int enemy::restoreHealth(int heal_pts)
{
	if (dead)
		return 1;
	Health += heal_pts;
	if (Health >= Max_Health)
		Health = Max_Health;
	return 0;
}
int enemy::restoreMP(int A)
{
	if (dead)
		return 1;
	int temp = mp;
	mp += A;
	if (mp > Max_mp)
		mp = Max_mp;
	printf("%s's mp resored by %d. Current mp: %d/%d\n", name, mp-temp, mp,
			 Max_mp);
	return 0;
}
int enemy::removeMP(int A)
{
	if (dead)
		return 1;
	int temp = mp;
	mp -= A;
	if (mp <= 0)
	{
		mp = 0;
		printf("%s is out of mp\n", name);
	}
	else
		printf("%s lost %d mp. Current mp: %d/%d\n", name, temp-mp, mp,
				 Max_mp);
	return 0;
}
int enemy::revive(int A)
{
	if (!dead)
		return 1;
	dead = false;
	printf("%s has been revived from the dead\n", name);
	restoreHealth(A);
	return 0;
}
void enemy::Defend()
{
	def_btl = 2*def;
	printf("%s's defense doubled for a turn\n", name);
}
void enemy::statusReset()
{
	def_btl = def;
	att_btl = att;
	mag_btl = mag;
	mdef_btl = mdef;
}
void enemy::initSkill()
{
	for (int i=0; i<num_skills; i++)
		skill_lst[i] = NULL;
}
void enemy::recieveSkill(int skill_id)
{
	int i;
	for (i=0; i<num_skills; i++)
	{
		if (skill_lst[i] == NULL)
		{
			skill_lst[i] = getSkill(skill_id);
			break;
		}
	}
	if (i == num_skills)
		printf("Skill list full, skill discarded\n");
}
void enemy::useSkillBtl(int skl_idx, player **ply_lst, int numP, enemy *enm_lst,
								int numE, int caster, int targ)
{
	printf("%s used %s\n", name, skill_lst[skl_idx]->get_name());
	printf("%s shoud cost %d mp, and %s has %d mp\n", skill_lst[skl_idx]->get_name(),
			 skill_lst[skl_idx]->get_MPcost(), name, mp);
	skill_lst[skl_idx]->use(ply_lst, numP, enm_lst, numE, caster, targ);	
}
bool enemy::skillPass(int idx)
{
	if (skill_lst[idx] != NULL)
		return true;
	else
		return false;		
}




