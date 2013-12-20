#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

class Player
{
	protected:
		char name[32];
		int health, Max_health;
	public:
		void set_vals(char*, int, int);
		void restore_Health(int);
		int get_health()
			{return health;}
		int get_Mhealth()
                        {return Max_health;}
		char* get_name()
			{return name;}
};
void Player::set_vals(char* NAME, int MH, int H)
{
	sprintf(name, "%s", NAME);
	Max_health = MH;
	if (H > Max_health)
                health = Max_health;
        else
                health = H;
}
void Player::restore_Health(int heal)
{
	int temp = health;
	health += heal;
	if (health > Max_health)
		health = Max_health;
	printf("%s's health restored by %d health pts.\n", name, health-temp);
	printf("%s's current health is %d/%d\n", name, health, Max_health);
}

class Enemy
{
	protected:
		char name[32];
		int health, Max_health;
		bool dead;
	public:
		void set_vals(char* NAME, int MH, int H);
		void take_damage(int);
		int get_health()
			{return health;}
		int get_Mhealth()
			{return Max_health;}
		char* get_name()
			{return name;}
};
void Enemy::set_vals(char* NAME, int MH, int H)
{
	sprintf(name, "%s", NAME);
        Max_health = MH;
	if (H > Max_health)
		health = Max_health;
	else
        	health = H;
	dead = false;
}
void Enemy::take_damage(int dam)
{
	int temp = health;
	health -= dam;	
	if (health <= 0)
	{
		health = 0;
		dead = true;
	}
	printf("%s took %d damage.\n", name, temp-health);
	printf("%s's current health is %d/%d\n", name, health, Max_health);
	if (dead)
		printf("%s is dead.\n", name);
}

class Item
{
	protected:
		char name[32];
		int stock, Max_stock, id;
		bool consum, equip, key;
	public:
		void set_vals(char*, int, int, int, bool, bool, bool);
		virtual void use(Player *players, Enemy *enemies, int target) =0;
		virtual void set_pts(int) =0;
};
void Item::set_vals(char* NA, int ST, int M_ST, int ID, bool CON, bool EQP, bool KEY)
{
	sprintf(name, "%s", NA);
        stock = ST; Max_stock = M_ST; id = ID;
        consum = CON; equip = EQP; key=KEY;
}

class healthPotion: public Item
{
	protected:
		int restore_pts;
	public:
		void use(Player *players, Enemy *enemies, int target);
		void set_pts(int);
};
void healthPotion::use(Player *players, Enemy *enemies, int target)
{
	if (stock > 0)
	{
		printf("Used %s\n", name);
		players[target].restore_Health(restore_pts);
		stock--;
	}
	else
		printf("%s out of stock, cannot use.\n", name);
	
}
void healthPotion::set_pts(int PTS)
{
	restore_pts = PTS;
//	printf("test\n");
}
class grenade: public Item
{
	protected:
		int damage_pts;
	public:
		void use(Player *players, Enemy *enemies, int target);
                void set_pts(int);
};
void grenade::use(Player *players, Enemy *enemies, int target)
{
	if (stock > 0)
	{
		printf("Used %s\n", name);
		enemies[target].take_damage(damage_pts);
		stock--;
	}
	else
		printf("%s out of stock, cannot use.\n", name);
}
void grenade::set_pts(int PTS)
{
	damage_pts = PTS;
}

int main()
{
	Player p1;
	Enemy e1;
	Item *item;
	Item **inv = new Item*[4];
	Item *INV[4];
	healthPotion test;

	p1.set_vals("Bobert", 5000, 300);
	e1.set_vals("Evil Bunny", 2500, 2500);
	printf("%s has %d/5000 health\n", p1.get_name(), p1.get_health());
	printf("%s has %d/5000 health\n", e1.get_name(), e1.get_health());

//	item = (healthPotion*)malloc(1*sizeof(healthPotion));
	item = new healthPotion;
	fprintf(stderr, "--1\n");
//	item = &test;

	item->set_vals("Potion", 1, 2, 3, false, false, false);	
	fprintf(stderr, "--2\n");

	for (int i=0; i<4; i++)
	{
		char temp[32];
		if ((i%2) == 0)
		{
			sprintf(temp, "Potion %d", i/2);
			inv[i] = new healthPotion;
			inv[i]->set_vals(temp, 1*(i+1), 2*(i+1), 3*(i+1),
					 false, false, false);
			inv[i]->set_pts((i+2)*50);
		}
		else	
		{
			sprintf(temp, "Grenade %d", i/2);
			inv[i] = new grenade;
			inv[i]->set_vals(temp, 1*(i+1), 2*(i+1), 3*(i+1),
                                         false, false, false);
			inv[i]->set_pts(((i/2)+1)*1000);
		}
		INV[i] = inv[i];
	}

//	test.set_pts(50);
	item->set_pts(50);
	fprintf(stderr, "--3\n");

	item->use(&p1, &e1, 0);
	fprintf(stderr, "--4\n");

	printf("%s has %d/500 health\n", p1.get_name(), p1.get_health());
	item->use(&p1, &e1, 0);	
	
	for (int i=0; i<4; i++)
	{
		inv[i]->use(&p1, &e1, 0);
		printf("\n");
		
//		delete inv[i];
	}
	//Want to replace potion 0 with a grenade item
	delete inv[0];
	inv[0] = new grenade;
	*(inv[0]) = *(inv[3]);//  ----This was dumb, but you might need it
	printf("\n\nI'm doing stuff\n");
//	inv[1] = inv[2];
	inv[0]->use(&p1, &e1, 0);

	INV[2]->use(&p1, &e1, 0);
//	delete[] inv;

	return 0;
}
