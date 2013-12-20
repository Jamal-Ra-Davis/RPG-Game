#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <limits>
#include <assert.h>
#include "../headers/Safe_Input.h"

#define red "\033[0;31m"
#define green "\033[0;32m"
#define yellow "\033[0;33m"
#define blue "\033[0;34m"
#define purple "\033[0;35m"
#define none "\033[0m"

using namespace std;




class enemy
{
public:
	char name[32];
	int health;
};

/*
Lists should probably be in it's own header file. Will contain all information about the every data group in the game. Like total enemy list, total item list, weapons, armor, ect...
*/
class lists
{	
	enemy enemy_list[6];
public:
	lists();
	enemy get_enemy(int);
};
lists::lists()
{
	//enemy_list[0].name = "Ulti-wolf";
	sprintf(enemy_list[0].name, "Ulti-wolf");
	enemy_list[0].health = 15;
	//enemy_list[1].name = "Mega Gopher";
	sprintf(enemy_list[1].name, "Mega Gopher");
	enemy_list[1].health = 10;
	//enemy_list[2].name = "Nega Ferret";
	sprintf(enemy_list[2].name, "Nega Ferret");
	enemy_list[2].health = 12;
	//enemy_list[3].name = "Ultor the Destroyer";
	sprintf(enemy_list[3].name, "Ultor the Destroyer");
	enemy_list[3].health = 20;
	//enemy_list[4].name = "Man with Bad Cough";
	sprintf(enemy_list[4].name, "Man with Bad Cough");
	enemy_list[4].health = 3;
	//enemy_list[5].name = "Giant Bird with Gun";
	sprintf(enemy_list[5].name, "Giant Bird with Gun");
	enemy_list[5].health = 8;
}
enemy lists::get_enemy(int n)
{
	return enemy_list[n];
}

lists first; //Total list of monsters, exists in global scope

class areas 
{
	char name[32];
	int id;//Currently is not being used
	int start_col, start_row, boss_col, boss_row;
	int loc_col, loc_row;
	enemy monster_list[3];
// int *area_monsters;
	int monster_list_length;
	bool **movement;//[5][5];
	int SIZE;///////////
	bool **treasure;//[5][5];
	int enc_rate;
	int treas_rate;
	bool boss_defeated;
	
