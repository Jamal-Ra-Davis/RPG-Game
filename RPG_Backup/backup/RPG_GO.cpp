#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;
#define INV_NUM 15 //Number of inventory slots

void header (char*);

class players 
{
	char name[32];
	int att, def, mgk, mdef;
	int health, max_health;
	int mana, max_mana;
	public:
//		players();
		void set_name();
//		void choose_class(int);
//		void reset_stats;		
};
void players::set_name()
{
	printf("Enter your hero's name: ");
	//getline(cin, name, '\n');
	scanf("%s", name);
	printf("\nHero's name is %s.\n", name);
	//cout << "\nHero's name is " << name << "." << endl;
}

class Item 
{
public:
	char name[32];
	int stock, MAX_stock, id;
	bool consum, equip, key;
//remember to uncomment this public, get rid of the upper one, and make functions to return all the indivdual item values
//	public:
		Item();
		Item(char*, int, int, int, bool, bool, bool);
		void set (char*, int, int, int, bool, bool, bool);
		void Use();		
};
Item::Item()
{
	sprintf(name, "%s", "NULL");
	stock = 0;MAX_stock = 0;id = -1;
	consum = false;equip = false;key=false;
}
/*Item::Item (string NA, int ST, int M_ST, int ID, bool CON, bool EQP, bool KEY) {
	name = NA;stock = ST;MAX_stock = M_ST;id = ID;
        consum = CON;equip = EQP;key=KEY;
} */
void Item::set (char* NA, int ST, int M_ST, int ID, bool CON, bool EQP, bool KEY) 
{
	sprintf(name, "%s", NA);
	stock = ST;MAX_stock = M_ST;id = ID;
        consum = CON;equip = EQP;key=KEY;
} 
void Item::Use() 
{
	if (id == -1)
		printf("CANNOT USE\n");
	else
		{
		stock = stock - 1;
		if (stock < 0)
			{
			stock = 0;
			printf("Out of %s, cannot use.\n", name);
			}
		}
}

/*class Shop {
	int num_inv;
	Item shop_inv[num_inv];
	int cost[num_inv];	
};*/

