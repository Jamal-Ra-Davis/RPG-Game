#include "../headers/item.h"
#include "../headers/player.h"
#include "../headers/enemy.h"
#include "../headers/Safe_Input.h"
#include "../headers/skill.h"
#include "../headers/FileFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
//#include <cstdlib>
using namespace std;

void printInventory(item** inv, int inv_size)
{
	printf("Inventory\n");
	printf("-----------------------------\n");
	printf("ID   Name                  Stock   Equip\n");
	printf("===  ====================  ======  =======\n");
	
	for (int k=0; k<inv_size; k++)
	{
		if (inv[k] != NULL)
		{
			printf("%2d.  %-20s  x%-5d  ", k+1, inv[k]->getName(), inv[k]->getStock());
			if (inv[k]->getType() == 4)
				printf("Yes\n");
			else
				printf("No\n");
		}
		else
			printf("%2d.  %-20s  %-6s  N/A\n", k+1, "<Empty Slot>", "N/A");
	}
	printf("\n");
}

void recieveItem(item **inv, int inv_size, item* loot)
{
	bool loop = true;

	fprintf(stderr, "DEBUG: Item's name is %s\n", loot->getName());
	
	for (int i=0; i<inv_size; i++)
	{
		if (inv[i] != NULL)
		{
			if (inv[i]->getID() == loot->getID())
			{
				int newStock = inv[i]->getStock() + loot->getStock();
				inv[i]->setStock(newStock);
				delete loot;
				loop = false;
				break;
			}
		}
	}
	if (loop)
	{
		for (int i=0; i<inv_size; i++)
		{
			if (inv[i] == NULL)
			{
				inv[i] = loot;
				loop = false;
				break;
			}
		}
	}
	if (loop)
	{
		printf("Inventory full, %s was discarded...\n", loot->getName());
		delete loot;
	}	
}
void discardItem(item** inv, int inv_size, int idx)
{
	if ((idx < 0)||(idx >= inv_size))
	{
		printf("Invalid selection\n");
		return;
	}
	if (inv[idx] != NULL)
	{
		printf("Discard %s?\n", inv[idx]->getName());
		int sel = getSel("1. Yes", "2. No");
		if (sel == 1)
		{
			printf("Discarding %s...\n", inv[idx]->getName());
			delete inv[idx];
			inv[idx] = NULL;
		}
		else
		{
			printf("Cancelling action...\n");
		}
	}
	else
	{
		printf("You did not select anything to discard\n");
	}
	
}