	int num_enm;
	enemy *enms_fought;
	int enm_level;

public:
	areas (char*, int);
	~areas ();
	void map_disp();
	void treasure_disp();
	void monster_disp();
	void info_disp();
	void enter_area();
	int change_loc(); 
	void examine_room();//Need to set it up so it actually gives you real items, not just a message
	bool enemy_encounter();
	void dungeon_loop();
	void random_dungeon();
	void random_dungeon2();
		int step_through(int,int);
	void file_dungeon();
	void battle_init();
	void battle_conclude();

};
areas::areas (char* NAME, int ID)
{
//	name = NAME;------------------------------------
	sprintf(name, "%s", NAME);
	id = ID;
	srand(time(NULL));
	boss_defeated = false;
	treas_rate = 15;
	monster_list_length = 3;

	num_enm = 0;	

	switch(ID)
	{
		case 1:
		{
			enc_rate = 25;
			monster_list[0]=first.get_enemy(0);
			monster_list[1]=first.get_enemy(2);
			monster_list[2]=first.get_enemy(4);
			start_col = 1;
			start_row = 0;
			boss_col = 1;
			boss_row = 4;
			SIZE = 5;
			movement = (bool**)malloc(SIZE*sizeof(bool*));
			treasure = (bool**)malloc(SIZE*sizeof(bool*));
			for (int i=0; i<SIZE; i++)
			{
				movement[i] = (bool*)malloc(SIZE*sizeof(bool));
				treasure[i] = (bool*)malloc(SIZE*sizeof(bool));
			}

			for (int i=0; i<SIZE;i++)
			{
				for (int j=0; j<SIZE; j++)
				{
					if (j==1)
						movement[i][j]=true;
					else
						movement[i][j]=false;
				}
			}
			break;	
		}
		case 2:
		{
			enc_rate = 35;
			monster_list[0]=first.get_enemy(5);
                        monster_list[1]=first.get_enemy(1);
                        monster_list[2]=first.get_enemy(3);
			start_col = 0;
			start_row = 2;
			boss_col = 4;
			boss_row = 2;
			SIZE = 7;
                        movement = (bool**)malloc(SIZE*sizeof(bool*));
			treasure = (bool**)malloc(SIZE*sizeof(bool*));
                        for (int i=0; i<SIZE; i++)
			{
                                movement[i] = (bool*)malloc(SIZE*sizeof(bool));
				treasure[i] = (bool*)malloc(SIZE*sizeof(bool));
			}

			for (int i=0; i<SIZE;i++)
                        {
                                for (int j=0; j<SIZE; j++)
                                {
                                        if (i==2)
                                                movement[i][j]=true;
                                        else
                                                movement[i][j]=false;
                                }
                        }
			break;
		}
		case 3:
		{
			enc_rate = 50;
			monster_list[0]=first.get_enemy(5);
                        monster_list[1]=first.get_enemy(4);
                        monster_list[2]=first.get_enemy(3);
			start_col = 3;//change back to 0
			start_row = 3;
			boss_col = 3;
			boss_row = 0;
			SIZE = 9;
                        movement = (bool**)malloc(SIZE*sizeof(bool*));
			treasure = (bool**)malloc(SIZE*sizeof(bool*));
                        for (int i=0; i<SIZE; i++)
			{
                                movement[i] = (bool*)malloc(SIZE*sizeof(bool));
				treasure[i] = (bool*)malloc(SIZE*sizeof(bool));
			}

			for (int i=0; i<SIZE;i++)
                        {
                                for (int j=0; j<SIZE; j++)
                                {
                                        if ((j==3)||(i==3))
                                                movement[i][j]=true;
                                        else
                                                movement[i][j]=false;
                                }
                        }
			break;
		}
		case 4:
		{
			random_dungeon();
			break;
		}
		case 5:
		{
			random_dungeon2();	
			break;
		}
		case 6:
		{
			file_dungeon();
			break;
		}
	}
}
areas::~areas()
{
	for (int i=0; i<SIZE; i++)
	{
		free(movement[i]);
		free(treasure[i]);
	}
	free(movement);
	free(treasure);
}
void areas::map_disp()
{
	cout << name << " map" << endl;
	printf("----------------------\n");
	for (int i=0; i<SIZE; i++)
	{
		for (int j=0; j<SIZE; j++)
		{
			printf("%s", none);
			if (((loc_col == j)&&(loc_row == i))&&((boss_col == j)&&(boss_row == i)))
				printf("%sO ", red);
			else if (((loc_col == j)&&(loc_row == i))&&((start_col == j)&&(start_row == i)))
				printf("%sO ", blue);
			else if ((loc_col == j)&&(loc_row == i))
				printf("%sO ", green);
			else if ((start_col == j)&&(start_row == i))
				printf("%sS ", blue);
			else if ((boss_col == j)&&(boss_row == i))
				printf("%sB ", red);
			else if (movement[i][j])
				printf("X ");
			else
				printf("_ ");
		}
		printf("%s\n", none);
	}
}
void areas::treasure_disp()
{
        cout << name << " treasure locations" << endl;
        printf("----------------------\n");
	for (int i=0; i<SIZE; i++)
	{
		for (int j=0; j<SIZE; j++)
		{
			printf("%s", none);
			if ((loc_col == j)&&(loc_row == i)&&(treasure[i][j]))
				printf("%sO ", yellow);
			else if ((loc_col == j)&&(loc_row == i))
				printf("%sO ", green);
                        else if ((treasure[i][j]))//&&(movement[i][j]))
                                printf("%sT ", yellow);
                        else if (movement[i][j])
                                printf("X ");
                        else
                                printf("_ ");
		}
		printf("%s\n", none);
	}
}
void areas::monster_disp()
{
	cout << "Monsters in " << name << endl;
	printf("-------------------------\n");
	for (int i=0; i<3; i++)
	{
		cout << "Monster " << (i+1) << ": " << monster_list[i].name << endl;
	}
}
void areas::info_disp()
{
	printf("Monster Encounter Rate: %d\n", enc_rate);
	printf("Dungeon size: %d\n", SIZE);
	monster_disp();
}
void areas::enter_area()
{
	loc_col = start_col;
	loc_row = start_row;
        for (int i=0; i<SIZE; i++)
        {
                for (int j=0; j<SIZE; j++)
                {
                        if (movement[i][j]&&((rand()%100)<treas_rate))
                                treasure[i][j] = true;
                        else
                                treasure[i][j] = false;
                        //treasure[i][j] = movement[i][j];
                }
        }
}
int areas::change_loc()
{
	bool choices_b[7] = {false, false, false, false, false, false, true};//up, left, down, right, leave dungeon, fight boss, do nothing
	int count = 0;
//	string choices_s[7] = {"Move Up", "Move Left", "Move Down", "Move Right", "Leave Dungeon", "Fight boss", "Do Nothing"};
	char choices_s[7][32] = {"Move Up", "Move Left", "Move Down", "Move Right", "Leave Dungeon", "Fight boss", "Do Nothing"};
	int choices_id[7] = {1, 2, 3, 4, 5, 6, 7};

	if ((loc_col == start_col)&&(loc_row == start_row))
		choices_b[4] = true;
	if ((loc_col == boss_col)&&(loc_row == boss_row))
                choices_b[5] = true;
	if (loc_row > 0)
	{
		if (movement[loc_row-1][loc_col])
			choices_b[0] = true;
	}
	if (loc_col > 0)
	{
		if (movement[loc_row][loc_col-1])
			choices_b[1] = true;
	}
	if (loc_row < SIZE-1)
	{
		if (movement[loc_row+1][loc_col])
			choices_b[2] = true;
	}
	if (loc_col < SIZE-1)
	{
		if (movement[loc_row][loc_col+1])
			choices_b[3] = true;
	}
	int sel;
	bool loop = true;
	int *array;
/**/	while (loop)
	{
		cin.clear();
		fflush(stdin);
//		std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n');
		count = 0;
		for (int i=0; i<7; i++)
		{
			if (choices_b[i])
			{
				count++;
				//cout << count << ". " << choices_s[i] << endl;
				printf("%d. %s\n", count, choices_s[i]);
			}
		}
		char *input = (char*)malloc(100*sizeof(char));;
		array = (int*)malloc(count*sizeof(int));
		int count2 = 0;
		for (int i=0; i<7;i++)
		{
			if (choices_b[i])
			{
				array[count2]=choices_id[i];
				count2++;	
			}
		}
		cin.getline(input, 100);
		sscanf(input, "%d", &sel);
		printf("This is my selection +1: %d\n", sel+1);
		if ((sel >= 1)&&(sel <= count))
		{
			loop = false;
		}
		else
		{
			printf("Invalid Entry\n");
		}
/**/	}
//	scanf("%d", &sel);
	printf("Premove Loc_row:%d,Loc_col:%d\n", loc_row, loc_col);
	switch(array[sel-1])
	{
		case 1:
		{
			loc_row = loc_row - 1;
			break;
		}
		case 2:
		{
			loc_col = loc_col - 1;
			break;
		}
		case 3:
		{
			loc_row = loc_row + 1;
			break;
		}
		case 4:
		{
			loc_col = loc_col + 1;
			break;
		}
		case 5:
		{
			//action for leaving dungeon
			printf("Leaving dungeon...\n");
			free(array);
			return 1;
			break;
		}
		case 6:
		{
			//action for fighting boss
			printf("Preparing to fight boss...\n");
			free(array);
			return 3;
			break;
		}
		case 7:
		{
			//action for doing nothing, HA!
			printf("Doing nothing now...\n");
			free(array);
			return 0;
			break;
		}
	}	
	printf("Postmove Loc_row:%d,Loc_col:%d\n", loc_row, loc_col);
	if (enemy_encounter()&&(array[sel-1]<5))
	{
		printf("Enemy encountered, prepare for battle...\n");
		//call battle function. Pass in necassary data from area
		free(array);
		return 2;
	}
	printf("Butts, I should have returned something!\n");
	free(array);
	return 0;
}
void areas::examine_room()
{
	//think about all the things that might happen when you examine a room
	//Just for treasure finding in it's current state
        printf("Examining room...\n");
        if ((treasure[loc_row][loc_col]))
	{
        	printf("You found a treasure chest!\n");
//		printf("Open it?\n1. Yes\n2. No\n");
		int sel;
		sel = getSel("1. Yes", "2. No");
//		scanf("%d", &sel);
		if (sel == 1)
		{
			srand(time(NULL));
			int chance = rand()%100;
			treasure[loc_row][loc_col] = false;//makes sure you can't open the same chest repeatedally
			/*From here, you need to randomly select from each tier of item list.
			 So if the item was common, and the common item list is 12 elements
			 long, you need to pick 1 of 12 random numbers to get the item.
			*/
			if (chance > 80) 
			{
				printf("You got rare item\n");
			}
			else if (chance > 50)
			{
				printf("You got mid-tier item\n");
			}
			else
			{
				printf("You got common item\n");
			}
		}
		else if (sel == 2)
		{
			printf("You decide to leave the chest unopened\n");
		}
	}
	else 
	{
		printf("Nothing found.\n");
	}
}
bool areas::enemy_encounter()
{
	srand(time(NULL));
	int chance = rand()%100;
	if (enc_rate > chance)
		return true;
	else
		return false;
}
void areas::dungeon_loop()
{
	bool in_area = true;
	enter_area();
	while (in_area)
	{
		int dun_sel;
		cout << name << endl;
		printf("------------------\n");
//		printf("1. Examine Room\n2. Change Location\n3. Check Status\n4. Open Inventory\n5. Open Map\n");
//		scanf("%d", &dun_sel);
		dun_sel = getSel("1. Examine Room","2. Change Location","3. Check Status","4. Open Inventory","5. Open Map","6. Leave Dungeon"); 
		switch(dun_sel)
		{
			case 1:
			{
				//Examine room
				examine_room();
				break;
			}
			case 2:
			{
				//Change Location
				int action;
				action = change_loc();
				switch(action)
				{
					case 0:
					{
						//do nothing
						break;
					}
					case 1:
					{
						//leave dungeon
						in_area = false;
						break;
					}
					case 2:
					{
						//random battle
/*
						battle_init();
						battle random_battle(enms_fought, num_enm, enm_level, RPG_GO.players, RPG_GO.num_ply);	
        					battle_conclude();
*/	
						break;
					}
					case 3:
					{
						//fight boss
						//boss_func() returns 0 if you win, 1 if you lose
						//boss_func() sets gameover to true if you lose
						in_area = false;
						break;
					}
				} 
				break;
			}
			case 3:
			{
				//Check Status
				printf("Checking Status...\n");
				break;
			}
			case 4:
			{
				//Open Inventory
				printf("Opening Inventory...\n");
				break;
			}
			case 5:
			{
				//Open Map
				int map_sel;
				printf("Which map would you like to see?\n");
//				printf("1. Area Map\n2. Treasure Map\n3. Dungeon Info\n");
//				scanf("%d", &map_sel);
				map_sel = getSel("1. Area Map","2. Treasure Map","3. Dungeon Info","4. Return");
				if (map_sel == 1)
				{
					map_disp();
				}
				else if (map_sel == 2)
				{
					treasure_disp();
				}
				else if (map_sel == 3)
				{
					info_disp();
				}
				else if (map_sel == 4)
				{
					//Return
				}
				break;
			}
			case 6:
			{
				in_area = false;
			}
		}
	}	
}
void areas::random_dungeon()
{
	
                      //  enc_rate = 25;
                      //  monster_list[0]=first.get_enemy(0);
                      //  monster_list[1]=first.get_enemy(2);
                      //  monster_list[2]=first.get_enemy(4);
                      //  start_col = 1;
                      //  start_row = 0;
                      //  boss_col = 1;
                      //  boss_row = 4;
                      //  for (int i=0; i<5;i++)
                      //          {
                      //          for (int j=0; j<5; j++)
                      //                  {
                      //                  if (j==1)
                      //                          movement[i][j]=true;
                      //                  else
                      //                          movement[i][j]=false;
                      //                  }
                      //
	SIZE = (rand()%36)+5;

        movement = (bool**)malloc(SIZE*sizeof(bool*));
        treasure = (bool**)malloc(SIZE*sizeof(bool*));
        for (int i=0; i<SIZE; i++)
        {
        	movement[i] = (bool*)malloc(SIZE*sizeof(bool));
                treasure[i] = (bool*)malloc(SIZE*sizeof(bool));
        }

	enc_rate = rand()%75;                        
	for (int i=0; i<3; i++)
	{
		monster_list[i] = first.get_enemy(rand()%6);
	}
	int num_mspc;
	do
	{
		start_col = rand()%SIZE;
		start_row = rand()%SIZE;
		for (int i=0; i<SIZE; i++)
		{
			for (int j=0; j<SIZE; j++)
			{
				movement[i][j]=false;
			}
		}
		movement[start_row][start_col] = true;

		int growth = SIZE;	
		for (int k=0; k<growth; k++)
		{
			for (int i=0; i<SIZE; i++)
			{
				for (int j=0; j<SIZE; j++)
				{
					int count = 0;
					bool chance = false;
					if ((j>0)&&(j<SIZE-1))
					{
						if (movement[i][j-1])
							count++;
						if (movement[i][j+1])
							count++;
					}
					else if (j==0)
					{
						if(movement[i][j+1])
							count++;
					}
					else if (j==SIZE-1)
					{
						if(movement[i][j-1])
							count++;
					}

					if ((i>0)&&(i<SIZE-1))
                                	{
                                        	if (movement[i-1][j])
							count++; 
						if (movement[i+1][j])
                                                	count++;
                                	}
                                	else if (i==0)
                                	{
                                        	if(movement[i+1][j])
                                                	count++;
                                	}
                                	else if (i==SIZE-1)
                                	{
                                        	if(movement[i-1][j])
                                                	count++;
                                	}
					if (count == 1)
						chance = true;

					if ((chance) && ((rand()%100) < 30))
						movement[i][j] = true;
				}
			}
		}

		num_mspc=0;
		for (int i=0; i<SIZE; i++)
		{
			for (int j=0; j<SIZE; j++)
			{
				if ((movement[i][j])&&((i != start_row)&&(j != start_col)))
					num_mspc++;
			}
		}
	}while(num_mspc < 1);
	int boss_loc = rand()%num_mspc;
	int loc_count = 0;
	for (int i=0; i<SIZE; i++)
	{
		for (int j=0; j<SIZE; j++)
		{
			if ((movement[i][j])&&((i != start_row)&&(j != start_col)))
			{
				if (loc_count == boss_loc)
				{
					boss_row=i;
					boss_col=j;
				}
			loc_count++;	
			}
		}
	}
}
void areas::random_dungeon2()
{
                      //  enc_rate = 25;
                      //  monster_list[0]=first.get_enemy(0);
                      //  monster_list[1]=first.get_enemy(2);
                      //  monster_list[2]=first.get_enemy(4);
                      //  start_col = 1;
                      //  start_row = 0;
                      //  boss_col = 1;
                      //  boss_row = 4;
                      //  for (int i=0; i<5;i++)
                      //          {
                      //          for (int j=0; j<5; j++)
                      //                  {
                      //                  if (j==1)
                      //                          movement[i][j]=true;
                      //                  else
                      //                          movement[i][j]=false;
                      //                  }
                      
        SIZE = (rand()%36)+5;

        movement = (bool**)malloc(SIZE*sizeof(bool*));
        treasure = (bool**)malloc(SIZE*sizeof(bool*));
        for (int i=0; i<SIZE; i++)
        {
	        movement[i] = (bool*)malloc(SIZE*sizeof(bool));
                treasure[i] = (bool*)malloc(SIZE*sizeof(bool));
        }

	int num_mspc, pos_col, pos_row;

	enc_rate = rand()%75;
        for (int i=0; i<3; i++)
        {
                monster_list[i] = first.get_enemy(rand()%6);
        }
	do
	{
        	for (int i=0; i<5; i++)
        	{
                	for (int j=0; j<5; j++)
                	{
                        	movement[i][j]=false;
                	}
        	}	
		start_row = rand()%SIZE;
		start_col = rand()%SIZE;
		movement[start_row][start_col] = true;
	
		pos_col = start_col;
		pos_row = start_row;
		num_mspc = step_through(start_row, start_col);
/*	
		bool loop = true;
		while(loop == true)
		{
			int dir = rand()%4;
			switch (dir)
			{
				case 0:
				{
					pos_row--;
					break;
				}
				case 1:
				{
					pos_col++;
					break;
				}
				case 2:
				{
					pos_row++;
					break;
				}
				case 3:
				{
					pos_col--;
					break;
				}
			}
			if ((pos_col>4)||(pos_col<0)||(pos_row>4)||(pos_row<0))
			{
				loop = false;	
			}
			else
			{
				movement[pos_row][pos_col] = true;
			}
		
		}
////////////////////////////////////////////
		num_mspc=0;
        	for (int i=0; i<5; i++)
        	{
                	for (int j=0; j<5; j++)
                	{
                        	if ((movement[i][j])&&((i != start_row)&&(j != start_col)))
                                	num_mspc++;
                	}
        	}
*/
	}while(num_mspc < 1);
	
        int boss_loc = rand()%num_mspc;
        int loc_count = 0;
        for (int i=0; i<SIZE; i++)
        {
                for (int j=0; j<SIZE; j++)
                {
                        if ((movement[i][j])&&((i != start_row)&&(j != start_col)))
                        {
                                if (loc_count == boss_loc)
                                {
                                        boss_row=i;
                                        boss_col=j;
                                }
                        	loc_count++;
                        }
                }
        }
}
void areas::file_dungeon()
{
	/*
			enc_rate = 25;
                        monster_list[0]=first.get_enemy(0);
                        monster_list[1]=first.get_enemy(2);
                        monster_list[2]=first.get_enemy(4);
                        start_col = 1;
                        start_row = 0;
                        boss_col = 1;
                        boss_row = 4;
                        SIZE = 5;
                        movement = (bool**)malloc(SIZE*sizeof(bool*));
                        treasure = (bool**)malloc(SIZE*sizeof(bool*));
                        for (int i=0; i<SIZE; i++)
                        {
                                movement[i] = (bool*)malloc(SIZE*sizeof(bool));
                                treasure[i] = (bool*)malloc(SIZE*sizeof(bool));
                        }

                        for (int i=0; i<SIZE;i++)
                        {
                                for (int j=0; j<SIZE; j++)
                                {
                                        if (j==1)
                                                movement[i][j]=true;
                                        else
                                                movement[i][j]=false;
                                }
                        }

	*/
	FILE *fp;
	fp = fopen("dungeon.txt", "r");
	assert(fp != NULL);
	fscanf(fp, "%d ", &SIZE);
	fscanf(fp, "%d %d", &start_row, &start_col);
	fscanf(fp, "%d %d", &boss_row, &boss_col);
	fscanf(fp, "%d ", &enc_rate);
	for (int i=0; i<3; i++)
        {
		int temp;
		fscanf(fp, "%d ", &temp);
                monster_list[i] = first.get_enemy(temp);
        }
	movement = (bool**)malloc(SIZE*sizeof(bool*));
        treasure = (bool**)malloc(SIZE*sizeof(bool*));
	for (int i=0; i<SIZE; i++)
        {
        	movement[i] = (bool*)malloc(SIZE*sizeof(bool));
                treasure[i] = (bool*)malloc(SIZE*sizeof(bool));
        }
	for (int i=0; i<SIZE;i++)
        {
        	for (int j=0; j<SIZE; j++)
        	{
			int temp;
			fscanf(fp, "%d ", &temp);
			movement[i][j] = temp;
        	}
        }
	fclose(fp);
}
int areas::step_through(int pos_row, int pos_col)
{
	bool loop = true;
        while(loop == true)
        {
        	int dir = rand()%4;
                switch (dir)
                {
                	case 0:
                        {
                        	pos_row--;
                                break;
                        }
                        case 1:
                        {
                                pos_col++;
                                break;
                        }
                        case 2:
                        {
                                pos_row++;
                                break;
                        }
                        case 3:
                        {
                                pos_col--;
                                break;
                        }
                }
                if ((pos_col>SIZE-1)||(pos_col<0)||(pos_row>SIZE-1)||(pos_row<0))
                {
                        loop = false;
                }
                else
                {
                        movement[pos_row][pos_col] = true;
                }

	}

        int num_mspc=0;
        for (int i=0; i<SIZE; i++)
        {
        	for (int j=0; j<SIZE; j++)
                {
                	if ((movement[i][j])&&((i != start_row)&&(j != start_col)))
                        	num_mspc++;
                }
        }
	return num_mspc;
}
void areas::battle_init()
{
	num_enm = (rand()%4)+1;
	enms_fought = (enemy*)malloc(num_enm*sizeof(enemy));
	
	for (int i=0; i<num_enm; i++)
	{
		enms_fought[i] = monster_list[rand()%monster_list_length];		
	}	
}
void areas::battle_conclude()
{
	num_enm = 0;
	free(enms_fought);
}
int main()
{

	printf("1\n");
	areas grass("Grassy Plains", 1);
	printf("2\n");
	areas snow ("Snow-covered Mountains", 2);
	areas fire ("Lava Hell-scape", 3);
	areas random("Random Dungeon", 4);
	areas rand_alt("Random Dun Alt", 5);
	areas file_dungeon("File Dungeon", 6);

	grass.dungeon_loop();
	snow.dungeon_loop();
	fire.dungeon_loop();
	random.dungeon_loop();
	rand_alt.dungeon_loop();
	file_dungeon.dungeon_loop();
	
/*
	printf("Grass map\n");
	grass.enter_area();
	grass.map_disp();
	grass.monster_disp();
	grass.change_loc();
	grass.map_disp();
	grass.treasure_disp();

	printf("\nSnow map\n");
	snow.enter_area();
	snow.map_disp();
	snow.monster_disp();
	snow.change_loc();
	snow.map_disp();
	snow.treasure_disp();

	printf("\nFire map\n");
	fire.enter_area();
	fire.map_disp();
	fire.monster_disp();
	fire.change_loc();
	fire.map_disp();
	fire.treasure_disp();	
*/

	return 0;
}
