#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <limits>
#include <assert.h>
#include "../headers/Safe_Input.h"
#include "../headers/enemy.h"
#include "../headers/areas.h"
#include "../headers/player.h"
#include "../headers/item.h"
#include "../headers/battle.h"
#include "../headers/FileFunctions.h"
#include "../headers/party.h"

#define red "\033[0;31m"
#define green "\033[0;32m"
#define yellow "\033[0;33m"
#define blue "\033[0;34m"
#define purple "\033[0;35m"
#define none "\033[0m"

using namespace std;


areas* getArea(int id, party *party_in)
{
    int cnt = lineCount("./files/Dungeon_List.txt");
    int ID;
    FILE *fp;
    char dungeon_filename[64];
    char *line;

    if ((id > cnt)||(id <= 0))
    {
        fprintf(stderr, "Item id did not match file's\n");
        return NULL;
    }

    line = getWholeLine("./files/Dungeon_List.txt", id);
    fp = fopen("./files/tempArea.txt", "w");
    fprintf(fp, "%s \n", line);
    fclose(fp);
    free(line);

    fp = fopen("./files/tempArea.txt", "r");
    fscanf(fp, " %d '%[^']' ", &ID,  dungeon_filename);
    fclose(fp);

    if (id != ID)
        fprintf(stderr, "You dun goofed with the area fetching!\n");    

    areas *dungeon = new areas(dungeon_filename, party_in);
    return dungeon;    

}



