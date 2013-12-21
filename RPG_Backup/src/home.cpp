#include "../headers/home.h"
#include "../headers/player.h"
#include "../headers/Inventory.h"
#include "../headers/party.h"
#include "../headers/Safe_Input.h"
#include "../headers/item.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

home::home()
{
	setHome();
}
home::home(int max_plys, int inv_size)
{
	setHome(max_plys, inv_size);
}
void home::setHome()
{
	num_plys = 0;
	Max_members = 4;
	ply_lst = new player*[Max_members];
	for (int i=0; i<Max_members; i++)
		ply_lst[i] = NULL;
	
	int inv_size = 100;
	storage = new Inventory(inv_size);
	player_party = NULL;
}
void home::setHome(int max_plys, int inv_size)
{
	num_plys = 0;
	Max_members = max_plys;
	ply_lst = new player*[Max_members];
	for (int i=0; i<Max_members; i++)
		ply_lst[i] = NULL;

	storage = new Inventory(inv_size);
	player_party = NULL;
}
home::~home()
{
	for (int i=0; i<Max_members; i++)
	{
		if (ply_lst[i] != NULL)
			delete ply_lst[i];
	}
	if (ply_lst != NULL)
		delete [] ply_lst;

	if (storage != NULL)
		delete storage;
}
player* home::recievePlayer(player *player_in)
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
void home::compressParty()
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
player* home::removePlayer(int idx)
{
	if ((idx < 0)||(idx >= num_plys))
	{  
		printf("Invalid selection...\n");
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
void home::deletePlayer(int idx)
{
	if ((idx < 0)||(idx >= num_plys))
	{  
		printf("Invalid selection...\n");
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
bool home::inventoryFull()
{
	return storage->inventoryFull();
}
bool home::partyFull()
{
	bool full = true;
	if (num_plys < Max_members)
		full = false;
	return full;
}
void home::printPlayers()
{
	if (num_plys == 0)
	{
		printf("There are no players at home...\n");
		return;
	}
	for (int i=0; i<num_plys; i++)
	{  
		printf("%d. %s\n", i+1, ply_lst[i]->getName());
	}
}
void home::printPlayersDetailed()
{
	if (num_plys == 0)
	{
		printf("There are no players at home...\n");
		return;
	}
	printf("Players currently residing at home. %d slots are available.\n\n", Max_members-num_plys);

	printf("Name                            Class      Lvl   Health     MP\n");
	printf("==============================  =========  ====  =========  =========\n");
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
void home::enterHome(party *in)
{
	player_party = in;
	bool in_home = true;

	while (in_home)
	{
		printf("Home\n");
		printf("-------------------------\n");
		int sel = getSel("1. Deposit Item", "2. Withdraw Item", 
							  "3. Deposit Player", "4. View Players in home",
							  "5. Withdraw Player", "6. Cancel");

		switch (sel)
		{
			case 1:
			{
				depositItem();
				break;
			}
			case 2:
			{
				withdrawItem();
				break;
			}
			case 3:
			{
				depositPlayer();
				break;
			}
			case 4:
			{
				printPlayersDetailed();
				break;
			}	
			case 5:
			{
				withdrawPlayer();
				break;
			}
			case 6:
			{
				printf("Cancelling action...\n");
				in_home = false;
				break;
			}
		}
	}
	player_party = NULL;	
}
void home::depositItem()
{
	player_party->printInventory();
	printf("\n%d. Cancel\n", player_party->getInventorySize()+1);

	printf("Select Item to store: ");
	int dep_sel = getSel(player_party->getInventorySize()+1);
	
	if (dep_sel < player_party->getInventorySize()+1)
	{
		if (inventoryFull())
		{
			printf("Home inventory is full, cannot store item...\n");
			return;
		}
		dep_sel--;
		item *temp = player_party->removeItem(dep_sel);
		if (temp != NULL)
		{
			printf("Deposit %s?\n", temp->getName());
			int confirm = getSel("1. Yes", "2. No");
		
			if (confirm == 1)
			{
				printf("Moving %s to home storage...\n", temp->getName());
				storage->recieveItem(temp);
			}
			else
			{
				printf("Cancelling action...\n");
				player_party->recieveItem(temp);
			}
		}
		else
		{
			//Do stuff when you select no item
		}
	}
	else
	{
		printf("Cancelling action...\n");
	}
}
void home::withdrawItem()
{
	int val = storage->printInventoryPage();
	printf("\n%d. Cancel\n", val+1);

	int sel = getInt();
	while (1)
	{
		if ((sel <= val+1)||(sel > val-10))
			break;
		else
		{
			printf("Invalid entry...\n");
			sel = getInt();
		}	
	}
	
	if (sel < val+1)
	{
		if (player_party->inventoryFull())
		{
			printf("Party inventory is full, cannot store item...\n");
			return;	
		}
		sel--;
		item *temp = storage->removeItem(sel);	
		if (temp != NULL)
		{
			printf("Withdraw %s?\n", temp->getName());
			int confirm = getSel("1. Yes", "2. No");
		
			if (confirm == 1)
			{
				printf("Moving %s to party inventory...\n", temp->getName());
				player_party->recieveItem(temp);
			}
			else
			{
				printf("Cancelling action...\n");
				storage->recieveItem(temp);
			}
		}
		else
		{
			//Do stuff when you select no item
		}
	}
	else
	{
		printf("Cancelling action...\n");
	}
	
}
void home::depositPlayer()
{
	player_party->printPlayers();
   printf("\n%d. Cancel\n", player_party->getPartySize()+1);

	printf("Select player to store: ");
   int dep_sel = getSel(player_party->getPartySize()+1);
	if (dep_sel < player_party->getPartySize()+1)
	{
		dep_sel--;
		player *temp = player_party->removePlayer(dep_sel);
		if (temp != NULL)
		{
			printf("# of current residents: %d. Max is %d.\n", num_plys, Max_members);
			printf("Do you want %s to stay at home?\n", temp->getName());
			int confirm = getSel("1. Yes", "2. No");
			if (confirm == 1)
			{
				player *temp2 = recievePlayer(temp);
				if (temp2 == NULL)
				{
					printf("%s is now living at home...\n", temp->getName());
				}
				else
				{
					printf("Not enough space, %s rejoining party...\n", temp->getName());
					player_party->recievePlayer(temp);
				}
			}
			else
			{
				printf("Cancelling action...\n");
				player_party->recievePlayer(temp);
			}
		}
		else
		{
			//Do stuff when you don't select player
		}
	}
	else
	{
		printf("Cancelling action...\n");
	}
}
void home::withdrawPlayer()
{
	if (num_plys == 0)
	{
		printf("There are no players at home to withdraw...\n");
      return;
	}
	if (player_party->partyFull())
	{
		printf("Player party full, cannot withdraw player...\n");
		return;
	}
	printPlayers();
	printf("\n%d. Cancel\n", num_plys+1);

	printf("Select player to withdraw: ");
	int with_sel = getSel(num_plys+1);	
	if (with_sel < num_plys+1)
	{
		with_sel--;
		player *temp = removePlayer(with_sel);
		if (temp != NULL)
		{
			printf("Do you want %s to comeback from home, and join party?\n", temp->getName());
			int confirm = getSel("1. Yes", "2. No");
			if (confirm == 1)
			{
				player *temp2 = player_party->recievePlayer(temp);
				if (temp2 == NULL)
				{
					printf("%s joined your party...\n", temp->getName());
				}
				else
				{
					printf("Something bad happened, not sure what, but you probably don't have enough space\n");
				}
			}
			else
			{
				printf("Cancelling action...\n");
			}
		}
		else
		{
			//Do stuff when you don't select player
		}
	}
	else
	{
		printf("Cancelling action...\n");
	}
}
void home::saveHome()
{
	//Save num_plys to home file
	FILE *fp;

	fp = fopen("./files/Home_save.txt", "w");
	assert(fp != NULL);
	fprintf(fp, "%d", num_plys);
	fclose(fp);

	//make file ptr for home inventory, pass it to Storage to have it save
	fp = fopen("./files/HomeInv_save.txt", "w");
	assert(fp != NULL);
	storage->saveInventory(fp);
	fclose(fp);
	//make file ptr for players in home, for loop through them to make each player save to file
	fp = fopen("./files/HomePlys_save.txt", "w");
	assert(fp != NULL);
	for (int i=0; i<num_plys; i++)
	{
		ply_lst[i]->saveInfo(fp);
	}
	fclose(fp);
}
void home::loadHome()
{
	//load
	FILE *fp;
	
	fp = fopen("./files/Home_save.txt", "r");
	assert(fp != NULL);
	fscanf(fp, " %d ", &num_plys);
	fclose(fp);

	fp = fopen("./files/HomeInv_save.txt", "r");
	assert(fp != NULL);
	storage->loadInventory(fp);
	fclose(fp);

	//do stuff to load in players
	fp = fopen("./files/HomePlys_save.txt", "r");
	assert(fp != NULL);
	for (int i=0; i<num_plys; i++)
	{
		ply_lst[i] = new player;
		ply_lst[i]->setPlayer(fp);
	}
	fclose(fp);
}







