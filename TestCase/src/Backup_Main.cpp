#include "../headers/player.h"
#include "../headers/enemy.h"
#include "../headers/battle.h"
#include "../headers/item.h"
#include "../headers/areas.h"
#include "../headers/party.h"
#include <stdio.h>
#include <stdlib.h>




int main()
{
	party Team;
	player *Characters[3];
	int PlayerNum = 3;
	for (int i=0; i<PlayerNum; i++)
	{
		Characters[i] = new player;
	}

	enemy BadGuys[4];
	int EnemyNum = 4;

	int InvNum = 10;
	item *Inventory[InvNum];

	int EnemyLevel = 5;

	for (int i=0; i<InvNum; i++)
		Inventory[i] = NULL;
/*
	printf("Inventory:\n");
	for (int i=0; i<InvNum; i++)
	{
		if (i < InvNum)
		{
			item *tmp;
			int isel = (rand()%8)+1;
//			Inventory[i] = getItem(isel);
			tmp = getItem(isel);
			int stk = (rand()%4)+1;
//			Inventory[i]->setStock(stk);
			tmp->setStock(stk);
			printf("%s\n", tmp->getName());
//			recieveItem(Inventory, InvNum, tmp);	
			Team.recieveItem(tmp);
		}
		else
		{
			Inventory[i] = NULL;
		}
	}
	printf("\n");
*/
	int sel;
	printf("1. Start New Battle\n2. Load Player Stats\n");
	scanf("%d", &sel);
	if (sel == 2)
	{
		/*
		FILE *fp;
		fp = fopen("./files/Battle_saveFile.txt", "r");
		Characters[0]->setPlayer(fp);
		Characters[1]->setPlayer(fp);
		Characters[2]->setPlayer(fp);
		fclose(fp);
		*/
		Team.loadParty();
		for (int i=0; i<PlayerNum; i++)
   	{
      	delete Characters[i];
  		}
	}
	else
	{
		Characters[0]->setPlayer("Mr. Warriorguy", 0);
		Characters[1]->setPlayer("Senor Tortuga", 1);
		Characters[2]->setPlayer("Miss MagicTits", 2);

		printf("Inventory:\n");
   	for (int i=0; i<InvNum; i++)
		{
			if (i < InvNum)
			{
				item *tmp;
				int isel = (rand()%8)+1;
//				Inventory[i] = getItem(isel);
				tmp = getItem(isel);
				int stk = (rand()%4)+1;
//				Inventory[i]->setStock(stk);
//				tmp->setStock(stk);
				printf("%s\n", tmp->getName());
//				recieveItem(Inventory, InvNum, tmp);   
				Team.recieveItem(tmp);
			}
			else
			{
				Inventory[i] = NULL;
			}
		}
		printf("\n");
	}
	if (sel != 2)
	{
		for (int i=0; i<PlayerNum; i++)
			Team.recievePlayer(Characters[i]);
	}

	printf("Set Enemy Level? (Default is 5)\n1. Yes\n2. No\n");
	scanf("%d", &sel);
	if (sel == 1)
	{
		int temp;
		printf("Enter enemy level: ");
		scanf("%d", &temp);
		if ((temp > 0)&&(temp <= 100))
			EnemyLevel = temp;
		else
			printf("Invalid entry, setting to default level\n");
	}


	BadGuys[0].setEnemy(1, "Evil Frog", 20, 20, 5, 5, 5, 5, 1);
	BadGuys[0].recieveSkill(9);
	BadGuys[0].setLevel(EnemyLevel);
	
	BadGuys[1].setEnemy(2, "Man-eating Goat", 65, 30, 3, 7, 2, 6, 1);
	BadGuys[1].recieveSkill(10);
	BadGuys[1].setLevel(EnemyLevel);

	BadGuys[2].setEnemy(3, "Wolf", 50, 60, 7, 3, 6, 4, 1);
	BadGuys[2].recieveSkill(9);
	BadGuys[2].setLevel(EnemyLevel);

/*
	BadGuys[3].setEnemy(4, "Bar Wench", 100, 80, 4, 4, 4, 4, 2);
	BadGuys[3].recieveSkill(9);
	BadGuys[3].recieveSkill(10);		
*/
	enemy *temp = getEnemy(4);
	BadGuys[3] = *temp;
	BadGuys[3].resetSkillLst();
	BadGuys[3].setLevel(EnemyLevel);
	delete temp;
//	Characters[2].recieveExp(999999999);


/*
	>----Commenting this out so that you can test out the areas deal by itself----<
	
	battle RandomBattle(BadGuys, EnemyNum, Characters, PlayerNum, Inventory, InvNum);
//	RandomBattle.set_enmLvl();
	int Vic = RandomBattle.battleLoop();
	if (Vic == 0)
		RandomBattle.battleAwards();
	else if (Vic == 1)
		printf("GAME OVER!\n");
*/



	areas grass("Grassy Plains", 1, &Team);
   areas snow ("Snow-covered Mountains", 2, &Team);
   areas fire ("Lava Hell-scape", 3, &Team);
   areas random("Random Dungeon", 4, &Team);
   areas rand_alt("Random Dun Alt", 5, &Team);
   areas file_dungeon("File Dungeon", 6, &Team);

   grass.dungeon_loop();
   snow.dungeon_loop();
   fire.dungeon_loop();
   random.dungeon_loop();
   rand_alt.dungeon_loop();
   file_dungeon.dungeon_loop();	




	for (int i=0; i<InvNum; i++)
	{
		if (Inventory != NULL)
			delete Inventory[i];
	}
/*
	for (int i=0; i<PlayerNum; i++)
   {
      delete Characters[i];
   }
*/
	return 0;
}
