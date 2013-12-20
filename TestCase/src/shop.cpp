#include "../headers/shop.h"
#include "../headers/Safe_Input.h"
#include "../headers/item.h"
#include "../headers/party.h"
#include "../headers/FileFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int getShopRange()
{
	int cnt = lineCount("./files/Shop_List.txt");
	return cnt;
}

shop* getShop(int ID)
{
	int cnt = lineCount("./files/Shop_List.txt");

	if ((ID > cnt)||(ID == 0))
		return NULL;	

	shop *temp = NULL;
	char *line_in;

	int id;
	char name[32];
	int size;
	int *item_ids;
	double sell_rate;

	line_in = getWholeLine("./files/Shop_List.txt", ID);
	printf("Line in: %s\n", line_in);
	FILE *fp = fopen("./files/tempShop.txt", "w");
	assert(fp != NULL);
	fprintf(fp, "%s \n", line_in);
	fclose(fp);
	free(line_in);

	fp = fopen("./files/tempShop.txt", "r");
	assert(fp != NULL);
	temp = new shop;
	temp->loadShop(fp);
	fclose(fp);

	return temp;	
}

//-------------------------------------------------

shop::shop()
{
	sprintf(name, "Default Shop");
	int size = 0;
	goods = NULL;
	sell_rate = 1.0;
	customer = NULL;
}
shop::~shop()
{
	for (int i=0; i<size; i++)
	{
		delete goods[i];
	}
	if (goods != NULL)
		delete [] goods;
}
shop::shop(int ID, char* NAME, int* IDs, int SIZE, double RATE)
{
	setShop(ID, NAME, IDs, SIZE, RATE);
}
void shop::setShop(int ID, char* NAME, int* IDs, int SIZE, double RATE)
{
	customer = NULL;
	id = ID;
	sprintf(name, "%s", NAME);
	size = SIZE;
	sell_rate = RATE;

	goods = new item*[size];
	for (int i=0; i < size; i++)
	{
		goods[i] = NULL;
		goods[i] = getItem(IDs[i]);
	}
}
void shop::loadShop(FILE *fp)
{
	customer = NULL;
	
	fscanf(fp, " %d '%[^']' %lf %d ", &id, name, &sell_rate, &size);
	goods = new item*[size];
	for (int i=0; i < size; i++)
	{
		int temp;
		fscanf(fp, " %d ", &temp);
		goods[i] = NULL;
		goods[i] = getItem(temp);
	}
}	
void shop::saveShop(FILE *fp)
{
	fprintf(fp, "%d '%s' %lf %d ", id, name, sell_rate, size);
	for (int i=0; i<size; i++)
	{
		fprintf(fp, "%d ", goods[i]->getID());	
	}
	fprintf(fp, "\n");
}
int shop::getSellPrice(int value)
{
/*
	if ((idx < 0)||(idx >= size))
	{
		printf("Invalid index...\n");
		return -1;
	}
*/
//	double price_d = sell_rate*goods[idx]->getGoldValue();
	double price_d = sell_rate*value;
	return (int)price_d;
}
void shop::printGoods()
{
	printf("%s's Selection\n", name);
	printf("--------------------------------------------\n");
	printf("ID   Name                  Price\n");
	printf("===  ====================  =========\n");
	for (int i=0; i<size; i++)
	{
//		char temp[32];
//		sprintf("");
		printf("%2d.  %-20s  %-5d gld\n", i+1, goods[i]->getName(), goods[i]->getGoldValue());
	}	
	printf("\n");
}
void shop::enterShop(party* in)
{
	customer = in;
	bool in_shop = true;
	while (in_shop)
	{
		printf("%s\n", name);
		printf("---------------------\n");
		int shop_sel = getSel("1. Buy", "2. Sell", "3. Leave Shop");
		switch (shop_sel)
		{
			case 1:
			{
				buyGoods();
				break;
			}
			case 2:
			{
				sellGoods();
				break;
			}
			case 3:
			{
				printf("Come again soon...\n");
				in_shop = false;
				customer = NULL;
				break;
			}
		}
	}
}
void shop::buyGoods()
{
	bool buyGoods = true;
	while (buyGoods)
	{
		printf("Current gold: %d\n\n", customer->getGold());
		printGoods();
		printf("%d. Cancel Action\n", size+1);
		int buy_sel = getSel(size+1);
		if (buy_sel < size+1)
		{
			buy_sel--;
			printf("How many %ss do you want to buy: ", goods[buy_sel]->getName());
			int count = getInt();
			if (count > 0)
			{
				int price = count*goods[buy_sel]->getGoldValue();
				printf("Buying %d %ss will cost %d gold. Is this ok?\n", count, goods[buy_sel]->getName(), 
						 price);
				int confirm = getSel("1. Yes", "2. No");
				if (confirm == 1)
				{
					if (customer->goldCheck(price))
					{
						item *purchase = getItem(goods[buy_sel]->getID());
						purchase->setStock(count);
					
						customer->recieveItem(purchase);
						customer->loseGold(price);
						printf("Purchase complete, you have %d gold left\n", customer->getGold());
					}
					else
					{
						printf("You do not have enough gold to make this purchase.\n");
						printf("Cancelling transaction...\n");
					}
/*
					printf("Would you like to keep shopping?\n");
					int cont_sel = getSel("1. Yes", "2. No");
            	if (cont_sel == 2)
            	{
               	printf("Exiting buy menu...\n");
               	buyGoods = false;
            	}	 
					else
					{
						printf("Take your time...\n");
					}
*/
				}
				else
				{
					printf("Cancelling transaction...\n");
				}
				printf("Would you like to keep shopping?\n");
				int cont_sel = getSel("1. Yes", "2. No");
				if (cont_sel == 2)
				{
					printf("Exiting buy menu...\n");
					buyGoods = false;
				}
				else
				{
					printf("Take your time...\n");
				}
			}
			else
			{
				printf("Sorry, you must select at least 1 item to buy\n\n");
			}		
		}
		else
		{
			printf("Cancelling action...\n");
			buyGoods = false;
		}	
	}	
}
void shop::sellGoods()
{
	bool sellGoods = true;
	while (sellGoods)
	{
		printf("Current gold: %d\n\n", customer->getGold());
		customer->printInventory();
		printf("%d. Cancel\n", customer->getInventorySize()+1);
		int sell_sel = getSel(customer->getInventorySize()+1);
		if (sell_sel < customer->getInventorySize()+1)
		{
			sell_sel--;
			if (customer->checkItem(sell_sel))
			{
				printf("How many %s do you want to sell: ", customer->getItemName(sell_sel));
				int count = getInt();
				int price = count*getSellPrice(customer->getItemValue(sell_sel));
				printf("Do you want to sell %d %ss for a total of %d gold?\n", count, 
						 customer->getItemName(sell_sel), price);
				int confirm = getSel("1. Yes", "2. No");
				if (confirm == 1)
				{
					if (customer->checkStock(sell_sel, count))
					{
						item *temp = customer->removeItems(sell_sel, count);	
						customer->recieveGold(price);
						delete temp;
						printf("Sale complete, you now have %d gold.\n", customer->getGold());
					}
					else
					{
						printf("You do not have enough %ss to make this sale.\n",
								 customer->getItemName(sell_sel));
						printf("Cancelling transaction...\n");
					}
				}
				else
				{
					printf("Cancelling transaction...\n");
				}
			}
			else
			{
				printf("You did not select anything to sell...\n");
				printf("Cancelling transaction...\n");
			}

			printf("Would you like to sell more items?\n");
         int cont_sel = getSel("1. Yes", "2. No");
         if (cont_sel == 2)
         {
            printf("Exiting sell menu...\n");
            sellGoods = false;
         }
         else
         {
            printf("Take your time...\n");
         }
		}
		else
		{
			printf("Cancelling action...\n");
			sellGoods = false;
		}
	}
}













