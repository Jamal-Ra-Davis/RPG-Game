#include "../headers/Inventory.h"
//#include "../headers/player.h"
//#include "../headers/skill.h"
#include "../headers/item.h"
#include "../headers/Safe_Input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <assert.h>
using namespace std;


Inventory::Inventory()
{
	inv_size = 0;
	inventory = NULL;
}
Inventory::Inventory(int A)
{
	inv_size = A;
	inventory = new item*[inv_size];
	for (int i=0; i<inv_size; i++)
		inventory[i] = NULL;
}
Inventory::~Inventory()
{
	for (int i=0; i<inv_size; i++)
	{
		if (inventory[i] != NULL)
			delete inventory[i];
	}

	if (inventory != NULL)
		delete [] inventory;
}
void Inventory::printInventory()
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
int Inventory::printInventoryPage()
{
//	int pages = inv_size/10 + 1;
	int pages = (inv_size-1)/10 + 1;
	printf("Select an inventory page to view (1-%d): ", pages);
	int page_sel = getSel(pages);

	printf("Inventory\n");
	printf("-----------------------------\n");
	printf("ID    Name                  Stock   Equip\n");
	printf("====  ====================  ======  =======\n");
	
	int start = (page_sel-1)*10;
	int end = start+10;
	if (end > inv_size)
		end = inv_size;
	for (int k=start; k<end; k++)
	{
		if (inventory[k] != NULL)
		{
			printf("%3d.  %-20s  x%-5d  ", k+1, inventory[k]->getName(), inventory[k]->getStock());
			if (inventory[k]->getType() == 4)
				printf("Yes\n");
			else
				printf("No\n");
		}
		else
			printf("%3d.  %-20s  %-6s  N/A\n", k+1, "<Empty Slot>", "N/A");
	}
	printf("\n");
	return (end);
}
bool Inventory::inventoryFull()
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
bool Inventory::checkStock(int idx, int num)
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
bool Inventory::checkItem(int idx)
{
	bool valid_item = true;
	if ((idx < 0)||(idx >= inv_size))
		valid_item = false;
	if (inventory[idx] == NULL)
		valid_item = false;

	return valid_item;
}
char* Inventory::getItemName(int idx)
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
int Inventory::getItemValue(int idx)
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
void Inventory::recieveItem(item *loot)
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
item* Inventory::removeItem(int idx)
{  
	if ((idx < 0)||(idx >= inv_size))
	{  
		printf("Invalid selection\n");
		return NULL;
	}
	if (inventory[idx] != NULL)
	{
		printf("Removing %s...\n", inventory[idx]->getName());
		item *temp = inventory[idx];
		inventory[idx] = NULL;
		return temp;
	}
	else
	{
		printf("You did not select anything to remove\n");
		return NULL;
	}
}
item* Inventory::removeOneItem(int idx)
{  
	if ((idx < 0)||(idx >= inv_size))
	{
		printf("Invalid selection\n");
		return NULL;
	}
	if (inventory[idx] != NULL)
	{
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
	}
	else
	{
		printf("You did not select anything to remove\n");
		return NULL;
	}
}
item* Inventory::removeItems(int idx, int num)
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
void Inventory::discardItem(int idx)
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
item* Inventory::swapItem(item *loot)
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
void Inventory::viewItemInfo(int itm_idx)
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
void Inventory::manageInventory()
{
	bool manageInventory = true;
	while(manageInventory)
	{
		printf("Inventory Management\n");
		printf("-------------------------------------\n");
		int man_sel = getSel("1. View Inventory",	"2. View Item Description", 
									"3. Discard Item", "4. Cancel");

		switch (man_sel)
		{
			case 1://View Inventory
			{
				printInventory();
				break;
			}
//			case 2://Use Item
//			{
//				useItem_OB();//Not a working function in Inventory class
//				break;
//			}
			case 2:
			{
				viewItemInfo_OB();
				break;
			}
			case 3://Discard Item
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
				}
				break;
			}
			case 4://Cancel
			{
				printf("Cancelling action...\n");
				manageInventory = false;
				break;
			}
		}
	}
}
void Inventory::viewItemInfo_OB()
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
void Inventory::saveInventory(FILE *fp)
{
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
}
void Inventory::loadInventory(FILE *fp)
{
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
}


