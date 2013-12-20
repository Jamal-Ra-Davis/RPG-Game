#pragma once
#include <stdio.h>
#include <stdlib.h>

class Inventory;
class player;
class enemy;
class item;

class Inventory
{
	protected:
		int inv_size;
		item **inventory;	
	public:
		Inventory();
		Inventory(int);
		~Inventory();

		int getInventorySize()
			{return inv_size;}
		item** getInventory()
			{return inventory;}

		void printInventory();
		int printInventoryPage();
		bool inventoryFull();
		bool checkStock(int, int);
		bool checkItem(int);
		char* getItemName(int);
		int getItemValue(int);

		void recieveItem(item*);
		item* removeItem(int);
		item* removeOneItem(int);
		item* removeItems(int, int);
		void discardItem(int);
		item* swapItem(item*);

//		void useItem(int, int); Will have to pass player array into 
		void viewItemInfo(int); 
	
		void manageInventory(); 
	//		void useItem_OB();
			void viewItemInfo_OB();

		void saveInventory(FILE*);
		void loadInventory(FILE*);
};
