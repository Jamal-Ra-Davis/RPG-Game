#pragma once
#include <stdio.h>
#include <stdlib.h>

class item;
class player;
class enemy;
class skill;

void printInventory(item**, int);
void recieveItem(item**, int, item*);
void discardItem(item**, int, int);

item* getItem(int id);

class item
{
	protected:
		char name[32];
		int stock, Max_stock, id;
		int type, value;
	public:
		void set_vals(char* N, int S, int MS, int ID, int T, int V);
		char* getName()
			{return name;}
		int getStock()
			{return stock;}
		int getMax_Stock()
			{return Max_stock;}
		int getID()
			{return id;}
		int getType()
			{return type;}
		int getGoldValue()
			{return value;}

		void setStock(int);
		void decrementStock();
		void removeStock(int);
		virtual void use(player **ply_lst, int numP, int targ) =0;
		virtual void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
							  int targ) =0;
		virtual void printInfo() =0;
		//virtual void set_pts(int) =0;
};

class healthPotion: public item
{
	protected:
		int heal_pts;
	public:
		void use(player **ply_lst, int numP, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE, int targ);
		void printInfo();
		void set_pts(int);
};

class manaPotion: public item
{
	protected:
		int mana_pts;
	public:
		void use(player **ply_lst, int numP, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE, int targ);
		void printInfo();
		void set_pts(int);
};

class grenade: public item
{
	protected:
		int damage_pts;
	public:
		void use(player **ply_lst, int numP, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE, int targ);
		void printInfo();
		void set_pts(int);
};

class revive: public item
{
   protected:
      int revive_pts;
   public:
      void use(player **ply_lst, int numP, int targ);
      void use(player **ply_lst, int numP, enemy *enm_lst, int numE, int targ);
		void printInfo();
      void set_pts(int);
};

class equipment: public item
{
	protected:
		int equip_type;
		int health, mp, att, def, mag, mdef;
	public:
      void use(player **ply_lst, int numP, int targ);
      void use(player **ply_lst, int numP, enemy *enm_lst, int numE, int targ);
		void printInfo();
		void set_pts(int, int, int, int, int, int, int);		
		int getEquipType()
			{return equip_type;}
		int getHealth()
			{return health;}
		int getMP()
			{return mp;}
		int getAtt()
			{return att;}
		int getDef()
			{return def;}
		int getMag()
			{return mag;}
		int getMDef()
			{return mdef;}

};

class skillTeach: public item
{
	protected:
		int skill_id;
	public:
		void use(player **ply_lst, int numP, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE, int targ);
		void printInfo();
		void set_ID(int);
};