//----------------------------------------
/*
item* getItem(int id)
{
	item* temp;
	switch(id)
	{
		case 1:
		{
			temp = new healthPotion;
			temp->set_vals("Potion", 1, 99, 1, 0, 100);
			((healthPotion*)temp)->set_pts(50);
			break;
		}
		case 2:
		{
			temp = new healthPotion;
			temp->set_vals("Mid-Potion", 1, 99, 2, 0, 250);
			((healthPotion*)temp)->set_pts(150);
			break;
		}
		case 3:
		{
			temp = new manaPotion;
			temp->set_vals("Mana Potion", 1, 99, 3, 0, 150);
			((manaPotion*)temp)->set_pts(20);
			break;
		}
		case 4:
		{
			temp = new manaPotion;
			temp->set_vals("Mid-Mana Potion", 1, 99, 4, 0, 400);
			((manaPotion*)temp)->set_pts(50);
			break;
		}
		case 5:
		{
			temp = new grenade;
			temp->set_vals("Grenade", 1, 99, 5, 2, 75);
			((grenade*)temp)->set_pts(50);
			break;
		}
		case 6:
		{
			temp = new grenade;
			temp->set_vals("Mid-Grenade", 1, 99, 6, 2, 200);
			((grenade*)temp)->set_pts(100);
			break;
		}
		case 7:
      {
         temp = new revive;
         temp->set_vals("Revive", 1, 99, 7, 0, 250);
         ((manaPotion*)temp)->set_pts(1);
         break;
      }
		case 8:
      {
         temp = new revive;
         temp->set_vals("Mid-Revive", 1, 99, 8, 0, 500);
         ((manaPotion*)temp)->set_pts(100);
         break;
      }
		case 9:
		{
			temp = new equipment;
			temp->set_vals("Staff of BookSmarts", 1, 99, 9, 4, 1500);
			((equipment*)temp)->set_pts(0, 0, 0, 0, 0, 10, 4); 
			//type, health, mp, att, def, mag, mdef
			break;
		}
		case 10:
      {
         temp = new equipment;
         temp->set_vals("The Orifice Maker", 1, 99, 10, 4, 1500);
         ((equipment*)temp)->set_pts(0, 0, 0, 12, 0, 0, 0);
         break;
      }
		case 11:
      {
         temp = new equipment;
         temp->set_vals("Big-Ass Shield", 1, 99, 11, 4, 1500);
         ((equipment*)temp)->set_pts(0, 0, 0, 2, 8, 0, 3);
         break;
      }
		case 12:
      { 
         temp = new equipment;
         temp->set_vals("Poofy Hat", 1, 99, 12, 4, 900);
         ((equipment*)temp)->set_pts(1, 0, 5, 0, 3, 0, 3);
         break;
      }
		case 13:
      {
         temp = new equipment;
         temp->set_vals("Snuggly Shirt", 1, 99, 13, 4, 1000);
         ((equipment*)temp)->set_pts(2, 5, 0, 0, 4, 0, 0);
         break;
      }
		case 14:
      {
         temp = new equipment;
         temp->set_vals("Steel-Toed Boots", 1, 99, 14, 4, 800);
         ((equipment*)temp)->set_pts(3, 0, 0, 2, 5, 0, 0);
         break;
      }
		case 15:
		{
			temp = new skillTeach;
			temp->set_vals("Cure 1 Tech", 1, 99, 15, 0, 400);
			((skillTeach*)temp)->set_ID(1);
			break;
		}
		case 16:
      {
         temp = new skillTeach;
         temp->set_vals("Revive 1 Tech", 1, 99, 16, 0, 500);
         ((skillTeach*)temp)->set_ID(7);
         break;
      }
	}
	return temp;
}
*/
item* getItem(int id)
{
	int cnt = lineCount("./files/Item_List.txt");
	item *temp;
	FILE *fp;	
	char *line;

	int ID, class_type;
	char name[32];
	int init_stk, max_stk, targ_type, price;

//	fprintf(stderr, "%d lines in item list\n", cnt);
	if ((id > cnt)||(id == 0))
	{
		fprintf(stderr, "Item id did not match file's\n");
		return NULL;
	}

	line = getWholeLine("./files/Item_List.txt", id);

	fp = fopen("./files/tempItem.txt", "w");
   fprintf(fp, "%s \n", line);
   fclose(fp);
   fp = fopen("./files/tempItem.txt", "r");
	free(line);

	fscanf(fp, " %d %d '%[^']' %d %d %d %d ", &ID, &class_type, name, 
			 &init_stk, &max_stk, &targ_type, &price);

	if (ID != id)
		fprintf(stderr, "You dun goofed with the item fetching!\n");

	switch (class_type)
	{
		case 0://Potion
		{
			int heal_pts;
			fscanf(fp, " %d ", &heal_pts);
			
			temp = new healthPotion;
			temp->set_vals(name, init_stk, max_stk, ID, targ_type, price);
			((healthPotion*)temp)->set_pts(heal_pts);
			break;
		}
		case 1://Mana Potion
		{
			int mana_pts;
			fscanf(fp, " %d ", &mana_pts);
			
			temp = new manaPotion;
			temp->set_vals(name, init_stk, max_stk, ID, targ_type, price);
			((manaPotion*)temp)->set_pts(mana_pts);
			break;
		}
		case 2://Grenade
		{
			int dam_pts;
			fscanf(fp, " %d ", &dam_pts);

			temp = new grenade;
			temp->set_vals(name, init_stk, max_stk, ID, targ_type, price);
			((grenade*)temp)->set_pts(dam_pts);
			break;
		}
		case 3://Revive
		{
			int rev_pts;
			fscanf(fp, " %d ", &rev_pts);

			temp = new revive;
			temp->set_vals(name, init_stk, max_stk, ID, targ_type, price);
			((revive*)temp)->set_pts(rev_pts);
			break;
		}
		case 4://Equipment
		{
			int eqp_type, health, mp, att, def, mag, mdef;
			fscanf(fp, " %d %d %d %d %d %d %d ", &eqp_type, &health, &mp, &att,
					 &def, &mag, &mdef); 

			temp = new equipment;
			temp->set_vals(name, init_stk, max_stk, ID, targ_type, price);
			((equipment*)temp)->set_pts(eqp_type, health, mp, att, def, mag, mdef);
			//type, health, mp, att, def, mag, mdef
			break;
		}
		case 5://Skill Tech
		{
			int skill_id;
			fscanf(fp, " %d ", &skill_id);			

			temp = new skillTeach;
			temp->set_vals(name, init_stk, max_stk, ID, targ_type, price);
         ((skillTeach*)temp)->set_ID(skill_id);
			break;
		}
	}
	fclose(fp);
	return temp;
}


//-------------------------------------------------------------------

void item::set_vals(char* N, int S, int MS, int ID, int T, int V)
{
	sprintf(name, "%s", N);
	stock = S; Max_stock = MS; id = ID;
	type = T; value = V;
}
void item::setStock(int A)
{
	if (A >= Max_stock)
		stock = Max_stock;
	else
		stock = A;
}
void item::decrementStock()
{
	stock--;
	if (stock < 0)
		stock = 0;
}
void item::removeStock(int num)
{
	stock = stock - num;
	if (stock < 0)
		stock = 0;
}
//-------------------------------------------------------------------

