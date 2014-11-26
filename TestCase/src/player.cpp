#include "../headers/player.h"
#include "../headers/enemy.h"
#include "../headers/skill.h"
#include "../headers/item.h"
#include "../headers/Safe_Input.h"
#include "../headers/FileFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <assert.h>
using namespace std;

player* getNewPlayer()
{
	FILE *fp;
   fp = fopen("./files/temp_name.txt", "w");
   fprintf(fp, "'Temp Name'");
   fclose(fp);

	int class_sel;
   char* temp_name;
///   printf("Starting new game...\n\n");
   while(1)
   {
      printf("Create new character\n");
      printf("-------------------------\n");
      printf("Choose a name for you character...\n");
      printf("Note: Name must be less than 30 characters.\n");
//      printf("Open the file ./files/temp_name.txt and write character's name in ''s\n");
      printf("Enter name: ");
      temp_name = getString();
      while (1)
      {
/*
         while (1)
         {
            printf("Have you entered the name and saved the file?\n");
            int confirm = getSel("1. Yes", "2. No");
            if (confirm == 1)
               break;
         }
         FILE *fp = fopen("./files/temp_name.txt", "r");
         temp_name = (char*)malloc(128*sizeof(char));
         assert(fscanf(fp, " '%[^']' ", temp_name) == 1);
         fclose(fp);
*/
         if (strlen(temp_name) > 31)
         {
         // printf("Name len = %d\n", strlen(temp_name));
				printf("Invalid entry, name is too long. Please try again...\n");
            free(temp_name);
				printf("Enter name: ");
				temp_name = getString();
         }
         else
            break;
		}
      printf("You typed %s...\n\n", temp_name);

      printf("Choose a class for you character...\n");
      class_sel = getSel("1. Warrior", "2. Defender", "3. Mage");
      class_sel--;
      char *classes[3] = {"warrior", "defender", "mage"};

      printf("Your character's name will be %s and they will be a %s...\n", temp_name, classes[class_sel]);
      printf("Is this ok?\n");
      int sel = getSel("1. Yes", "2. No", "3. Cancel");
		if (sel == 1)
			break;
      else if (sel == 2)
      {  
         free(temp_name);
         printf("Resetting character...\n");
      }
      else
		{
			printf("Cancelling action...\n");
			free(temp_name);
         return NULL;
		}
   }
   player *character = new player;
   character->setPlayer(temp_name, class_sel);
	free(temp_name);

	return character;
}
player* getRandomPlayer()
{
	int class_sel;
	char *temp_name;
	while(1)
	{
		int gender = rand()%2;
		char file_name[64];
		if (gender == 0)
		{
			sprintf(file_name, "./files/FemaleNamesFormatted.txt");
		}
		else
		{
			sprintf(file_name, "./files/MaleNamesFormatted.txt");
		}
		int count = lineCount(file_name);
		int line = rand()%count + 1;

		temp_name = getLine(file_name, line);
		class_sel = rand()%3;

		char *classes[3] = {"warrior", "defender", "mage"};
      printf("Your character's name will be %s and they will be a %s...\n", temp_name, classes[class_sel]);
		printf("Is this ok?\n");
      int sel = getSel("1. Yes", "2. No");
		if (sel == 2)
      {  
         free(temp_name);
         printf("Resetting character...\n");
      }
		else
			break;
	}
	player *character = new player;
   character->setPlayer(temp_name, class_sel);
   free(temp_name);

   return character;
}
//-----------------------------------------------------------------

