#include "../headers/skill.h"
#include "../headers/player.h"
#include "../headers/enemy.h"
#include "../headers/FileFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

/*
skill* getSkill(int id)
{
   skill *temp = NULL;
	//FILE* fp;
	//fp = fopen("./files/skills.txt", "r");
	//Read in information from file, use it for set_vals
	if (id < 1)
		return NULL;

   switch(id)
   {
      case 1:
      {
         temp = new HealSpell;
         temp->set_vals("Cure 1", 1, 10, 0);
         ((HealSpell*)temp)->set_pts(12);
         break;
      }
      case 2:
      {
         temp = new HealSpell;
         temp->set_vals("Cure 2", 2, 20, 0);
         ((HealSpell*)temp)->set_pts(30);
         break;
      }
      case 3:
      {
         temp = new AttSpell;
         temp->set_vals("Fire 1", 3, 10, 2);
         ((AttSpell*)temp)->set_pts(10);
         break;
      }
      case 4:
      {
         temp = new AttSpell;
         temp->set_vals("Fire 2", 4, 20, 2);
         ((AttSpell*)temp)->set_pts(25);
         break;
      }
		case 5:
      {  
         temp = new AttSpellAll;
         temp->set_vals("All-Fire 1", 5, 35, 3);
         ((AttSpellAll*)temp)->set_pts(10);
         break;
      }
		case 6:
      {  
         temp = new AttSpellAll;
         temp->set_vals("All-Fire 2", 6, 60, 3);
         ((AttSpellAll*)temp)->set_pts(25);
         break;
      }
      case 7:
      {
         temp = new RevSpell;
         temp->set_vals("Revive 1", 7, 30, 0);
         ((RevSpell*)temp)->set_pts(1);
         break;
      }
      case 8:
      {
         temp = new RevSpell;
         temp->set_vals("Revive 2", 8, 50, 0);
         ((RevSpell*)temp)->set_pts(100);
         break;
      }
		case 9:
      {
         temp = new E_AttSpell;
         temp->set_vals("Ice 1", 9, 15, 0);
         ((E_AttSpell*)temp)->set_pts(6);
         break;
      }
		case 10:
      {
         temp = new E_AttSpellAll;
         temp->set_vals("All-Ice 1", 10, 50, 1);
         ((E_AttSpellAll*)temp)->set_pts(6);
         break;
      }
		case 11:
		{
			temp = new E_CripplingShot;
			temp->set_vals("Crippling Shot", 11, 75, 1);
			break;
		}
   }
   return temp;
}
*/
skill* getSkill(int id)
{
	int cnt = lineCount("./files/Skill_List.txt");
	skill *temp;
	FILE *fp;
	char *line;

	int ID, class_type;
	char name[32];
	int targ_type, mp_cost;

// fprintf(stderr, "%d lines in item list\n", cnt);
	if ((id > cnt)||(id == 0))
	{
		fprintf(stderr, "Item id did not match file's\n");
		return NULL;
	}

	line = getWholeLine("./files/Skill_List.txt", id);

	fp = fopen("./files/tempSkill.txt", "w");
	fprintf(fp, "%s \n", line);
	fclose(fp);
	fp = fopen("./files/tempSkill.txt", "r");
	free(line);

	fscanf(fp, " %d %d '%[^']' %d %d ", &ID, &class_type, name,
			 &targ_type, &mp_cost);

	if (ID != id)
		fprintf(stderr, "You dun goofed with the item fetching!\n");

	switch (class_type)
	{
		case 0://HealSpell
		{
			int heal_pts;
			fscanf(fp, " %d ", &heal_pts);			

			temp = new HealSpell;
			temp->set_vals(name, ID, mp_cost, targ_type);
			((HealSpell*)temp)->set_pts(heal_pts);
			break;
		}
		case 1://AttSpell
		{
			int att_pts;
			fscanf(fp, " %d ", &att_pts);

			temp = new AttSpell;
			temp->set_vals(name, ID, mp_cost, targ_type);
			((AttSpell*)temp)->set_pts(att_pts);
			break;
		}
		case 2://AttSpellAll
		{
			int att_pts;
			fscanf(fp, " %d ", &att_pts);

			temp = new AttSpellAll;
			temp->set_vals(name, ID, mp_cost, targ_type);
			((AttSpellAll*)temp)->set_pts(att_pts);
			break;
		}
		case 3://RevSpell
		{
			int heal_pts;
			fscanf(fp, " %d ", &heal_pts);

			temp = new RevSpell;
			temp->set_vals(name, ID, mp_cost, targ_type);
			((RevSpell*)temp)->set_pts(heal_pts);
			break;
		}
		case 4://E_AttSpell
		{
			int att_pts;
			fscanf(fp, " %d ", &att_pts);

			temp = new E_AttSpell;
			temp->set_vals(name, ID, mp_cost, targ_type);
			((E_AttSpell*)temp)->set_pts(att_pts);
			break;
		}
		case 5://E_AttSpellAll
		{
			int att_pts;
			fscanf(fp, " %d ", &att_pts);
			
			temp = new E_AttSpellAll;
			temp->set_vals(name, ID, mp_cost, targ_type);
			((E_AttSpellAll*)temp)->set_pts(att_pts);
			break;
		}
		case 6://E_CripplingShot
		{
			temp = new E_CripplingShot;
			temp->set_vals(name, ID, mp_cost, targ_type);
			break;
		}
	}
	fclose(fp);
	return temp;	
}
//----------------------------------------------------------------------------------