class Game_sys 
{
	bool Game_win;
	Item Inventory[INV_NUM];
	int gold;
	bool Quest_Prog[10];
	public:
		Game_sys ();
		Game_sys (bool);
		int Win_Cond();
		bool gold_compare (int);
		void gold_add (int);
		void gold_sub (int);
		void gold_disp();
		void inv_add(Item,int);
		void inv_disp();
		void inv_man();
		void inv_disc();
		void Quest_Prog_disp();

};
Game_sys::Game_sys()
{
	Game_win = false;
//	for (int i=0;i<INV_NUM;i++)
//		Inventory[i];
	gold = 300; //Will probably end up changing later, or may be arbitray at start and be reinitilzed upon new or load game
	for (int j=0;j<10;j++)
		Quest_Prog[j]=false;
}
Game_sys::Game_sys (bool a)
{
	Game_win = a;
}
int Game_sys::Win_Cond()
{
	return Game_win;
}
bool Game_sys::gold_compare (int cost)
{
	bool afford;
	if (cost > gold)
		afford = false;
		
	else 
		afford = true;

	return afford;		
}
void Game_sys::gold_add (int gold_get)
{
	gold = gold + gold_get;
	printf("You now have %d gold\n", gold);
}
void Game_sys::gold_sub (int cost)
{
	gold = gold - cost;
	if (gold < 0)
	{
		gold = 0;
		printf("You're totally out of money brah. This really shouldn't happen, this is an error i guess\n");
	}
	printf("You have %d gold left\n", gold);
}
void Game_sys::gold_disp()
{
	printf("You have %d gold\n",gold);
}
void Game_sys::inv_add(Item thing, int amt)
{
	bool loop = true, GET=true;
	if (loop == true)
	{
		for (int i=0; i<INV_NUM; i++)
		{
			if (Inventory[i].id == thing.id)
			{
				Inventory[i].stock = Inventory[i].stock + (amt*thing.stock);
				loop = false;
				printf("Aquired %d %s.\n", (amt * thing.stock), thing.name);
//				cout << "Aquired " << (amt * thing.stock) << " " << thing.name << "." << endl;
				if (Inventory[i].stock > Inventory[i].MAX_stock)
				{
					Inventory[i].stock = Inventory[i].MAX_stock;
					printf("You have reached the maximum number of %s.\n", Inventory[i].name);
//					cout << "You have reached the maximum number of " << Inventory[i].name << "." <<endl;
				}
				printf("Current %s stock is %d.\n", Inventory[i].name, Inventory[i].stock); 
//				cout << "Current " << Inventory[i].name << " stock is " << Inventory[i].stock << "." << endl; 
			}
		}
	}
	if (loop == true)
	{
		for (int i=0; i<INV_NUM; i++)
		{
			if ((Inventory[i].id == -1)&&(GET == true))
			{
				Inventory[i] = thing;
				Inventory[i].stock = (thing.stock * amt);
				GET = false;
				loop = false;
				printf("Aquired %d %s.\n", (amt * thing.stock), thing.name);
//                                cout << "Aquired " << (amt * thing.stock) << " " << thing.name << "." << endl;
				printf("Current %s stock is %d.\n", Inventory[i].name, Inventory[i].stock);
//                                cout << "Current " << Inventory[i].name << " stock is " << Inventory[i].stock << "." << endl;
			}
		}
	}
	if (loop == true)
	{
//		cout << "No more inventory space. " << thing.name << "was discarded." << endl;
		printf("No more inventory space. %s was discarded\n", thing.name);
	}	
}
void Game_sys::inv_disp()
{
	header("Inventory");
	for (int i=0; i<INV_NUM; i++)
//		cout << i+1 << ". " << Inventory[i].name << "                x" << Inventory[i].stock << endl;
		printf("%-2.2d. %-20s  x%d\n", i+1, Inventory[i].name, Inventory[i].stock);
}
void Game_sys::inv_man()
{
	Item temp;
	header("Invetory Managament");
	int confirm_sel;
	printf("Swap items?\n1. Yes\n2. No\n");
	scanf("%d", &confirm_sel);
	switch (confirm_sel)
	{
		case 1:
		{
			inv_disp();
			int swap1_sel;
			int swap2_sel;
			printf("You will pick two items in inventory to have their places swapped\n");
			printf("Select 1st item: ");
			scanf("%d", &swap1_sel);
			swap1_sel = swap1_sel - 1;
			printf("Select 2nd item: ");
			scanf("%d", &swap2_sel);
			swap2_sel = swap2_sel - 1;
			int swap_confirm;
			cout << "Swap " << Inventory[swap1_sel].name << " with " << Inventory[swap2_sel].name << "?\n1. Yes\n2. No" << endl;
			scanf("%d", &swap_confirm);
			switch (swap_confirm)
			{
				case 1:
				{
					//cout << "Swapping " << Inventory[swap1_sel].name << " with " << Inventory[swap2_sel].name << "..." << endl;
					printf("Swapping %s with %s...\n", Inventory[swap1_sel].name, Inventory[swap2_sel].name);
					temp = Inventory[swap1_sel];
					Inventory[swap1_sel] = Inventory[swap2_sel];
					Inventory[swap2_sel] = temp;
					break;
				}
				case 2:
				{
					printf("Cancelling action...\n");
					break;
				}
			} 
			break;
		}
		case 2:
		{
			printf("Exiting Inventory Management...\n");
			break;
		}
	} 
}
void Game_sys::inv_disc()
{
        Item discard;
        header("Discard Item");
        int confirm_sel;
        printf("Do you want to discard an item?\n1. Yes\n2. No\n");
        scanf("%d", &confirm_sel);
        switch (confirm_sel)
        {
                case 1:
                {
                        inv_disp();
                        int disc_sel;
                        //int swap2_sel;
                        printf("Select the item you wish to discard: ");
                        //printf("Select 1st item: ");
                        scanf("%d", &disc_sel);
                        disc_sel = disc_sel - 1;
                        //printf("Select 2nd item: ");
                        //scanf("%d", &swap2_sel);
                        //swap2_sel = swap2_sel - 1;
                        int disc_confirm;
                        //cout << "This will discard all of the " << Inventory[disc_sel].name << " that you have in your inventroy. Is this ok?\n1. Yes\n2. No" << endl;
			printf("This will discard all of the %s that ypu have in your inventroy. Is this ok?\n1. Yes\n2. No\n", Inventory[disc_sel].name);
                        scanf("%d", &disc_confirm);
                        switch (disc_confirm)
                        {
                                case 1:
                                {
                                        //cout << "Discarding " << Inventory[disc_sel].name << " from inventroy..." << endl;
					printf("Discarding %s from inventory...\n", Inventory[disc_sel].name);
                                        //temp = Inventory[swap1_sel];
                                        Inventory[disc_sel] = discard;
                                        //Inventory[swap2_sel] = temp;
                                        break;
                                }
                                case 2:
                                {
                                        printf("Cancelling action...\n");
                                        break;
                                }
                        }
                        break;
                }
                case 2:
                {
                        printf("Exiting Inventory Management...\n");
                        break;
                }
        }
}
void Game_sys::Quest_Prog_disp()
{
	header("Quest Progress");
	for (int i=0; i<10; i++)
	{
		printf("%d. Quest %d status: ",i+1,i+1);
		if (Quest_Prog[i])
			printf("Quest Completed\n");
		else if (Quest_Prog[i] == false)
			printf("Uncomplete\n");
	}
}