void healthPotion::use(player **ply_lst, int numP, int targ)
{
	if (stock > 0)
	{
		ply_lst[targ]->restoreHealth(heal_pts);
		stock--;
	}
	else
		printf("%s out of stock, cannot use.\n", name);
}
void healthPotion::use(player **ply_lst, int numP, enemy *enm_lst, int numE, 
                       int targ)
{
	if (stock > 0)
	{ 
		ply_lst[targ]->restoreHealth(heal_pts);
		stock--;
	}
	else
		printf("%s out of stock, cannot use.\n", name); 
}
void healthPotion::printInfo()
{
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("%s can be used in and out of battle to restore a player's health.\n", name);
	printf("Restores %d health points when used.\n\n", heal_pts);
}
void healthPotion::set_pts(int PTS)
{
	heal_pts = PTS;
}

//-------------------------------------------------------------------

void manaPotion::use(player **ply_lst, int numP, int targ)
{
	if (stock > 0)
	{
		ply_lst[targ]->restoreMP(mana_pts);
		stock--;
	}
	else
		printf("%s out of stock, cannot use.\n", name);
}
void manaPotion::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
							int targ)
{
	if (stock > 0)
	{
		ply_lst[targ]->restoreMP(mana_pts);
		stock--;
	}
	else
		printf("%s out of stock, cannot use.\n", name);
}
void manaPotion::printInfo()
{
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("%s can be used in and out of battle to restore a player's mana.\n", name);
	printf("Restores %d mana points when used.\n\n", mana_pts);
}
void manaPotion::set_pts(int PTS)
{
	mana_pts = PTS;
}

//-------------------------------------------------------------------

void grenade::use(player **ply_lst, int numP, int targ)
{
	printf("Cannot use %s outside of battle.\n", name);
}
void grenade::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
						int targ)
{
	if (stock > 0)
	{
		enm_lst[targ].takeDamage(damage_pts);
		stock--;
	}
	else
		printf("%s out of stock, cannot use.\n", name);
}
void grenade::printInfo()
{
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("%s can only be used inside of battle to damage an enemy.\n", name);
	printf("Deals %d damage to an enemy regardless of it's defense.\n\n", damage_pts);
}
void grenade::set_pts(int PTS)
{
	damage_pts = PTS;
}

//-------------------------------------------------------------------

void revive::use(player **ply_lst, int numP, int targ)
{
   if (stock > 0)
   {
      ply_lst[targ]->revive(revive_pts);
      stock--;
   }
   else
      printf("%s out of stock, cannot use.\n", name);
}
void revive::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					  int targ)
{
   if (stock > 0)
   {
      ply_lst[targ]->revive(revive_pts);
      stock--;
   }
   else
      printf("%s out of stock, cannot use.\n", name);
}
void revive::printInfo()
{
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("%s can only used in and out of battle to revive a dead player.\n", name);
	printf("Revives a player from the dead and restores %d health.\n\n", revive_pts);
}
void revive::set_pts(int PTS)
{
   revive_pts = PTS;
}

//-------------------------------------------------------------------

void equipment::use(player **ply_lst, int numP, int targ)
{
	printf("%s is a piece of equipment and cannot be used\n", name);
}
void equipment::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
						  int targ)
{
	printf("%s is a piece of equipment and cannot be used\n", name);
}
void equipment::printInfo()
{
	char *Equip_names[4] = {"Weapon", "Head", "Chest", "Legs"};
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("Equipment Type: %s\n", Equip_names[equip_type]);
	printf("Health Bonus: %d%\n", health);
	printf("MP Bonus: %d%\n", mp);
	printf("Attack Bonus: %d\n", att);
	printf("Defense Bonus: %d\n", def);
	printf("Magic Bonus: %d\n", mag);
	printf("Magic Defense Bonus: %d\n\n", mdef);
}
void equipment::set_pts(int T, int H, int MP, int A, int D, int M, int MD)
{
	equip_type = T;
	health = H;
	mp = MP;
	att = A;
	def = D;
	mag = M;
	mdef = MD;
}

//-------------------------------------------------------------------

void skillTeach::use(player **ply_lst, int numP, int targ)
{
	if (stock > 0)
	{
		skill *temp = getSkill(skill_id);
		printf("Teach %s the skill %s?\n", ply_lst[targ]->getName(), temp->get_name());
		int sel = getSel("1. Yes", "2. No");
		if (sel == 1)
		{
			if (!ply_lst[targ]->allSkillRepeat(temp->getID()))
			{
				ply_lst[targ]->pushSkill(temp);
				printf("%s learned %s\n", ply_lst[targ]->getName(), temp->get_name());
				stock--;
			}
			else
			{
				printf("%s already knows %s, cancelling action...\n",  ply_lst[targ]->getName(), temp->get_name());
				delete temp;
			}
		}
		else
		{
			printf("Cancelling action...\n");
			delete temp;
		}
	}
	else
		printf("%s out of stock, cannot use.\n", name);
}
void skillTeach::use(player **ply_lst, int numP, enemy *enm_lst, int numE,
							int targ)
{
	printf("Cannot use %s in battle...\n", name);	
}
void skillTeach::printInfo()
{
	skill *temp = getSkill(skill_id);
	printf("%s Desription:\n", name);
	printf("------------------------\n");
	printf("%s can only be used outside of battle to teach a player the skill \"%s\".\n\n",
			 name, temp->get_name());
	delete temp;
}
void skillTeach::set_ID(int ID)
{
	skill_id = ID;
}
