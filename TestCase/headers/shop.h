#pragma once
#include <stdio.h>
#include <stdlib.h>

class shop;
class item;
class party;

int getShopRange();
shop* getShop(int);

class shop
{
	int id;
	char name[32];
	int size;
	item **goods;
	double sell_rate;

	party *customer;
//	int type; Could be used to ensure that only certain items are processed

	public:
		shop();
		~shop();
		shop(int, char*, int*, int, double);

		void setShop(int, char*, int*, int, double);
		void loadShop(FILE*);
		void saveShop(FILE*);

		int getID()
			{return id;}
		char* getName()
			{return name;}
//		int getSize()
//			{return size;}
//		double getSellRate()
//			{return sell_rate;}	
		int getSellPrice(int);
		void printGoods();
		void enterShop(party*);
		void buyGoods();
		void sellGoods();
};