void header (char* title)
{
	printf("%s\n", title);
	printf("----------------------------\n");
}

Game_sys RPG_GO; // ---------------------This probably shouldn't be here---------------------

void Wep_Shop()
{
Item wep_shop[3];
/*sets what the shop has in it's inventory, in this case 3 items
	-(1)Sets the name of the item, (2)the stock that you would get upon buying one, (3)the maximum stock that can be held, 
	 (4)the ID number for that item, (5)determines if the item is a consumable, (6) determines if item can be equipped,
	 (7)determines if the item is a key item.

	-Ex: wep_shop[NUM].set("name", stock, max_stock, ID, consumable, equip, key);  
*/
wep_shop[0].set("Assy Sword", 1, 99, 6, false, true, false);
wep_shop[1].set("Ok Sword", 1, 99, 7, false, true, false);
wep_shop[2].set("Brotabulus Sword", 1, 99, 8, false, true, false);

//Seperate array for the costs of each item, at this particualar shop
int cost[3];
cost[0]=50;
cost[1]=80;
cost[2]=150;
bool shopping=true;

while (shopping == true)
{
	RPG_GO.inv_disp();
	header("Weapons Shop");
	for (int i=0;i<3;i++)
		//cout << i+1 << "." << wep_shop[i].name << "            COST: " << cost[i] << endl;
		printf("%-2.2d. %-20s COST: %d\n", i+1, wep_shop[i].name, cost[i]);
	printf("\n");
	RPG_GO.gold_disp(); //Displays your current gold amount
	printf("What do you want to buy?\n");
	int sel;
	scanf("%d", &sel);// If you type a number (< 1) or (> 15), throw error and send back to selection
	sel = sel - 1;
	//cout << "How many " << wep_shop[sel].name <<"s?"<< endl;
	printf("How many %ss?\n", wep_shop[sel].name);
	int amount;
	scanf("%d", &amount);
	//cout << "You are buying " <<amount<< " " <<wep_shop[sel].name<< ". Total is " <<amount*cost[sel]<<"g. Is this ok?" << endl;
	printf("You are buying %d %s. Total is %dg. Is this ok?\n", amount, wep_shop[sel].name, amount*cost[sel]);
	int confirm;
	printf("1. Yes\n2. No\n");
	scanf("%d", &confirm);
	switch (confirm)
	{
		case 1:
		{
			if (RPG_GO.gold_compare(amount*cost[sel]))
			{
				//call functions to put items you bought in inventory, and subtract total from gold supply
				//maybe could return a bool that tells you if you reached inventory limit
				RPG_GO.inv_add(wep_shop[sel], amount);
				RPG_GO.gold_sub(amount*cost[sel]);
			}
			else
				printf("You do not have enough gold to make this purchase\n");
			break;
		}
		case 2:
		{
			printf("Cancelling transaction...\n");		
			break;
		}
	}
	int exit;
	printf("Keep shopping?\n1. Yes\n2. No\n");
	scanf("%d", &exit);
	switch (exit)
	{
		case 1:
		{
			printf("Ok, take your time\n");
			break;
		}
		case 2:
		{
			printf("Thank you, come again soon\n");
			shopping = false;
			break;
		}
	}
}
}