void skill::set_vals(char* NAME, int ID, int MPC, int TYPE)
{
   sprintf(name, "%s", NAME);
   id = ID;
   mp_cost = MPC;
	type = TYPE;
	next = NULL;
}

//----------------------------------------------------------------------------------

void HealSpell::set_pts(int A)
{
   heal_pts = A;
}
void HealSpell::use(player **ply_lst, int numP, int caster, int targ)
{
   if (ply_lst[caster]->getMP() >= mp_cost)
   {
      int heal = ply_lst[caster]->getMag_btl()*heal_pts;
      ply_lst[targ]->restoreHealth(heal);
      ply_lst[caster]->removeMP(mp_cost);
   }
   else
   {
      printf("%s does not have enough MP to cast %s\n", ply_lst[caster]->getName(),
             name);
   }
}
void HealSpell::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
                    int caster, int targ)
{
	printf("Target recieved: %d\n", targ);
   if (ply_lst[caster]->getMP() >= mp_cost)
   {
      int heal = ply_lst[caster]->getMag_btl()*heal_pts;
      ply_lst[targ]->restoreHealth(heal);
      ply_lst[caster]->removeMP(mp_cost);
   }
   else
   {
      printf("%s does not have enough MP to cast %s\n", ply_lst[caster]->getName(),
             name);
   }
}
void HealSpell::printInfo()
{
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("%s can be used in and out of battle to restore a player's health.\n", name);
	printf("Healing factor: %d\n\n", heal_pts);
}
//----------------------------------------------------------------------------------

void AttSpell::set_pts(int A)
{
   att_pts = A;
}
void AttSpell::use(player **ply_lst, int numP, int caster, int targ)
{
   printf("%s cannot  be used outside of battle\n", name);
}
void AttSpell::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
                    int caster, int targ)
{
   if (ply_lst[caster]->getMP() >= mp_cost)
   {
		printf("**DEBUG** Pmag = %d, Emdef = %d, att_pts = %d\n", ply_lst[caster]->getMag_btl(),
				 enm_lst[targ].getMdef_btl(), att_pts);
		int mag_p = ply_lst[caster]->getMag_btl();
		int mdef_e = enm_lst[targ].getMdef_btl();
		
//		int dam = (int)((ply_lst[caster]->getMag_btl()/((float)(enm_lst[targ].getMdef_btl())))*att_pts);
		float dam = (mag_p - mdef_e)/((float)(mag_p + mdef_e));
		dam = 3*dam*(mag_p/4.0) + mag_p;
		dam *= att_pts;
      enm_lst[targ].takeDamage((int)dam);
      ply_lst[caster]->removeMP(mp_cost);
   }
   else
   {
      printf("%s does not have enough MP to cast %s\n", ply_lst[caster]->getName(),
             name);
   }  
}
void AttSpell::printInfo()
{
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("%s can only be used inside of battle to damage an enemy\n", name);
	printf("Damage factor: %d\n\n", att_pts);
}

void AttSpellAll::set_pts(int A)
{  
   att_pts = A;
}
void AttSpellAll::use(player **ply_lst, int numP, int caster, int targ)
{  
   printf("%s cannot  be used outside of battle\n", name);
}
void AttSpellAll::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
                    int caster, int targ)
{  
   if (ply_lst[caster]->getMP() >= mp_cost)
   {  
		bool allDead = true;
		for (int i=0; i<numE; i++)
		{
			if (enm_lst[i].isDead() == false)
			{
				printf("**DEBUG** Pmag = %d, Emdef = %d, att_pts = %d\n", ply_lst[caster]->getMag_btl(),
						 enm_lst[i].getMdef_btl(), att_pts);
				allDead = false;
				int mag_p = ply_lst[caster]->getMag_btl();
				int mdef_e = enm_lst[i].getMdef_btl();

//				int dam = (int)((ply_lst[caster]->getMag_btl()/((float)(enm_lst[i].getMdef_btl())))*att_pts);
				float dam = (mag_p - mdef_e)/((float)(mag_p + mdef_e));
				dam = 3*dam*(mag_p/4.0) + mag_p;
				dam *= att_pts;
      		enm_lst[i].takeDamage((int)dam);
			}
		}
		if (allDead == false)
			ply_lst[caster]->removeMP(mp_cost);
		else
			printf("All enemies are dead\n");
   }
   else
   {  
      printf("%s does not have enough MP to cast %s\n", ply_lst[caster]->getName(),
             name);
   }
}
void AttSpellAll::printInfo()
{
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("%s can only be used inside of battle to damage all enemies\n", name);
	printf("Damage factor: %d\n\n", att_pts);
}

//----------------------------------------------------------------------------------

