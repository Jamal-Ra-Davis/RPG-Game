#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../headers/game_hub.h"
#include "../headers/party.h"
#include "../headers/areas.h"
#include "../headers/Safe_Input.h"
#include "../headers/player.h"
#include "../headers/shop.h"
#include "../headers/inn.h"
#include "../headers/home.h"
#include "../headers/quest.h"
#include "../headers/questHall.h"

game_hub::game_hub()
{
	Team = new party;

	printf("party gold: %d, party max size: %d\n", Team->getGold(), Team->getMaxPartySize());	

	num_areas = 4;
	active_areas = 0;

	Dungeons = new areas*[num_areas];
	for (int i=0; i<num_areas; i++)
	{
		Dungeons[i] = new areas;
	}
	Dungeons[0]->setArea("Grassy Plains", 1, Team);
	Dungeons[1]->setArea("Snow-covered Mountains", 2, Team);
	Dungeons[2]->setArea("Lava Hell-scape", 3, Team);
	Dungeons[3]->setArea("File Dungeon", 6, Team);

	rand_dungeon = NULL;

	num_shops = getShopRange();
	printf("Number of total shops = %d\n", num_shops);
	active_shops = 0;
	
	Shops = new shop*[num_shops];
	for (int i=0; i<num_shops; i++)
	{
		Shops[i] = getShop(i+1);
	}

	Inn = new inn;
	Inn->setInn(300);

	Home = new home(4, 100);

//	Hall = new questHall(1, "Quest Hall", 4);
    Hall = getQuestHall(1);
/*
	int ids[14];
	int size = 14;
	for (int i=0; i<14; i++)
		ids[i] = i+1;
	Shops[0]->setShop(1, "Everything Shop", ids, size, 0.75);
*/
}
game_hub::~game_hub()
{
	if (Team != NULL)
		delete Team;

	for (int i=0; i<num_areas; i++)
	{
		if (Dungeons[i] != NULL)
			delete Dungeons[i];
	}
	delete [] Dungeons;
	if (rand_dungeon != NULL)
		delete rand_dungeon;

	for (int i=0; i<num_shops; i++)
		delete Shops[i];
	if (Shops != NULL)
		delete [] Shops;

	delete Inn;	
	delete Home;
	delete Hall;
}
void game_hub::startGame()
{
	printf("Would you like to start a new game, or load a previous save?\n");
	int sel = getSel("1. New Game", "2. Load Game");
	if (sel == 1)
		newGame();
	else
		loadGame();
}
void game_hub::newGame()
{
	printf("Starting new game...\n\n");
	active_areas = 1;
	active_shops = 1;

	player *character = getNewPlayer();

	Team->recievePlayer(character);
	Team->recieveGold(300);	
}
void game_hub::loadGame()
{
	printf("Loading game...\n");
	FILE *fp;
	fp = fopen("./files/Game_save.txt", "r");
	fscanf(fp, " %d %d ", &active_areas, &active_shops);
    fclose(fp);
	Team->loadParty();
	Home->loadHome();
    fp = fopen("./files/QuestHall_save.txt", "r");
    Hall->loadQuestHall(fp);
    fclose(fp);
}
void game_hub::saveGame()
{
	printf("Do you want to save your game?\n");
	int sel = getSel("1. Yes", "2. No");
	if (sel == 2)
		return;
	printf("Saving game...\n");
	FILE *fp;
	fp = fopen("./files/Game_save.txt", "w");
	fprintf(fp, "%d %d", active_areas, active_shops);
	fclose(fp);
	Team->saveParty();
	Home->saveHome();
    fp = fopen("./files/QuestHall_save.txt", "w");
    Hall->saveQuestHall(fp);
    fclose(fp);
}
void game_hub::gameLoop()
{
	startGame();
	bool in_town = true;
	while (in_town)
	{
		printf("Hub Town\n");
		printf("----------------------\n");
		int hub_sel = getSel("1. Free Exploration", "2. Marketplace",
									"3. Quest Hall", "4. Inn", "5. Home",
									"6. Open Menu", "7. Quit");
		switch (hub_sel)
		{
			case 1://Free Exploration
			{
				/*
				printf("Chooses dungeon to go to:\n");
				int dun_sel = getSel("1. Babby's first dungeon", "2. Random Dungeon", "3. Cancel");
				switch(dun_sel)
				{
					case 1:
					{
						Dungeons[0]->dungeon_loop();
						break;
					}
					case 2:
					{
						rand_dungeon = new areas;
						rand_dungeon->setArea("Random Dungeon", 4, Team);
						rand_dungeon->dungeon_loop();
						delete rand_dungeon;
						rand_dungeon = NULL;
						break;
					}
					case 3:
					{
						break;
					}
				}
				*/
				areaMenu();	
				break;
			}
			case 2://Marketplace
			{
				shopMenu();
//				Shops[0]->enterShop(Team);
				break;
			}
			case 3://Quest Hall
			{
				Hall->enterQuestHall(Team);
				break;
				//--------------------------------------------
				printf("Welcome to the quest hall, what would you like to do?\n");
				int sel = getSel("1. Recruit new party member", "2. Take Quest",
									  "3. Leave");
				if (sel == 1)
				{
					printf("\n");
					int sel_c = getSel("1. Make new player", "2. Make random player",
											 "3. Cancel action");
					switch(sel_c)
					{
						case 1:
						{
							player *player_n = getNewPlayer();
							player *test;
							test = Team->recievePlayer(player_n);
							if (test != NULL)
								delete test;
							break;
						}
						case 2:
						{
							printf("**DEBUG** Generating random party member...\n");
							player *random = getRandomPlayer();
							player *test;
							test = Team->recievePlayer(random);
							if (test != NULL)
								delete test;
							break;
						}
/*
						case 3:
						{
							quest *temp = getQuest(1);
							printf("Accept quest: '%s'?\n", temp->getName());
							int sel = getSel("1. Yes", "2. No");
							if (sel == 1)
							{
								Team->acceptQuest(temp);
							}
							else
							{
								printf("Did not accept '%s'...\n", temp->getName());
								delete temp;
							}
							break;
						}
*/
						case 3:
						{
							printf("Cancelling action...\n");
							break;
						}
					}
				}
				else if (sel == 2)
				{
					quest *temp = getQuest(1);
					printf("Accept quest: '%s'?\n", temp->getName());
					int sel = getSel("1. Yes", "2. No");
					if (sel == 1)
					{
						Team->acceptQuest(temp);
					}
					else
					{
						printf("Did not accept '%s'...\n", temp->getName());
						delete temp;
					}
				}
				else
					printf("Leaving quest hall...\n");
				break;
			}
			case 4://Inn
			{
				Inn->enterInn(Team);
				break;
			}
			case 5://Home
			{
				Home->enterHome(Team);
				break;
			}
			case 6://Open Menu
			{
				hubMenu();
				break;
			}
			case 7:
			{
				printf("Are you sure you want to quit?\n");
				int sel = getSel("1. Yes", "2. No");
				if (sel == 1)
					in_town = false;
				else
					printf("Cancelling action...\n");
				break;
			}	
		}
		if (Team->checkGameOver())
			in_town = false;	

	}	
}
void game_hub::hubMenu()
{
	bool hubMenu = true;
	while (hubMenu)
	{
		printf("Hub Menu\n");
		printf("----------------------\n");
		int menu_sel = getSel("1. Open Party Menu", "2. Manage Characters",
								 	 "3. Save Game", "4. Cancel");
		switch (menu_sel)
		{
			case 1://Open party menu
			{
				Team->partyMenu();
				break;
			}
			case 2://Manage Characters
			{
				Team->managePlayers();
				break;
			}
			case 3://Save Game
			{
				saveGame();
				break;
			}
			case 4://Cancel
			{
				printf("Cancelling action...\n");
				hubMenu = false;
				break;
			}
		}
	}
}
void game_hub::printAreas()
{
	printf("ID   Name                            Difficulty\n");
	printf("===  ==============================  ============\n");
	for(int i=0; i<active_areas; i++)
	{
		printf("%2d.  %-30s  ", i+1, Dungeons[i]->getName());
		int diff = Dungeons[i]->getDifficulty();
		for (int j=0; j<diff; j++)
			printf("*");

		printf("\n");
	}
	printf("\n");
}
void game_hub::printShops()
{
	printf("ID   Name              \n");
	printf("===  ==============================\n");
	for (int i=0; i<active_shops; i++)
	{
		printf("%2d.  %-30s\n", i+1, Shops[i]->getName());
	}
	printf("\n");
}
void game_hub::areaMenu()
{
	bool areaMenu = true;
	while (areaMenu)
	{
		int area_sel = getSel("1. Go to dungeons", "2. Go to random dungeon",
									 "3. Return to town");
		switch(area_sel)
		{
			case 1:
			{
				printAreas();
				printf("%d. Cancel\n", active_areas+1);
				int sel = getSel(active_areas+1);
				if (sel < active_areas+1)
				{
					sel--;
					Dungeons[sel]->dungeon_loop();
					areaMenu = false;
				}
				else
					printf("Cancelling action...\n");
				break;
			}
			case 2:
			{
				rand_dungeon = new areas;
				rand_dungeon->setArea("Random Dungeon", 4, Team);
				rand_dungeon->dungeon_loop();
				areaMenu = false;
				delete rand_dungeon;
				rand_dungeon = NULL;
				break;
			}
			case 3:
			{
				printf("Returning to town...\n");
				areaMenu = false;
				break;
			}
		}
	}
	incrementAreas();
}
void game_hub::shopMenu()
{
	bool shopMenu = true;
	while(shopMenu)
	{
		printShops();
		printf("%d. Return to town\n", active_shops+1);
		int sel = getSel(active_shops+1);
		if (sel < active_shops+1)
		{
			sel--;
			Shops[sel]->enterShop(Team);
		}
		else
		{
			printf("Returning to town...\n");
			shopMenu = false;
		}
	}
	incrementShops();
}
void game_hub::incrementAreas()
{
	active_areas++;
	if (active_areas > num_areas)
		active_areas = num_areas;
}
void game_hub::incrementShops()
{
   active_shops++;
   if (active_shops > num_shops)
      active_shops = num_shops;
}