int main()
{
//Declare the Game class
//		Game_sys RPG_GO; Remember to uncomment this, temporarily moving to line 102
//Title
printf("-------RPG GO!!!-------\n\n\n");

//Intro stuff: class and player stats and name and junk
players player[2];
int game_sel;
bool new_load = true;
while (new_load == true)
{
	printf("1. New Game\n2. Load Game\n");
	scanf("%d", &game_sel);
	switch (game_sel)
	{
		case 1:
		{
			//Reset and Initialize player data
			bool class_choose = true;
			while (class_choose == true)
			{
				printf("Name your hero:\n");
				player[0].set_name();
				int class_sel;
				printf("Choose your class:\n");
				printf("1. Warrior - Has higher attack, but lacks defense and magic stats\n");
				printf("2. Defender - Has higher defense, but lacks attack and magic stats\n");
				printf("3. Mage - Has higher magic abilites, but lacks attack and defense stats\n");
				scanf("%d", &class_sel);
				switch (class_sel)
				{
					case 1:
						//Sets base player stats to the warrior's
						printf("You have chosen the warrior class\n");
						break;
					case 2:
						//Sets base player stats to the warrior's
						printf("You have chosen the defender class\n");
						break;
					case 3:
						//Sets base player stats to the warrior's
						printf("You have chosen the mage class\n");
						break;
				}
				//Print out player data for confirmation, ask if the information is correct
				header("Is player information correct?");
				printf("1. Yes\n2. No\n");
				int confirm;
				scanf("%d", &confirm);
				if (confirm == 1)
					class_choose = false; 	
			}
			new_load = false;
			break;
		}
		case 2:
		{
			bool load_success = false;
			//Loads player and game data, if load is unsuccessful, throw error and do not exit loop
			printf("Loading game...\n");
			if (load_success == true)
				new_load = false;
			else if (load_success == false)
				printf("Load was unsuccessful\n");
			break;
		}
	}
}


//main gameloop
//may want to move game_running boolean to Game_sys class. If not you might have to pass it into any battle functions that the player could lose.
bool game_running = true;
while (game_running == true)
{
	//Hub town
	int hub_sel;
	header("Main Hub");
	printf("1. Free Exploration\n2. Item/Weapon Shop\n3. Quest Hall\n");
	printf("4. Inn\n5. Death Dungeon\n6. Open Menu\n");
	scanf("%d", &hub_sel);
	 
	switch (hub_sel)
	{
		case 1:
			/*Return to already explored areas:
				Search for loot and fight monster for exp*/
			//Area and battle classes work together to do stuff
			/*calls function that displays what areas are available
				-gets info from the areas class
				-if an area has already been traveled to, it should be available
			*/
			printf("Phasers are now in the 'fun' position\n");
			break;	
		case 2:
		{
			//Enter to do shopping and junk
			bool shop_enter = true;
			while (shop_enter == true)
			{
				int shop_sel;
				header("Weapon/Item Shop");
				printf("1. Buy Weapons/Armor\n2. Buy Items\n");
				printf("3. Exit Shop\n");
				scanf("%d", &shop_sel);
				switch (shop_sel)
				{
					case 1:
						//Buy Weapons or Armor
						printf("What weapons you want?\n");
						Wep_Shop();
						break;
					case 2:
						//Buy items
						printf("What items you want?\n");
						break;
					case 3:
						//Exits the shop
						printf("Thank you for coming to the shop.\n");
						shop_enter = false;
						break;
				}
			}
			break;
		}
		//	break;
			 
		case 3:
		{	
			//Enter to take on new quests
			bool qHall_enter = true;
			while (qHall_enter == true)
			{
				int qHall_sel;
				header("Quest Hall");
				printf("1. Do things\n2. Exit Hall\n");
				scanf("%d", &qHall_sel);
				switch (qHall_sel)
				{
					case 1:
						/* View which quests are available, check quest
						requirements, accept and go on quests */
						printf("What quests you want\n");
						break;
					case 2:
						//Exits the hall
						printf("Leaving so soon?\n");
						qHall_enter = false;
						break;
				}
			}
			break;
		}
		//	break;
		case 4:
		{
			bool inn_enter = true;
			while (inn_enter == true)
			{
				int inn_sel;
				header("Inn");
				printf("1. Stay the night\n2. Exit\n");
				scanf("%d", &inn_sel);
				switch (inn_sel)
				{
					case 1:
						//Restores health and mana back to full
						//Costs a certain gold fee
						//Have a confirmation option so, you don't accidentally spent gold
						printf("Health and mana have been restored\n");
						break;
					case 2:
						//Exits the inn
						printf("Why you no wanna to stay at my inn?! We have'a the sheets made from the goblin hide, is very nice.\n");
						inn_enter = false;
						break;
				}
			}
			break;
		}
		//	break;
		case 5:
		{
			/*Final dungeon of game:
				-Opens up after the required main quests have been completed
				-Completing will take game out of main loop, and take you to win screen
				-Losing will just result in gameover, exit main loop
			*/
			printf("Entering death dungeon. You best have prepared youself son.\n");
			break;
		}
		//	break;	
		case 6:
		{
			//Opens the game menu, can check quest progess, look at player stats, manage equipment, and save the game from this menu
			header("Game Menu");
			bool menu_open = true;
			while (menu_open == true)
			{
				int menu_sel;
				printf("1. View Inventory\n2. Check Quest Progress\n3. Manage Inventory\n4. Discard Item\n5. Close menu\n");
				scanf("%d", &menu_sel);
				switch (menu_sel)
				{
					case 1:
					{
						//Different cases call functions to do the things you mentioned eariler	
						RPG_GO.inv_disp(); 
						break;
					}
					case 2:
					{
						RPG_GO.Quest_Prog_disp();
						break;
					}
					case 3:
					{
						RPG_GO.inv_man();
						break;
					}
					case 4:
					{
						RPG_GO.inv_disc();
						break;
					}
					case 5:
					{
						menu_open = false;
						printf("Closing menu\n");
						break;
					}
				}
			}
		}
	} 
}


if (RPG_GO.Win_Cond() == true)
{
	//Victory Stuff, game complete
	printf("You won the game and junk\n");
}
else if(RPG_GO.Win_Cond() == false)
{
	//Game over
	printf("Game over, you lose. Try again chump and/or chumpette.\n");
}


return 0;
}