player::player()
{
	Equipment = NULL;
	skill_lst = NULL;
	equip_size = 0;;
	skill_cnt = 0;
	allSkills = NULL;
}
player::player(char* NAME, int CLASS)
{
	setPlayer(NAME, CLASS);
/*
        sprintf(name, "%s", NAME);
        dead = false;
        level = 1;
        exp = 0;
	num_skills = 3;
	initSkill();
        //Default warrior
        Max_Health = 50;
	Max_mp = 20;
        att = 6;
        def = 4;
	mag = 4;
	mdef = 4;
        if (CLASS == 1)//Defender
        {
                Max_Health = 50;
		Max_mp = 20;
                att = 4;
                def = 6;
		mag = 4;
		mdef = 5;
        }
        else if (CLASS == 2)//Mage
        {
                Max_Health = 40;
		Max_mp = 40;
                att = 4;
                def = 4;
		mag = 6;
		mdef = 6;
        }

	statusReset();
        Health = Max_Health;
	mp = Max_mp;
        setExp();
*/
}
player::~player()
{
	/*
	for (int i=0; i<num_skills; i++)
	{
		if (skill_lst[i] != NULL)
			delete skill_lst[i];
	}
	*/
	for (int i=0; i<equip_size; i++)
	{
		if (Equipment[i] != NULL)
			delete Equipment[i];
	}
	if (Equipment != NULL)
		delete [] Equipment;
	if (skill_lst != NULL)
		delete [] skill_lst;
	freeAllSkills();
}
player::player(char* NAME, int MAX_HEALTH, int MAX_MP, int ATT, int DEF, int MAG,
					int MDEF)
{
	setPlayer(NAME, MAX_HEALTH, MAX_MP, ATT, DEF, MAG, MDEF);
/*
        sprintf(name, "%s", NAME);
        dead = false;
        level = 1;
        exp = 0;
	num_skills = 3;
	initSkill();

        Max_Health = MAX_HEALTH;
	Max_mp = MAX_MP;

        att = ATT;
        def = DEF;
	mag = MAG;
	mdef = MDEF;
        Health = Max_Health;

	statusReset();
        setExp();
*/
}
void player::setPlayer(char* NAME, int CLASS)
{
	sprintf(name, "%s", NAME);
	dead = false;
	level = 1;
	exp = 0;
	num_skills = 10;
	skill_cnt = 0;
	skill_lst = new skill*[num_skills];

	allSkills = NULL;
	allSkill_num = 0;

	initSkill();

	equip_size = 4;
	Equipment = new item*[equip_size];
	for (int i=0; i<equip_size; i++)
	{
		Equipment[i] = NULL;
	}
		
	class_type = CLASS;
	switch (CLASS)
	{
		case 1://Defender
		{
			Max_Health = 50;
			Max_mp = 20;
			att = 4;
			def = 6;
			mag = 4;
			mdef = 5;
			
			pushSkill(1);
			pushSkill(2);
			pushSkill(7);
			//maybe change back to the ids pushskill
			recieveSkill(getIndexSkill(1));
			recieveSkill(getIndexSkill(2));
			recieveSkill(getIndexSkill(3));
			break;
		}
		case 2://Mage
		{
			Max_Health = 40;
			Max_mp = 40;
			att = 4;
			def = 4;
			mag = 6;
			mdef = 6;

			pushSkill(2);
         pushSkill(4);
         pushSkill(6);
			pushSkill(1);
			pushSkill(5);
			//maybe change back to the ids pushskill
         recieveSkill(getIndexSkill(1));
         recieveSkill(getIndexSkill(2));
         recieveSkill(getIndexSkill(3));
			break;
		}
		default://Warrior
		{
			class_type = 0;
			Max_Health = 50;
			Max_mp = 20;
			att = 6;
			def = 4;
			mag = 4;
			mdef = 4;

			pushSkill(1);
         pushSkill(3);
         pushSkill(4);
         //maybe change back to the ids pushskill
         recieveSkill(getIndexSkill(1));
         recieveSkill(getIndexSkill(2));
         recieveSkill(getIndexSkill(3));
			break;
		}
	}
	char *classes[3] = {"Warrior", "Defender", "Mage"};
	sprintf(class_name, "%s", classes[class_type]);
	Health = 0;
   mp = 0;
	statusReset();
	Health = Max_Health_btl;
	mp = Max_mp_btl;
	setExp();
}
void player::setPlayer(char* NAME, int MAX_HEALTH, int MAX_MP, int ATT, int DEF,
							  int MAG, int MDEF)
{
	sprintf(name, "%s", NAME);
	dead = false;
	level = 1;
	exp = 0;
	num_skills = 10;
	skill_cnt = 0;
	skill_lst = new skill*[num_skills];

	allSkills = NULL;	
	allSkill_num = 0;
	initSkill();
	equip_size = 4;

	Equipment = new item*[equip_size];
   for (int i=0; i<equip_size; i++)
      Equipment[i] = NULL;
//NOTE: Figure out way to select skills for this initilaztion option

	Max_Health = MAX_HEALTH;
	Max_mp = MAX_MP;

	att = ATT;
	def = DEF;
	mag = MAG;
	mdef = MDEF;
	class_type = getClass();

	Health = 0;
	mp = 0;
	statusReset();
	Health = Max_Health_btl;
	mp = Max_mp_btl;

	setExp();
}
void player::setPlayer(FILE* FP)
{
	int temp;
	num_skills = 10;
	skill_cnt = 0;

	allSkills = NULL;
	allSkill_num = 0;	
	skill_lst = new skill*[num_skills];

	initSkill();
	equip_size = 4;
	Equipment = new item*[equip_size];
   for (int i=0; i<equip_size; i++)
      Equipment[i] = NULL;

	int temp_health, temp_mp;
	fscanf(FP, " '%[^']' %d %d %d %d %d %d %d %d %d %d %d %d ", name, &class_type, &temp, &level, &exp,
			 &temp_health, &Max_Health, &temp_mp, &Max_mp, &att, &def, &mag, &mdef);
	dead = temp;

	char *classes[3] = {"Warrior", "Defender", "Mage"};
	sprintf(class_name, "%s", classes[class_type]);

	int skill_loop;
	fscanf(FP, " %d ", &skill_loop);
	for (int i=0; i<skill_loop; i++)
	{
		int temp;
		fscanf(FP, " %d ", &temp);
		if (temp > 0)
			pushSkill(temp);
	}

	fscanf(FP, " %d ", &num_skills);//Use if you want to have dynamic # of skills per character	
	for (int i=0; i<num_skills; i++)
	{
		int temp;
		fscanf(FP, " %d ", &temp);
		if (temp > 0)
		{
			skill *tmp_skl;
			tmp_skl = getIndexSkill(temp);
			recieveSkill(tmp_skl);
		}
	}
	fscanf(FP, " %d ", &equip_size);
	for (int i=0; i<equip_size; i++)
	{
		int temp;
		fscanf(FP, " %d ", &temp);
		if (temp > 0)
		{
			item *tmp_item;
			tmp_item = getItem(temp);
			Equipment[i] = tmp_item;
		}
	}
//	fprintf(FP, "'%s' %d %d %d %d %d %d %d %d %d", name, dead, level, exp, Max_Health, Max_mp, att, def, mag, mdef);
	Health = 0;
   mp = 0;
	statusReset();
//	Health = Max_Health_btl;
	Health = temp_health;
	if (Health > Max_Health_btl)
		Health = Max_Health_btl;

	if (dead)
		Health = 0;
//	mp = Max_mp_btl;
	mp = temp_mp;
	if (mp > Max_mp_btl)
		mp = Max_mp_btl;
	setExp();
}
void player::initSkill()
{
	for (int i=0; i<num_skills; i++)
		skill_lst[i] = NULL;
}
int player::getClass()
{
	float stat_sum, att_per, def_per, mag_per;

	stat_sum = att + def + mag;
	att_per = att/stat_sum;
	def_per = def/stat_sum;
	mag_per = mag/stat_sum;

	if ((def_per > att_per)&&(def_per > mag_per))
	{
		sprintf(class_name, "Defender");
		return 1;
	}
	else if ((def_per > att_per)&&(def_per > mag_per))
	{
		sprintf(class_name, "Mage");
		return 2;
	}
	else
	{
		sprintf(class_name, "Warrior");
		return 0;
	}

}
bool player::isDead()
{
	if (Health <= 0)
	{
		Health = 0;
		dead = true;
	}
	return dead;
}
int player::takeDamage(int damage)
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
int player::restoreHealth(int heal_pts)
{
	if (dead)
		return 1;
	int temp = Health;
	Health += heal_pts;
	if (Health >= Max_Health_btl)
		Health = Max_Health_btl;
	printf("%s recovered %d health points. Current health: %d/%d\n", name,
			 Health-temp, Health, Max_Health_btl);
	return 0;
}
int player::restoreMP(int A)
{
	if (dead)
		return 1;
	int temp = mp;
	mp += A;
	if (mp > Max_mp_btl)
		mp = Max_mp_btl;
	printf("%s's mp resored by %d. Current mp: %d/%d\n", name, mp-temp, mp, Max_mp_btl);
	return 0;
}
int player::removeMP(int A)
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
		printf("%s lost %d mp. Current mp: %d/%d\n", name, temp-mp, mp, Max_mp_btl);
	return 0;
}
int player::revive(int A)
{
	if (!dead)
		return 1;
	dead = false;
	printf("%s has been revived from the dead\n", name);
	restoreHealth(A);
	return 0;
}
void player::Defend()
{
	sumEquipment();
	def_btl = (2*def)+eqp_def;
	printf("%s's defense doubled for a turn\n", name);
}
void player::sumEquipment()
{
	eqp_Health=0, eqp_mp=0, eqp_att=0, eqp_def=0, eqp_mag=0, eqp_mdef=0;

   for (int i=0; i<equip_size; i++)
   {   
      if (Equipment[i] != NULL)
      {  
         double hlt, mana;

         hlt = (((equipment*)Equipment[i])->getHealth())*0.01;
         hlt = Max_Health*hlt;
         eqp_Health += (int)hlt;

         mana = (((equipment*)Equipment[i])->getMP())*0.01;
         mana = Max_mp*mana;
         eqp_mp += (int)mana;

         eqp_att += ((equipment*)Equipment[i])->getAtt();
         eqp_def += ((equipment*)Equipment[i])->getDef();
         eqp_mag += ((equipment*)Equipment[i])->getMag();
         eqp_mdef += ((equipment*)Equipment[i])->getMDef();
      }
   }
}
void player::statusReset()
{
	sumEquipment();
	/*
	int eqp_Health=0, eqp_mp=0, eqp_att=0, eqp_def=0, eqp_mag=0, eqp_mdef=0;
	
	for (int i=0; i<equip_size; i++)
	{
		if (Equipment[i] != NULL)
		{
			double hlt, mana;
			
			hlt = (((equipment*)Equipment[i])->getHealth())*0.01;
			hlt = Max_Health*hlt;	
			eqp_Health += (int)hlt;

			mana = (((equipment*)Equipment[i])->getMP())*0.01;
			mana = Max_mp*mana;
			eqp_mp += (int)mana;

			eqp_att += ((equipment*)Equipment[i])->getAtt();
			eqp_def += ((equipment*)Equipment[i])->getDef();
			eqp_mag += ((equipment*)Equipment[i])->getMag();
			eqp_mdef += ((equipment*)Equipment[i])->getMDef();
		}
	}
	*/

	Max_Health_btl = Max_Health + eqp_Health;
	Max_mp_btl = Max_mp + eqp_mp;
	def_btl = def + eqp_def;
	att_btl = att + eqp_att;
	mag_btl = mag + eqp_mag;
	mdef_btl = mdef +eqp_mdef;

	if (Health >= Max_Health_btl)
		Health = Max_Health_btl;
	if (mp >= Max_mp_btl)
		mp = Max_mp_btl;
}
void player::printStats()
{
	char *classes[3] = {"Warrior", "Defender", "Mage"};
	printf("%s's Stats\n", name);
	printf("-----------------------\n");
	printf("Class: %s\n", classes[class_type]);
	printf("Level: %d\nExp to next Lvl: %d\n", level, exp_req[level-1]-exp);
	printf("Total Exp: %d\n\n", exp);
	printf("Health: %d/%d\nMP: %d/%d\n", Health, Max_Health_btl, mp, Max_mp_btl);
	printf("Attack: (%d) %d\n", att, att_btl);
	printf("Defense: (%d) %d\n", def, def_btl);
	printf("Magic: (%d) %d\n", mag, mag_btl);
	printf("Magic Defense: (%d) %d\n", mdef, mdef_btl);
}
void player::printSkills()
{
	printf("%s's Battle Skills:\n", name);
	printf("--------------------------------------\n");
	for (int i=0; i<skill_cnt; i++)//you replaced num_skills with skill_cnt
	{
		if (skill_lst[i] != NULL)
		{
			printf("%d. %-16s  MP Cost: %d\n", i+1, skill_lst[i]->get_name(),
				 	 skill_lst[i]->get_MPcost());
		}
		else
		{
			printf("%d. <Empty Skill Slot>\n", i+1);
		}
	}
	printf("\n");
}
void player::printAllSkills()
{
	printf("%s's Skill Bank:\n", name);
   printf("--------------------------------------\n");
	if (allSkills == NULL)
	{
		printf("%s has no skills.\n", name);
		return;
	}
	skill *loop_ptr;
	int i=1;
	for (loop_ptr = allSkills; loop_ptr != NULL; loop_ptr = loop_ptr->getNext(), i++)
	{
		printf("%d. %-16s  MP Cost: %d\n", i, loop_ptr->get_name(), loop_ptr->get_MPcost());
	}
	printf("\n");
}
void player::useSkill(int skl_idx, player **ply_lst, int numP, int caster, int targ)
{
	if (skill_lst[skl_idx] == NULL)
	{
		printf("Skill is invalid, %s cannot use it...\n", name);
		return;
	}
	if (dead)
   {
      printf("%s is dead and cannot cast %s...\n", name, skill_lst[skl_idx]->get_name());
      return;
   }
	printf("%s used %s\n", name, skill_lst[skl_idx]->get_name());
	skill_lst[skl_idx]->use(ply_lst, numP, caster, targ);
}
void player::useSkillBtl(int skl_idx, player **ply_lst, int numP, enemy *enm_lst,
								 int numE, int caster, int targ)
{
	if (skill_lst[skl_idx] == NULL)
   {
      printf("Skill is invalid, %s cannot use it...\n", name);
      return;
   }
	if (dead)
   { 
      printf("%s is dead and cannot cast %s...\n", name, skill_lst[skl_idx]->get_name());
      return;
   }
	printf("%s used %s\n", name, skill_lst[skl_idx]->get_name());
	skill_lst[skl_idx]->use(ply_lst, numP, enm_lst, numE, caster, targ);
}
bool player::skillPass(int idx)
{
	if (skill_lst[idx] != NULL)
		return true;
	else
		return false;	
}
int player::skillType(int idx)
{
	return skill_lst[idx]->getType();
}
void player::viewSkillInfoIdx(int idx)
{
	if ((idx < 0)||(idx >= num_skills))
	{
		printf("Invalid selection...\n");
		return;
	}
	if (skill_lst[idx] == NULL)
	{
		printf("You did not select a skill...\n");
		return;
	}
	skill_lst[idx]->printInfo();
}
void player::viewSkillInfo()
{
	int skill_sel;
	printSkills();
	printf("%d. Cancel action\n", getSkill_cnt()+1);

	skill_sel = getSel(getSkill_cnt()+1);
	skill_sel--;	

	if (skill_sel < getSkill_cnt())
	{
		viewSkillInfoIdx(skill_sel);
	}
	else
		printf("Cancelling action...\n");
}
int player::recieveExp(int exp_pts)
{
	if (dead)
	{
		printf("%s is dead, and recieved no experience.\n", name);
		return 1;
	}
	printf("%s recieved %d experience.\n", name, exp_pts);
	exp += exp_pts;
	levelUp();
	return 0;
}
void player::setExp()
{
	int levelExp;
	levelExp = 300;
	exp_req[0] = 300;

	for (int i=1; i<99; i++)
	{
		levelExp = (int)(levelExp*1.1);
		exp_req[i] = exp_req[i-1] + levelExp;
	}
}
void player::levelUp()
{
	int temp_level = 1;
	int i;
	for (i=0; i<99; i++)
	{
		if (exp < exp_req[i])
			break;
	}
	temp_level = i+1;

	if (temp_level > level)
	{
		printf("Level up! %s's level has increased by %d.\n", name,
				 temp_level - level);
		statUp(temp_level - level);
		level = temp_level;
		if (level < 100)
			printf("%d experience needed to reach level %d.\n\n", 
					 exp_req[level-1]-exp, level+1);

	}
}
void player::statUp(int num_lvl)
{
	for (int i=0; i<num_lvl; i++)
	{
		int att_buff = 0, def_buff = 0, mag_buff = 0, mdef_buff = 0, Health_buff = 0, MP_buff = 0;
		switch (class_type)
		{
			case 0:
			{
				if (75 > rand()%100)
					att_buff = rand()%2 + 1;
				if (60 > rand()%100)
					def_buff = rand()%2 + 1;
				if (65 > rand()%100)
					mag_buff = rand()%2 + 1;
				if (60 > rand()%100)
					mdef_buff = rand()%2 + 1;
				break;
			}
			case 1:
			{
				if (60 > rand()%100)
					att_buff = rand()%2 + 1;
				if (75 > rand()%100)
					def_buff = rand()%2 + 1;
				if (60 > rand()%100)
					mag_buff = rand()%2 + 1;
				if (65 > rand()%100)
					mdef_buff = rand()%2 + 1;
				break;
			}
			case 2:
			{
				if (55 > rand()%100)
					att_buff = rand()%2 + 1;
				if (55 > rand()%100)
					def_buff = rand()%2 + 1;
				if (75 > rand()%100)
					mag_buff = rand()%2 + 1;
				if (75 > rand()%100)
					mdef_buff = rand()%2 + 1;
				break;
			}
		}
//		att_buff = rand()%2;
//		def_buff = rand()%2;
//		mag_buff = rand()%2;
//		mdef_buff = rand()%2;
//		Health_buff = ((int)(Max_Health*(.1 + (rand()%3 - 1)*.025)))*(rand()%2);
//		MP_buff = ((int)(Max_mp*(.1 + (rand()%3 - 1)*.025)))*(rand()%2);
		Health_buff = ((int)(Max_Health*(.1 + (rand()%3 - 1)*.025)));
		MP_buff = ((int)(Max_mp*(.1 + (rand()%3 - 1)*.025)));
		
//		if ((att_buff+def_buff+Health_buff) <= 0)
//		{
//			i--;
//		}
		if ((att_buff+def_buff+mag_buff+mdef_buff) <= 0)
			i--;
		else
		{
			if (att_buff > 0)
			{
				printf("Attack has increased by %d. Base attack is now %d.\n", att_buff,
						 att_buff+att);
				att += att_buff;
			}
			if (def_buff > 0)
			{
				printf("Defense has increased by %d. Base defense is now %d.\n", def_buff,
					 	 def_buff+def);
				def += def_buff;
			}
			if (mag_buff > 0)
			{
				printf("Magic has increased by %d. Base magic is now %d.\n", mag_buff,
						 mag_buff+mag);
				mag += mag_buff;
			}
			if (mdef_buff > 0)
			{
				printf("Magic defense has increased by %d. Base magic defense is now %d.\n",
						 mdef_buff, mdef_buff+mdef);
				mdef += mdef_buff;
			}
			if (Health_buff > 0)
			{
				printf("Max Health has increased by %d. Base max Health is now %d.\n",
						 Health_buff, Health_buff+Max_Health);
			Max_Health += Health_buff;
			}
			if (MP_buff > 0)
			{
				printf("Max MP has increased by %d. Base max MP is now %d.\n", MP_buff,
						 MP_buff+Max_mp);
				Max_mp += MP_buff;
			}
		}
	}
	statusReset();
	Health = Max_Health_btl;
	mp = Max_mp_btl;
}
void player::recieveSkill(int skill_id)
{
	int i;
	skill *temp;
	for (i=0; i<num_skills; i++)
	{
		if (skill_lst[i] == NULL)
		{
			skill_lst[i] = getSkill(skill_id);
			temp = skill_lst[i];
			if (skill_lst[i] != NULL)
				skill_cnt++;
			break;
		}
	}
	if (i == num_skills)
		printf("%s's skill list is full, %s cannot be equipped\n", name, temp->get_name());
}
void player::recieveSkill(skill *skill_in)
{
   int i;
   for (i=0; i<num_skills; i++)
   {
      if (skill_lst[i] == NULL)
      {
         skill_lst[i] = skill_in;
         if (skill_lst[i] != NULL)
            skill_cnt++;
			printf("Skill count: %d\n", skill_cnt);
         break;
      }
   }
   if (i == num_skills)
      printf("%s's skill list is full, %s cannot be equipped\n", name, skill_in->get_name());
}
bool player::skillRepeat(int skill_id)
{
	bool repeat = false;
	for (int i=0; i<num_skills; i++)
	{
		if (skill_lst[i] != NULL)
		{
			if (skill_lst[i]->getID() == skill_id)
			{
				repeat = true;
				break;
			}
		}
	}
	return repeat;
}
bool player::allSkillRepeat(int skill_id)
{
	bool repeat = false;
	skill *loop_ptr;

	for (loop_ptr = allSkills; loop_ptr != NULL; loop_ptr = loop_ptr->getNext())
	{
		if (loop_ptr->getID() == skill_id)
		{
			repeat = true;
			break;
		}
	}
	return repeat;
}
void player::pushSkill(int skill_id)
{
	skill *newSkill = getSkill(skill_id);

	skill *loop_ptr;
	if (allSkills == NULL)
	{
		allSkills = newSkill;
		allSkill_num++;
		return;
	}
	
	for (loop_ptr = allSkills; loop_ptr->getNext() != NULL; loop_ptr = loop_ptr->getNext());

	loop_ptr->setNext(newSkill);
	allSkill_num++;
}
void player::pushSkill(skill* skill_in)
{

   skill *loop_ptr;
   if (allSkills == NULL)
   {
      allSkills = skill_in;
		allSkill_num++;
      return;
   }

   for (loop_ptr = allSkills; loop_ptr->getNext() != NULL; loop_ptr = loop_ptr->getNext());

   loop_ptr->setNext(skill_in);
	allSkill_num++;
}
skill* player::getIndexSkill(int idx)
{
	if (idx > allSkill_num)
		return NULL;

	skill *loop_ptr = allSkills;
	int i;
	for (i=1, loop_ptr = allSkills; i<idx; i++, loop_ptr = loop_ptr->getNext());

	printf("Character: %s, Idx: %d, Skill: %s, Skill_id: %d, allskill_num: %d\n", name, idx, 
			 loop_ptr->get_name(), loop_ptr->getID(), allSkill_num);
	return loop_ptr;
}
int player::getIndexSkillID(int idx)
{
	if (idx > allSkill_num)
		return 0;
	skill *loop_ptr = allSkills;
   int i;
   for (i=1, loop_ptr = allSkills; i<idx; i++, loop_ptr = loop_ptr->getNext());

   return (loop_ptr->getID());
}
int player::findIndex(int id)
{
	skill *loop_ptr;
	int index;
	int i;
	for (loop_ptr = allSkills, i=1; loop_ptr != NULL; loop_ptr = loop_ptr->getNext(), i++)
	{
		if (id == loop_ptr->getID())
		{
			index = i;
			break;
		}
	}
	return index;
}
void player::freeAllSkills()
{
	while(allSkills != NULL)
	{
		skill *temp;
		temp = allSkills->getNext();
		delete allSkills;
		allSkills = temp;
	}
}
void player::equipSkill(int idx)
{
	skill *temp = getIndexSkill(idx);
	if (temp == NULL)
		printf("Error Invalid selection\n");
	else
	{
		if (skillRepeat(temp->getID()) == false)
		{
			printf("%s can now use %s in battle...\n", name, temp->get_name());
			recieveSkill(temp);
		}
		else
		{
			printf("%s is already equipped\n", temp->get_name());
		}
	}
}
//Assume index you recieve is 0 based, not 1 based
void player::unequipSkill(int idx)
{

//	int idx = IDX - 1;
	//Had assumed index was 1 based, but that was getting messy
	if ((idx < num_skills)&&(idx >= 0))
	{
		skill_lst[idx] = NULL;
		skill_cnt--;
		compressSkillList();
	}
	else
	{
		printf("Cannot unequip, selection invalid\n");
	}
}
void player::compressSkillList()
{
	for (int i=0; i<num_skills-1; i++)
	{
		for (int j=i+1; j<num_skills; j++)
		{
			if ((skill_lst[i] == NULL)&&(skill_lst[j] != NULL))
			{
				skill *temp = skill_lst[i];
				skill_lst[i] = skill_lst[j];
				skill_lst[j] = temp;
			}
		}
	}
}
void player::printEquipment()
{
	char *TYPE[4] = {"Weapon:", "Head:", "Chest:", "Legs:"};
	printf("%s's Equipment:\n", name);
   printf("-----------------------\n");
	printf("ID   Type      Name\n");
	printf("===  ========  =================\n");
	for (int i=0; i<equip_size; i++)
	{
		if (Equipment[i] != NULL)
			printf("%d.   %-8s  %s\n", i+1, TYPE[i], Equipment[i]->getName());
		else
			printf("%d.   %-8s  %s\n", i+1, TYPE[i], "<Empty Slot>");
	}
}
/*
void player::equipGear(item **inv, int inv_size, int IDX)
{
	int idx = IDX-1;
	if ((idx >= inv_size)||(idx < 0))
	{
		printf("Invalid selection\n");
		statusReset();
		return;
	}
	if (inv[idx] == NULL)
	{
		printf("You did not select anything to equip\n");
		statusReset();
		return;
	}
	if (inv[idx]->getType() != 4)
	{
		printf("%s is not a piece of equipment, cannot equip\n", inv[idx]->getName());
		statusReset();
		return;
	}
	int equip_type = ((equipment*)inv[idx])->getEquipType();

	if (Equipment[equip_type] == NULL)
	{
		if (inv[idx]->getStock() == 1)
		{
			Equipment[equip_type] = inv[idx];
			inv[idx] = NULL;
			statusReset();
			return;
		}
		else
		{
			int ID = inv[idx]->getID();
			inv[idx]->decrementStock();
			Equipment[equip_type] = getItem(ID);
			statusReset();
			return;
		}
	}	

	printf("%s is already equipped.\n", Equipment[equip_type]->getName());
	printf("Swapping it for %s...\n", inv[idx]->getName());
	if (inv[idx]->getStock() == 1)
	{
		item *temp = Equipment[equip_type];
		Equipment[equip_type] = inv[idx];
		inv[idx] = NULL;
		recieveItem(inv, inv_size, temp);
		statusReset();
		return;
	}
	else
	{
		item *temp = Equipment[equip_type];
		int ID = inv[idx]->getID();
		inv[idx]->decrementStock();
		Equipment[equip_type] = getItem(ID);
		recieveItem(inv, inv_size, temp);
		statusReset();
		return;
	}
}
*/
item* player::equipGear(item *in)
{
	int equip_type = ((equipment*)in)->getEquipType();
	if (Equipment[equip_type] == NULL)
	{
		printf("%s equipped %s...\n", name, in->getName());
		Equipment[equip_type] = in;
		statusReset();
		return NULL;
	}
	printf("%s is already equipped.\n", Equipment[equip_type]->getName());
	printf("Would you like to swap %s for %s?\n", Equipment[equip_type]->getName(), in->getName());
	int sel = getSel("1. Yes", "2. No");
	if (sel == 1)
	{
		printf("%s unequipped %s, and equipped %s...\n", name, Equipment[equip_type]->getName(), in->getName());
		item *temp = Equipment[equip_type];
		Equipment[equip_type] = in;
		statusReset();
		return temp;
	}
	else
	{
		printf("%s did not equip %s...\n", name, in->getName());
		statusReset();
		return in;
	}
}
/*
void player::unequipGear(item **inv, int inv_size, int IDX)
{
	int idx = IDX-1;
	if ((idx >= equip_size)||(idx < 0))
   {  
      printf("Invalid selection\n");
		statusReset();
      return;
   }
	if (Equipment[idx] == NULL)
	{
		printf("%s has no equipment in slot %d, cannot unequip...\n", name, IDX);
		statusReset();
		return;
	}
	recieveItem(inv, inv_size, Equipment[idx]);
	Equipment[idx] = NULL;
	statusReset();
}
*/
item* player::unequipGear(int idx)
{
	if ((idx >= equip_size)||(idx < 0))
   {  
      printf("Invalid selection\n");
      statusReset();
      return NULL;
   }
   if (Equipment[idx] == NULL)
   {  
      printf("%s has no equipment in slot %d, cannot unequip...\n", name, idx+1);
      statusReset();
      return NULL;
   }

	item *temp = Equipment[idx];
	Equipment[idx] = NULL;
	statusReset();
	return temp;
}
void player::saveInfo(FILE* FP)
{
	//Copy of working function
        //fprintf(FP, "'%s' %d %d %d %d %d %d %d %d %d\n", name, dead, level, exp, Max_Health, Max_mp, att, def, mag, mdef);
	fprintf(FP, "'%s' %d %d %d %d %d %d %d %d %d %d %d %d", name, class_type, dead, level, exp, 
			  Health, Max_Health, mp, Max_mp, att, def, mag, mdef);
	
	fprintf(FP, " %d", allSkill_num);
	for (int i=1; i<=allSkill_num; i++)
	{
		int temp_id;
		temp_id = getIndexSkillID(i);
		if (temp_id == 0)
			fprintf(FP, " 0");
		else
			fprintf(FP, " %d", temp_id);	
	}

	fprintf(FP, " %d", num_skills);
	for (int i=0; i<num_skills; i++)
	{
		if (skill_lst[i] == NULL)
			fprintf(FP, " 0");
		else
		{
			int val = findIndex(skill_lst[i]->getID());
//			fprintf(FP, " %d", skill_lst[i]->getID());
			fprintf(FP, " %d", val);
		}
	}
	fprintf(FP, " %d", equip_size);
	for (int i=0; i<equip_size; i++)
	{
		if (Equipment[i] == NULL)
			fprintf(FP, " 0");
		else
		{
			int temp = Equipment[i]->getID();
			fprintf(FP, " %d", temp);
		}	
	}
	fprintf(FP, "\n");
}


