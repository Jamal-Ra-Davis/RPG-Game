#include "../headers/party.h"
#include "../headers/player.h"
#include "../headers/skill.h"
#include "../headers/item.h"
#include "../headers/Safe_Input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <assert.h>
using namespace std;



party::party()
{
	gold = 0;
	num_plys = 0;
	Max_members = 4;
	game_over = false;

	ply_lst = new player*[Max_members];

	inv_size = 10;
	inventory = new item*[inv_size];

	for (int i=0; i<Max_members; i++)
	{
		ply_lst[i] = NULL;
	}
	for (int i=0; i<inv_size; i++)
	{
		inventory[i] = NULL;
	}
}
party::~party()
{
	for (int i=0; i<Max_members; i++)
	{
		if (ply_lst[i] != NULL)
			delete ply_lst[i];
	}
	delete [] ply_lst;

	for (int i=0; i<inv_size; i++)
	{
		if (inventory[i] != NULL)
			delete inventory[i];
	}
	delete [] inventory;
}	
void party::recieveGold(int A)
{
	gold += A;
}
void party::loseGold(int A)
{
	gold -= A;
	if (gold < 0)
		gold = 0;
}
bool party::goldCheck(int cost)
{
	if (cost > gold)
		return false;
	else
		return true;
}
bool party::checkGameOver()
{
	bool test = true;
	for (int i=0; i<num_plys; i++)
	{
		if (ply_lst[i]->isDead() == false)
			test = false;
	}
	game_over = test;
	return game_over;
}
player* party::recievePlayer(player *player_in)
{
	if (num_plys >= Max_members)
	{
		printf("The party is already at max capacity, cannot take on another member\n");
		return player_in;
	}
	
	ply_lst[num_plys] = player_in;
	num_plys++;
	compressParty();
	printf("Current party size is %d\n", num_plys);
	return NULL;
}
void party::compressParty()
{
	for (int i=0; i<Max_members-1; i++)
	{
		for (int j=i+1; j<Max_members; j++)
		{
			if ((ply_lst[i] == NULL)&&(ply_lst[j] != NULL))
			{
				player *temp = ply_lst[i];
				ply_lst[i] = ply_lst[j];
				ply_lst[j] = temp;
			}
		}
	}
}
player* party::removePlayer(int idx)
{
	if ((idx < 0)||(idx >= num_plys))
	{
		printf("Invalid selection...\n");
		return NULL;
	}
	if (num_plys == 1)
	{
		printf("Party only has 1 member, you can not remove the only member of your party.\n");
		return NULL;
	}
	if (ply_lst[idx] != NULL)
	{
		printf("Removing %s from party...\n", ply_lst[idx]->getName());
		player* temp = ply_lst[idx];
		ply_lst[idx] = NULL;
		compressParty();
		num_plys--;
		return temp;
	}	
	else
	{
		printf("Invalid selection...\n");
		return NULL;
	}
}
void party::deletePlayer(int idx)
{
	if ((idx < 0)||(idx >= num_plys))
	{
		printf("Invalid selection...\n");
		return;
	}
	if (num_plys == 1)
	{
		printf("Party only has 1 member, you can not remove the only member of your party.\n");
		return;
	}
	if (ply_lst[idx] != NULL)
	{
		printf("Do you want to permanently remove %s from party?\n", 
				 ply_lst[idx]->getName());
		int sel = getSel("1. Yes", "2. No");
		if (sel == 1)
		{
			player *temp = removePlayer(idx);
			delete temp;
		}
		else
		{	
			printf("Cancelling action...\n");
		}
	}
	else
		printf("Invalid selection...\n");
	
	
}	
void party::printPlayers()
{
	for (int i=0; i<num_plys; i++)
	{
		printf("%d. %s\n", i+1, ply_lst[i]->getName());
	}
}
void party::printInventory()
{
	printf("Inventory\n");
	printf("-----------------------------\n");
	printf("ID   Name                  Stock   Equip\n");
	printf("===  ====================  ======  =======\n");

	for (int k=0; k<inv_size; k++)
	{ 
		if (inventory[k] != NULL)
		{ 
			printf("%2d.  %-20s  x%-5d  ", k+1, inventory[k]->getName(), inventory[k]->getStock());
			if (inventory[k]->getType() == 4)
				printf("Yes\n");
			else
				printf("No\n");
		}
		else
			printf("%2d.  %-20s  %-6s  N/A\n", k+1, "<Empty Slot>", "N/A");
	}
	printf("\n");
}
bool party::inventoryFull()
{
	bool full = true;
	for (int i=0; i < inv_size; i++)
	{
		if (inventory[i] != NULL)
		{
			if (inventory[i]->getStock() == 0)
			{
				delete inventory[i];
				inventory[i] = NULL;
			}
		}
		if (inventory[i] == NULL)
		{
			full = false;
		}
	}
	return full;
}
bool party::partyFull()
{
	bool full = true;
	if (num_plys < Max_members)
		full = false;
	return full;
}
bool party::checkStock(int idx, int num)
{
	if ((idx < 0)||(idx >= inv_size))
	{
		printf("Invalid selection\n");
		return false;
	}
	if (inventory[idx] != NULL)
	{
		int stk = inventory[idx]->getStock();
		if (stk >= num)
			return true;
		else
			return false;
	}
	else
	{
		printf("You did not select anything to remove\n");
		return false;
	}
}
bool party::checkItem(int idx)
{
	bool valid_item = true;
	if ((idx < 0)||(idx >= inv_size))
		valid_item = false;
	if (inventory[idx] == NULL)
		valid_item = false;

	return valid_item;		
}
char* party::getItemName(int idx)
{
	if ((idx < 0)||(idx >= inv_size))
	{
		printf("Invalid selection\n");
		return NULL;
	}
	if (inventory[idx] != NULL)
	{
		return inventory[idx]->getName();
	}
	else
	{
		printf("You did not select anything to remove\n");
		return NULL;
	}
}
int party::getItemValue(int idx)
{  
   if ((idx < 0)||(idx >= inv_size))
   {  
      printf("Invalid selection\n");
      return -1;
   }
   if (inventory[idx] != NULL)
   {  
      return inventory[idx]->getGoldValue();
   }
   else
   {  
      printf("You did not select anything to remove\n");
      return -1;
   }
}
void party::recieveItem(item *loot)
{
	if (loot == NULL)
	{
		printf("Invalid item\n");
		return;
	}
	bool loop = true;

	fprintf(stderr, "DEBUG: Item's name is %s\n", loot->getName());

	for (int i=0; i<inv_size; i++)
	{
		if (inventory[i] != NULL)
		{
			if (inventory[i]->getID() == loot->getID())
			{
				int newStock = inventory[i]->getStock() + loot->getStock();
				inventory[i]->setStock(newStock);
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
			if (inventory[i] == NULL)
			{
				inventory[i] = loot;
				loop = false;
				break;
			}
		}
	}
	if (loop)
	{
		printf("Inventory is full, what would you like to do?\n");
		char sel1[128], sel2[128];
		sprintf(sel1, "1. Swap %s for item in inventory", loot->getName());
		sprintf(sel2, "2. Discard %s", loot->getName());
		int sel = getSel(sel1, sel2);
		if (sel == 1)
		{
			item *temp = swapItem(loot);
			recieveItem(temp);
		}
		else
		{
			printf("%s was discarded...\n", loot->getName());
			delete loot;
			fprintf(stderr, "Stack, why you get smashed!!\n");
		}
	}
}
item* party::removeItem(int idx)
{
	if ((idx < 0)||(idx >= inv_size))
	{
		printf("Invalid selection\n");
		return NULL;
	}
	if (inventory[idx] != NULL)
	{
//		printf("Remove %s?\n", inv[idx]->getName());
//		int sel = getSel("1. Yes", "2. No");
//		if (sel == 1)
//		{

		printf("Removing %s...\n", inventory[idx]->getName());
		item *temp = inventory[idx];	
		inventory[idx] = NULL;
		return temp;
//		}
//		else
//		{
//			printf("Cancelling action...\n");
//		}
	}
	else
	{
		printf("You did not select anything to remove\n");
		return NULL;
	}
}
item* party::removeOneItem(int idx)
{
	if ((idx < 0)||(idx >= inv_size))
	{
		printf("Invalid selection\n");
		return NULL;
	}
	if (inventory[idx] != NULL)
	{
//    printf("Remove %s?\n", inv[idx]->getName());
//    int sel = getSel("1. Yes", "2. No");
//    if (sel == 1)
//    {
		
		printf("Removing one %s...\n", inventory[idx]->getName());
		item *temp;
		if (inventory[idx]->getStock() == 1)
		{
			temp = inventory[idx];
			inventory[idx] = NULL;
		}
		else
		{
			temp = getItem(inventory[idx]->getID());
			inventory[idx]->decrementStock();
		}
		return temp;
//    }
//    else
//    {
//       printf("Cancelling action...\n");
//    }
	}
	else
	{  
		printf("You did not select anything to remove\n");
		return NULL;
	}
}
item* party::removeItems(int idx, int num)
{
	if ((idx < 0)||(idx >= inv_size))
	{
		printf("Invalid selection\n");
		return NULL;
	}
	if (inventory[idx] != NULL)
	{
		printf("Removing %d %ss...\n", num, inventory[idx]->getName());
		item *temp;
		if (inventory[idx]->getStock() == num)
		{
			temp = inventory[idx];
			inventory[idx] = NULL;
		}
		else
		{
			temp = getItem(inventory[idx]->getID());
			temp->setStock(num);
			inventory[idx]->removeStock(num);
		}
		return temp;
	}
	else
	{
		printf("You did not select anything to remove\n");
		return NULL;
	}
}
void party::discardItem(int idx)
{
	if ((idx < 0)||(idx >= inv_size))
   { 
      printf("Invalid selection\n");
      return;
   }
	item *temp = removeItem(idx);
	if (temp != NULL)
	{
		printf("Discard %s?\n", temp->getName());
		int sel = getSel("1. Yes", "2. No");
		if (sel == 1)
		{
			delete temp;
		}
		else
		{
			printf("Cancelling action...\n");
			recieveItem(temp);
		}
	}
	else
	{
		printf("You did not select anything to discard.\n");
	}
}
item* party::swapItem(item *loot)
{
	printInventory();
	printf("Swap %s for which item:", loot->getName());

	int sel = getSel(inv_size);
	sel--;
	
	printf("Swapping %s for %s...\n", inventory[sel]->getName(), loot->getName());
	item *temp = inventory[sel];
	inventory[sel] = loot;
	return temp;
}
//Assume index coming in is 0 based, not 1 based
void party::equipGear(int idx, int player_idx)
{
//	int idx = IDX-1;
   if ((idx >= inv_size)||(idx < 0))
   {
      printf("Invalid selection\n");
      return;
   }
   if (inventory[idx] == NULL)
   {
      printf("You did not select anything to equip\n");
      return;
   }
   if (inventory[idx]->getType() != 4)
   {
      printf("%s is not a piece of equipment, cannot equip\n", inventory[idx]->getName());
      return;
   }


	item *eqp = removeOneItem(idx);
	if (eqp == NULL)
	{
		printf("Invalid item, **DEBUG** removeOneItem derped...\n");
		return;
	}
	item *temp = ply_lst[player_idx]->equipGear(eqp);
	if (temp != NULL)
		recieveItem(temp);
}
void party::unequipGear(int idx, int player_idx)
{
//	int idx = IDX - 1;
	
	item *temp = ply_lst[player_idx]->unequipGear(idx);
	if (temp != NULL)
		recieveItem(temp);
		
}
void party::useItem(int itm_idx, int targ)
{
	if (inventory[itm_idx] == NULL)
	{
		printf("You did not select an item...\n");
		return;
	}
	if ((itm_idx < 0)||(itm_idx >= inv_size))
	{
		printf("Invalid selection...\n");
		return;
	}
	if (inventory[itm_idx] == NULL)
	{
		printf("You did not select an item...\n");
		return;
	}

	printf("Used %s...\n", inventory[itm_idx]->getName()); 
	inventory[itm_idx]->use(ply_lst, num_plys, targ);
	printf("You have %d %s left in inventory\n", inventory[itm_idx]->getStock(), inventory[itm_idx]->getName());
	if (inventory[itm_idx]->getStock() == 0)
	{
		delete inventory[itm_idx];
		inventory[itm_idx] = NULL;
	}
	
}
/*
void party::useItemBtl(int itm_idx, player **ply_lst, int num_plys, enemy *enm_lst, int num_enms, int targ)
{

}
*/
void party::viewItemInfo(int itm_idx)
{
	if ((itm_idx < 0)||(itm_idx >= inv_size))
	{
		printf("Invalid selection...\n");
		return;
	}
	if (inventory[itm_idx] == NULL)
	{
		printf("You did not select an item...\n");
		return;
	}
	inventory[itm_idx]->printInfo();
}
void party::managePlayers()
{
	bool managePlayers = true;
	while (managePlayers)
	{
		printf("Party Management\n");
   	printf("-------------------------------------\n");
		int player_sel;
		printPlayers();
		printf("\n%d. Cancel\n", num_plys+1);
	
		player_sel = getSel(num_plys+1);

		if (player_sel < num_plys+1)
		{
			bool loop = true;
			while (loop)
			{
				int idx = player_sel-1;
				printf("Manage %s\n", ply_lst[idx]->getName());
				printf("-------------------------------------\n");
				int man_sel = getSel("1. View Character Stats", "2. Manage Skills", 
											"3. Manage Equipment", "4. Cancel Action");
//				int idx = player_sel-1;
				switch (man_sel)
				{
					case 1://View Character stats
					{
						ply_lst[idx]->printStats();
						break;
					}
					case 2://Manage SKills
					{
						manageSkills(idx);
						break;
					}
					case 3://Manage Equipment
					{
						manageEquipment(idx);
						break;
					}
					case 4://Cancel Action
					{
						printf("Cancelling action...\n");
						loop = false;
						break;
					}
				}
			}
		}
		else
		{
			printf("Cancelling action...\n");
			managePlayers = false;
		}
	}

}
void party::manageInventory()
{
	bool manageInventory = true;
	while(manageInventory)
	{
		printf("Inventory Management\n");
   	printf("-------------------------------------\n");
		int man_sel = getSel("1. View Inventory", "2. Use Item", 
									"3. View Item Description", "4. Discard Item", 
									"5. Cancel");

		switch (man_sel)
		{
			case 1://View Inventory
			{
				printInventory();
				break;
			}
			case 2://Use Item
			{
				useItem_OB();
				break;
			}
			case 3:
			{
				viewItemInfo_OB();
				break;
			}
			case 4://Discard Item
			{
				printInventory();
				printf("\n%d. Cancel\n", inv_size+1);

				printf("Select Item to discard: ");
				int discard_sel = getSel(inv_size+1);

				if (discard_sel < inv_size+1)
				{
					discard_sel--;
					discardItem(discard_sel);	
				}
				else
				{
					printf("Cancelling action...\n");
//					manageInventory = false;
				}
				break;
			}
			case 5://Cancel
			{
				printf("Cancelling action...\n");
				manageInventory = false;
				break;
			}
		}
	}
}
void party::manageSkills(int idx)
{
	bool manageSkills = true;
	while (manageSkills)
	{
		printf("Skill Management\n");
   	printf("-------------------------------------\n");
		int man_sel = getSel("1. View Battle Skills", "2. View Skill Bank",
									"3. Use Skill", "4. View Skill Description", 
									"5. Equip Skill", "6. Unequip Skill",
									"7. Cancel");
	
		switch(man_sel)
		{
			case 1://View Battle Skills
			{
				ply_lst[idx]->printSkills();
				break;
			}
			case 2://View Skill Bank
			{
				ply_lst[idx]->printAllSkills();
				break;
			}
			case 3://Use Skill
			{
				useSkill_OB(idx);
				break;
			}
			case 4://View Skill Info
			{
				ply_lst[idx]->viewSkillInfo();
				break;
			}
			case 5://Equip skill
			{
				int num_skills = ply_lst[idx]->getAllSkillNum();
				printf("Choose a skill to equip\n");
				ply_lst[idx]->printAllSkills();
				printf("\n%d. Cancel\n", num_skills+1);

				int skl_eqp = getSel(num_skills+1);
				if (skl_eqp < num_skills+1)
				{
					ply_lst[idx]->equipSkill(skl_eqp);	
				}
				else
				{
					printf("Cancelling action...\n");
//					manageSkills = false;
				}
				break;
			}
			case 6://Unequip skill
			{
				int num_skills = ply_lst[idx]->getSkill_cnt();
				printf("Choose a skill to unequip\n");
				ply_lst[idx]->printSkills();
				printf("\n%d. Cancel\n", num_skills+1);

				int skl_eqp = getSel(num_skills+1);
				if (skl_eqp < num_skills+1)
				{
					skl_eqp--;
					ply_lst[idx]->unequipSkill(skl_eqp);
				}
				else
				{
					printf("Cancelling action...\n");
//					manageSkills = false;
				}
				break;
			}
			case 7://Cancel
			{
				printf("Cancelling action...\n");
				manageSkills = false;
				break;
			}
		}
	}
}
void party::manageEquipment(int idx)
{
	bool manageEquipment = true;
	while (manageEquipment)
	{
		printf("Equipment Management\n");
		printf("-------------------------------------\n");
		int man_sel = getSel("1. View Equipment", "2. Equip Gear",
									"3. Unequip Gear", "4. Cancel");

		switch(man_sel)
		{
			case 1://View Equipment
			{
				ply_lst[idx]->printEquipment();
				break;
			}
			case 2://Equip Gear
			{
				printf("Choose a piece of gear to equip\n");
				printInventory();
				printf("%d. Cancel\n", inv_size+1);
				
				int eqp_sel = getSel(inv_size+1);
				if (eqp_sel < inv_size+1)
				{
					eqp_sel--;
					equipGear(eqp_sel, idx);
				}
				else
				{
					printf("Cancelling action...\n");
//					manageEquipment = false;
				}

				break;
			}
			case 3://Unequip Gear
			{
				int eqp_cnt = ply_lst[idx]->getEquipSize();
				printf("Choose a piece of gear to unequip\n");
				ply_lst[idx]->printEquipment();
				printf("\n%d. Cancel\n", eqp_cnt+1);

				int eqp_sel = getSel(eqp_cnt+1);
				if (eqp_sel < eqp_cnt+1)
				{
					eqp_sel--;
					unequipGear(eqp_sel, idx);
				}
				else
				{
					printf("Cancelling action...\n");
//					manageEquipment = false;
				}

				break;
			}
			case 4://Cancel
			{
				printf("Cancelling action...\n");
				manageEquipment = false;
				break;
			}
		}
	}
}
void party::useSkill_OB(int caster)
{
	int skill_sel;
	ply_lst[caster]->printSkills();
	printf("%d. Cancel action\n", ply_lst[caster]->getSkill_cnt()+1);

	skill_sel = getSel(ply_lst[caster]->getSkill_cnt()+1);
	skill_sel--;

	if (skill_sel < ply_lst[caster]->getSkill_cnt())
	{
		int target;
		printf("Targets\n");
		printf("---------------------------\n");
		printPlayers();
		printf("\n%d. Cancel action...\n", num_plys+1);
		
		target = getSel(num_plys+1);
		if (target < num_plys+1)
		{
			target--;
			ply_lst[caster]->useSkill(skill_sel, ply_lst, num_plys, caster, target);
		}
		else
			printf("Cancelling action...\n");
	}
	else
		printf("Cancelling action...\n");
}
void party::useItem_OB()
{
	int item_sel;
	printInventory();
	printf("%d. Cancel action\n", inv_size+1);
	
	item_sel = getSel(inv_size+1);
	item_sel--;

	if (item_sel < inv_size)
	{
		int target;
		printf("Targets\n");
		printf("---------------------------\n");
		printPlayers();
		printf("\n%d. Cancel action...\n", num_plys+1);

		printf("Select target: ");
		target = getSel(num_plys+1);
		if (target < num_plys+1)
		{
			target--;
			useItem(item_sel, target);
		}
		else
			printf("Cancelling action...\n");
	}
	else
		printf("Cancelling action...\n");
}
void party::viewItemInfo_OB()
{
	int item_sel;
	printf("Select an item: \n");
	printInventory();
	printf("%d. Cancel action\n", inv_size+1);

	item_sel = getSel(inv_size+1);
	item_sel--;

	if (item_sel < inv_size)
	{
		viewItemInfo(item_sel);
	}
	else
		printf("Cancelling action...\n");
}
player* party::removePlayer_OB()
{
	if (num_plys <= 1)
	{
		printf("Party only has 1 member, you can not remove the only member of your party.\n");
		return NULL;
	}
	printf("Select the party member you wish to remove...\n");
	printPlayers();
	int p_idx = getSel(num_plys);
	p_idx--;
	printf("Remove %s from party?\n", ply_lst[p_idx]->getName());
	int confirm = getSel("1. Yes", "2. No");
	if (confirm == 1)
	{
		player *temp = removePlayer(p_idx);
		return temp;
	}
	else
	{
		printf("Cancelling action...\n");
		return NULL;
	}
}
void party::deletePlayer_OB()
{
	if (num_plys <= 1)
	{
		printf("Party only has 1 member, you can not delete the only member of your party.\n");
		return;
	}	
	printf("Select the party member you with to delete...\n");
	printPlayers();
	int p_idx = getSel(num_plys);
	p_idx--;
	printf("Delete %s?\n", ply_lst[p_idx]->getName());
	int confirm = getSel("1. Yes", "2. No");
	if (confirm == 1)
	{
		deletePlayer(p_idx);
	}
	else
	{
		printf("Cancelling action...\n");
	}
}
void party::partyMenu()
{
	bool partyMenu = true;
	while (partyMenu)
	{
		printf("Party Menu\n");
		printf("-------------------------------------\n");
		int man_sel = getSel("1. View Party Info", "2. Manage Inventory",
									"3. Change Party Order", "4. Remove Party Member",
									"5. Cancel");

		switch (man_sel)
		{
			case 1:
			{
				printPartyInfo();
				break;
			}	
			case 2:
			{
				manageInventory();
				break;
			}
			case 3:
			{
				switchPartyOrder();
				break;
			}
			case 4:
			{
				deletePlayer_OB();
				break;
			}
			case 5:
			{
				printf("Cancelling action...\n");
				partyMenu = false;
				break;
			}
		}
	}
}
void party::printPartyInfo()
{
	printf("Party Information\n");
	printf("---------------------------------\n");
	printf("Gold: %d\n\n", gold);
	
	printf("Name                            Class      Lvl   Health     MP\n");
	printf("==============================  =========  ====  =========  =========\n");
//	char *classes[3] = {"Warrior", "Defender", "Mage"};
	for (int i=0; i<num_plys; i++)
	{
		char LVL[32], HEALTH[32], MP[32];
		sprintf(LVL, "%d", ply_lst[i]->getLevel());
		sprintf(HEALTH, "%d/%d", ply_lst[i]->getHealth(), ply_lst[i]->getMax_Health_btl());
		sprintf(MP, "%d/%d", ply_lst[i]->getMP(), ply_lst[i]->getMax_MP_btl());

		printf("%-30s  %-9s  %-4s  %-9s  %-9s\n", ply_lst[i]->getName(), 
				 ply_lst[i]->getClassName(), LVL, HEALTH, MP);
	}
	printf("\n");
}
void party::switchPartyOrder()
{
	if (num_plys <= 1)
	{
		printf("There is only %d member in party, switch would have no effect\n", num_plys);
		return;
	}
	printf("Are you sure you want to change party order?\n");
	int sel = getSel("1. Yes", "2. No");
	if (sel == 2)
	{
		printf("Cancelling action...\n");
		return;
	}
	printf("Select the players whose order you want to switch:\n");
	printPlayers();
	printf("1st player:\n");
	int p1 = getSel(num_plys);
	p1--;
	printf("2nd player:\n");
	int p2 = getSel(num_plys);
	p2--;
	printf("\nSwitching %s and %s's party order...\n", ply_lst[p1]->getName(),
			 ply_lst[p2]->getName());

	player *temp = ply_lst[p1];
	ply_lst[p1] = ply_lst[p2];
	ply_lst[p2] = temp;

}
void party::saveParty()
{
	FILE *fp;

	printf("Saving party info...\n");	
	fp = fopen("./files/Character_save.txt", "w");
	assert(fp != NULL);
	for (int i=0; i<num_plys; i++)
	{
		ply_lst[i]->saveInfo(fp);
	}
	fclose(fp);

	inventoryFull();
	fp = fopen("./files/Inventory_save.txt", "w");	
	assert(fp != NULL);
	for (int i=0; i<inv_size; i++)
	{
		if (inventory[i] != NULL)
		{
			fprintf(fp, "%d %d \n", inventory[i]->getID(), inventory[i]->getStock());
		}
		else
			fprintf(fp, "0 \n");
	}
	fclose(fp);

	fp = fopen("./files/Party_save.txt", "w");
	assert(fp != NULL);
	fprintf(fp, "%d %d \n", gold, num_plys);
	fclose(fp);
}
void party::loadParty()
{
	FILE *fp;
	
	fp = fopen("./files/Party_save.txt", "r");
	assert(fp != NULL);
	fscanf(fp, " %d %d ", &gold, &num_plys);
	fclose(fp);

	fp = fopen("./files/Character_save.txt", "r");
	assert(fp != NULL);
	for (int i=0; i<num_plys; i++)
	{
		ply_lst[i] = new player;
		ply_lst[i]->setPlayer(fp);
	}
	fclose(fp);

	fp = fopen("./files/Inventory_save.txt", "r");
	assert(fp != NULL);
	for (int i=0; i<inv_size; i++)
	{
		int temp_id, temp_stock;
		fscanf(fp, " %d ", &temp_id);
		if (temp_id != 0)
		{
			fscanf(fp, " %d ", &temp_stock);
			inventory[i] = getItem(temp_id);
			inventory[i]->setStock(temp_stock);
		}
		else
			inventory[i] = NULL;
	}
	fclose(fp);
	inventoryFull();
}