void RevSpell::set_pts(int A)
{
   restore_pts = A;
}
void RevSpell::use(player **ply_lst, int numP, int caster, int targ)
{
   if (ply_lst[caster]->getMP() >= mp_cost)
   {
      if (ply_lst[targ]->isDead())
      {
         ply_lst[targ]->revive(restore_pts);
         ply_lst[caster]->removeMP(mp_cost);
      }
      else
      {
         printf("%s is not dead, %s will have no effect\n", ply_lst[caster]->getName(),
                name);
      }
   }
   else
   {
      printf("%s does not have enough MP to cast %s\n", ply_lst[caster]->getName(),
             name);
   }
}
void RevSpell::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
                    int caster, int targ)
{
   if (ply_lst[caster]->getMP() >= mp_cost)
   {
      if (ply_lst[targ]->isDead())
      {
         ply_lst[targ]->revive(restore_pts);
         ply_lst[caster]->removeMP(mp_cost);
      }
      else
      {
         printf("%s is not dead, %s will have no effect\n", ply_lst[caster]->getName(),
                name);
      }
   }
   else
   {
      printf("%s does not have enough MP to cast %s\n", ply_lst[caster]->getName(),
             name);
   }
}
void RevSpell::printInfo()
{
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("%s can be used in and out of battle to revive a player from the dead\n", name);
	printf("Health Points Restored: %d\n\n", restore_pts);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

void E_AttSpell::set_pts(int A)
{
   att_pts = A;
}
void E_AttSpell::use(player **ply_lst, int numP, int caster, int targ)
{
   printf("%s cannot  be used outside of battle\n", name);
}
void E_AttSpell::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
                    int caster, int targ)
{
   if (enm_lst[caster].getMP() >= mp_cost)
   {
//		int dam = (int)((enm_lst[caster].getMag_btl()/((float)(ply_lst[targ]->getMdef_btl())))*att_pts);
//      ply_lst[targ]->takeDamage(dam);
//      enm_lst[caster].removeMP(mp_cost);
//---------------------------------------------

		printf("**DEBUG** Emag = %d, Pmdef = %d, att_pts = %d\n", enm_lst[caster].getMag_btl(),
             ply_lst[targ]->getMdef_btl(), att_pts);
		int mag_e = enm_lst[caster].getMag_btl();
		int mdef_p = ply_lst[targ]->getMdef_btl();

		float dam = (mag_e - mdef_p)/((float)(mag_e + mdef_p));
		dam = 3*dam*(mag_e/4.0) + mag_e;
		dam *= att_pts;

		ply_lst[targ]->takeDamage(dam);
		enm_lst[caster].removeMP(mp_cost);
   }
   else
   {
      printf("%s does not have enough MP to cast %s\n", enm_lst[caster].getName(),
             name);
   }
}

void E_AttSpellAll::set_pts(int A)
{
   att_pts = A;
}
void E_AttSpellAll::use(player **ply_lst, int numP, int caster, int targ)
{
   printf("%s cannot  be used outside of battle\n", name);
}
void E_AttSpellAll::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
                    int caster, int targ)
{  
	if (enm_lst[caster].getMP() >= mp_cost)
   {
      bool allDead = true;
      for (int i=0; i<numP; i++)
      {  
         if (ply_lst[i]->isDead() == false)
         {
				printf("**DEBUG** Emag = %d, Pmdef = %d, att_pts = %d\n", enm_lst[caster].getMag_btl(),
						 ply_lst[i]->getMdef_btl(), att_pts);
            allDead = false;
//            int dam = (int)((enm_lst[caster].getMag_btl()/((float)(ply_lst[i]->getMdef_btl())))*att_pts);
//            ply_lst[i]->takeDamage(dam);

				//----------------------------

				int mag_e = enm_lst[caster].getMag_btl();
				int mdef_p = ply_lst[i]->getMdef_btl();

				float dam = (mag_e - mdef_p)/((float)(mag_e + mdef_p));
				dam = 3*dam*(mag_e/4.0) + mag_e;
				dam *= att_pts;

				ply_lst[i]->takeDamage(dam);
         }
      } 
      if (allDead == false)
         enm_lst[caster].removeMP(mp_cost);
      else
         printf("All enemies are dead\n");
   }
   else
   {
      printf("%s does not have enough MP to cast %s\n", enm_lst[caster].getName(),
             name);
   }
}

//----------------------------------------------------------------------------------

void E_CripplingShot::use(player **ply_lst, int numP, int caster, int targ)
{
	printf("%s cannot  be used outside of battle\n", name);
}
void E_CripplingShot::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
								  int caster, int targ)
{
	if (enm_lst[caster].getMP() >= mp_cost)
	{
		bool allDead = true;
		for (int i=0; i<numP; i++)
		{
			if (ply_lst[i]->isDead() == false)
			{
				allDead = false;
				int dam = ply_lst[i]->getHealth()-1;
				ply_lst[i]->takeDamage(dam);
			}
		}
		if (allDead == false)
			enm_lst[caster].removeMP(mp_cost);
		else
			printf("All enemies are dead\n");
	}
	else
	{
		printf("%s does not have enough MP to cast %s\n", enm_lst[caster].getName(),
				 name);
	}
}