areas::areas(char* NAME, int ID, party *party_in)
{
	setArea(NAME, ID, party_in);
}
areas::areas(char* filename, party* team)
{
    loadArea(filename, team);
}
void areas::setArea(char* NAME, int ID, party *party_in)
{
    nullPointers();
	sprintf(name, "%s", NAME);
	id = ID;
	srand(time(NULL));
	boss_defeated = false;
	treas_rate = 75;

	num_enm = 0;	

	characters = party_in;	

	//Could set individually, but lazy
	num_monsters = 3;
	monster_lvl = 5;

//	boss = NULL;
    boss_encounter_size = 1;
    boss_IDs = (int*)malloc(boss_encounter_size*sizeof(int));
    
    //You know encouter size is 1, so this is ok
    boss_IDs[0] = 5;
    boss_encounter = new enemy*[boss_encounter_size];
	boss_encounter[0] = getEnemy(boss_IDs[0]);

//    enms_fought = NULL;
    random_encounter = NULL;

	switch(ID)
	{
		case 1:
		{
			enc_rate = 25;
//Remember to go back and set the internal monster lists using getEnemy
//			monster_list[0]=first.get_enemy(0);
//			monster_list[1]=first.get_enemy(2);
//			monster_list[2]=first.get_enemy(4);
			area_monsters = (int*)malloc(num_monsters*sizeof(int));
			area_monsters[0] = 1;
			area_monsters[1] = 2;
			area_monsters[2] = 4;

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

//			boss = getEnemy(5);
			boss_level = 4;
//			boss->setLevel(boss_level);
			break;	
		}
		case 2:
		{
			enc_rate = 35;
//Remember to go back and set the internal monster lists using getEnemy
//			monster_list[0]=first.get_enemy(5);
//			monster_list[1]=first.get_enemy(1);
//			monster_list[2]=first.get_enemy(3);
			area_monsters = (int*)malloc(num_monsters*sizeof(int));
         area_monsters[0] = 1;
         area_monsters[1] = 2;
         area_monsters[2] = 3;

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
//			boss = getEnemy(5);
         boss_level = 6;
//       boss->setLevel(boss_level);
			break;
		}
		case 3:
		{
			enc_rate = 50;
//Remember to go back and set the internal monster lists using getEnemy
//			monster_list[0]=first.get_enemy(5);
//			monster_list[1]=first.get_enemy(4);
//			monster_list[2]=first.get_enemy(3);
			area_monsters = (int*)malloc(num_monsters*sizeof(int));
         area_monsters[0] = 2;
         area_monsters[1] = 3;
         area_monsters[2] = 4;

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

//			boss = getEnemy(5);
         boss_level = 8;
//       boss->setLevel(boss_level);
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
	lvl_range = (int)(monster_lvl*0.2);	
	boss_encounter[0]->setLevel(boss_level);
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
	free(area_monsters);
	if (boss_encounter != NULL)
    {
        for (int i=0; i<boss_encounter_size; i++) {
            delete boss_encounter[i];
        }
		delete []boss_encounter;
    }
    if (boss_IDs != NULL)
        free(boss_IDs);
/*
    if (enms_fought != NULL)
        delete [] enms_fought;
*/
    if (random_encounter != NULL)
    {
        for (int i=0; i<num_enm; i++)
        {
            delete random_encounter[i];
        }
        delete [] random_encounter;
    }
}
void areas::nullPointers()
{
    movement = NULL;
    treasure = NULL;
//    enms_fought = NULL;
    random_encounter = NULL;
    boss_IDs = NULL;
    characters = NULL;
}
void areas::loadArea(char* filename, party* team)
{
    //boss_level = 25;
    nullPointers();
    boss_defeated = false;
//    enms_fought = NULL;
    random_encounter = NULL;
    characters = team;
    sprintf(name, "%s", "Test Dungeon");

    FILE *fp;
    fp = fopen(filename, "r");
    assert(fp != NULL);

    fscanf(fp, " '%[^']' ", name);                          //Dungeon Name
    fscanf(fp, "%d ", &SIZE);                               //Dungeon Size
    fscanf(fp, "%d %d", &start_row, &start_col);            //Starting location
    fscanf(fp, "%d %d", &boss_row, &boss_col);              //Boss location
    fscanf(fp, "%d ", &treas_rate);                         //Treasure spawn rate
    fscanf(fp, "%d ", &enc_rate);                           //Enemy encounter rate
    fscanf(fp, "%d ", &monster_lvl);                        //Base monster level
    fscanf(fp, "%d ", &lvl_range);                          //Monster level offset
    fscanf(fp, "%d ", &num_monsters);                       //Size of enemy pool
    area_monsters = (int*)malloc(num_monsters*sizeof(int)); //Enemy pool array

    //Set the monster IDs for area
    for (int i=0; i<num_monsters; i++)
    {
        int temp;
        fscanf(fp, "%d ", &temp);
        area_monsters[i] = temp;
    }

    //Set Boss information
    
    //    How it should work in the future:
    //boss_encounter_size;
    //int *boss_IDs;
    fscanf(fp, "%d ", &boss_level);
    fscanf(fp, "%d ", &boss_encounter_size);
    boss_IDs = (int*)malloc(boss_encounter_size*sizeof(int));
    boss_encounter = new enemy*[boss_encounter_size];
    for (int i=0; i<boss_encounter_size; i++) 
    {
        int temp;
        fscanf(fp, "%d ", &temp);
        boss_IDs[i] = temp;
        boss_encounter[i] = getEnemy(boss_IDs[i]);
        boss_encounter[i]->setLevel(boss_level);
    }
    
//----------
//    int boss_ID;
//    fscanf(fp, "%d ", &boss_level);
//    fscanf(fp, "%d ", &boss_ID);
//    boss = new enemy*[1];
//    boss[0] = getEnemy(boss_ID);
//    boss[0]->setLevel(boss_level);
//------------

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
void areas::map_disp()
{
	printf("%s map\n", name);
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
	printf("%s treasure locations\n", name);
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
	printf("Monsters in %s\n", name);
	printf("-------------------------\n");
	for (int i=0; i<num_monsters; i++)
	{
		enemy *temp;
		temp = getEnemy(area_monsters[i]);
		printf("Monster %d: %s\n", i+1, temp->getName());
		delete temp;
	}
}
void areas::info_disp()
{
	printf("Monster Encounter Rate: %d\n", enc_rate);
	printf("Dungeon size: %d\n", SIZE);
	printf("Area Level: %d\n", monster_lvl);
	monster_disp();
}
int areas::getDifficulty()
{
	return ((monster_lvl/10)+1);
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
    //Need to remember to reset boss' stats if it hasn't been defeated
    dungeon_loop();
}
void areas::leave_area()
{
    //Whatever cleanup code you want to call when the area is left goes here
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
	while (loop)
	{
		count = 0;
		for (int i=0; i<7; i++)
		{
			if (choices_b[i])
			{
				count++;
				printf("%d. %s\n", count, choices_s[i]);
			}
		}
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
		sel = getInt();
		printf("This is my selection +1: %d\n", sel+1);
		if ((sel >= 1)&&(sel <= count))
		{
			loop = false;
		}
		else
		{
			printf("Invalid Entry\n");
			free(array);
		}
	}

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
	printf("No enemies encounterd\n");
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
		printf("You found a treasure chest! Open it?\n");
//		printf("Open it?\n1. Yes\n2. No\n");
		int sel;
		sel = getSel("1. Yes", "2. No");
//		scanf("%d", &sel);
		if (sel == 1)
		{
			srand(time(NULL));
//			int chance = rand()%100;
			int tres_id = (rand()%14)+1;

			item *tres = getItem(tres_id);
			treasure[loc_row][loc_col] = false;//makes sure you can't open the same chest repeatedally
			printf("You found a %s\n", tres->getName());

//			recieveItem(inventory, num_itms, tres);
			characters->recieveItem(tres);


			/*From here, you need to randomly select from each tier of item list.
			 So if the item was common, and the common item list is 12 elements
			 long, you need to pick 1 of 12 random numbers to get the item.
			*/
/*
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
*/
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
void areas::resetBoss()
{
    for (int i=0; i<boss_encounter_size;i++)
        boss_encounter[i]->resetEnemy();
}
void areas::dungeon_loop()
{
	bool in_area = true;
	//enter_area();
	while (in_area)
	{
		int dun_sel;
		printf("%s\n", name);
		printf("------------------\n");
//		printf("1. Examine Room\n2. Change Location\n3. Check Status\n4. Open Inventory\n5. Open Map\n");
//		scanf("%d", &dun_sel);
		dun_sel = getSel("1. Examine Room", "2. Change Location", "3. Manage Characters",
							  "4. Open Party Menu", "5. Open Map", "6. Leave Dungeon"); 
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
						int status;
						battle_init();

						//battle random_battle(enms_fought, num_enm, characters);	
                        battle random_battle(random_encounter, num_enm, characters);
        				status = random_battle.battleLoop();
						if (status == 0)
                        {
                            //Players won battle
							random_battle.battleAwards();
                        }
						else if (status == 1)
						{
							printf("Game over\n");
							//Do stuff that will take you to gameover screen
							in_area = false;
								//RPG_GO.gameOver = true;				
						}
                        else if(status == 2)
                        {       
                            //Party fled from battle. Put whatever code you want to happen here when party flees
                        }

						printf("Battle concluded...\n");
						battle_conclude();
						
						break;
					}
					case 3:
					{
						//fight boss

						//boss_func() returns 0 if you win, 1 if you lose
						//boss_func() sets gameover to true if you lose
                        if (boss_defeated)
                        {
                            printf("The boss for this dungeon has aready been defeated. Would you like to leave dungon?\n");
                            int leave_sel = getSel("1. Yes", "2. No");
                            if (leave_sel == 1)
                            {
                                printf("Leaving dungeon...\n");
                                in_area = false;
                            }
                            else 
                            {
                                printf("You decide to stay in the dungeon...\n");
                            }
                            break;
                            
                        }
						int status;
						printf("Preparing to fight %s boss...\n", name);
                        resetBoss();
						battle boss_battle(boss_encounter, boss_encounter_size, characters);
						status = boss_battle.battleLoop();
						if (status == 0)
						{
                            //Players won battle
							boss_battle.battleAwards();
							boss_defeated = true;
                            in_area = false;
						}
                        else if (status == 1)
                        {
                            //Were defeated by the boss. Put appropriate code here 
                            in_area = false;
                        }
                        else if (status == 2)
                        {
                            //Party fled from battle. Put whatever code you want to happen here when party flees
                            //reset boss' health and stats
                        }
						break;
					}
				} 
				break;
			}
			case 3:
			{
				characters->managePlayers();
				break;
			}
		
			case 4:
			{
				characters->partyMenu();
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
				break;
			}
		}
	}	
    leave_area();
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

	monster_lvl = (rand()%76)+5; 
	num_monsters = (rand()%10)+1;
	area_monsters = (int*)malloc(num_monsters*sizeof(int));
	int mon_range = getEnemyRange();
	for (int i=0; i<num_monsters; i++)
	{
		area_monsters[i] = (rand()%mon_range)+1;
	}
	boss_level = (rand()%76)+10;


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
						if (movement[i][j+1])
							count++;
					}
					else if (j==SIZE-1)
					{
						if (movement[i][j-1])
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

	monster_lvl = (rand()%76)+5;	
	num_monsters = (rand()%10)+1;
	area_monsters = (int*)malloc(num_monsters*sizeof(int));
	int mon_range = getEnemyRange();
   for (int i=0; i<num_monsters; i++)
   {   
      area_monsters[i] = (rand()%mon_range)+1;
   }
	boss_level = (rand()%76)+10;

	do
	{
		for (int i=0; i<SIZE; i++)
		{
			for (int j=0; j<SIZE; j++)
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
	boss_level = 25;
	FILE *fp;
	fp = fopen("./files/dungeon.txt", "r");
	assert(fp != NULL);
	fscanf(fp, "%d ", &SIZE);
	fscanf(fp, "%d %d", &start_row, &start_col);
	fscanf(fp, "%d %d", &boss_row, &boss_col);
	fscanf(fp, "%d ", &enc_rate);
	fscanf(fp, "%d ", &monster_lvl);
	fscanf(fp, "%d ", &num_monsters);
	area_monsters = (int*)malloc(num_monsters*sizeof(int));

	for (int i=0; i<num_monsters; i++)
	{
		int temp;
		fscanf(fp, "%d ", &temp);
		area_monsters[i] = temp;
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
/*
    if (enms_fought != NULL)
    {
        delete [] enms_fought;
        enms_fought = NULL;
    }
*/
    if (random_encounter != NULL)
    {
        for (int i=0; i<num_enm; i++)
            delete random_encounter[i];
        delete [] random_encounter;
        random_encounter = NULL;
    }   
 
	num_enm = (rand()%4)+1;
//	enms_fought = new enemy [num_enm];
    random_encounter = new enemy*[num_enm];	

	for (int i=0; i<num_enm; i++)
	{
		//Should have an array of enemy IDs specific to each area
		enemy *temp = getEnemy(area_monsters[rand()%num_monsters]);
//		enms_fought[i] = *temp;
//		enms_fought[i].resetSkillLst();

        random_encounter[i] = temp;

		int LEVEL = monster_lvl;
        int offset = (rand()%(2*lvl_range + 1)) - lvl_range;
		LEVEL += offset;

		if (LEVEL < 1)
			LEVEL = 1;
		if (LEVEL > 100)
			LEVEL = 100;

//		enms_fought[i].setLevel(LEVEL);
        random_encounter[i]->setLevel(LEVEL);
		//delete temp;		
	}	
}
void areas::battle_conclude()
{
//	free(enms_fought);
/*
    if (enms_fought != NULL)
    {
//	    delete [] enms_fought;
        enms_fought = NULL;
    }
*/
    if (random_encounter != NULL)
    {
        for (int i=0; i<num_enm; i++)
            delete random_encounter[i];
        delete [] random_encounter;
        random_encounter = NULL;
    }    
    //num_enm = 0;
}




