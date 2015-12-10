#include "../headers/quest.h"
#include "../headers/enemy.h"
#include "../headers/item.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


quest* getQuest(int id)
{
	quest *quest_out = NULL;
	switch(id)
	{
		case 1:
		{
			quest_out = new killQuest;
			quest_out->setVals("Temp Quest", 1, 250, 6, 3);
			((killQuest*)quest_out)->setQuest(1, 3);
			break;
		}
		case 2:
		{
			quest_out = new killQuest;
			quest_out->setVals("Wild Wolves", 2, 750, 0, 0);
			((killQuest*)quest_out)->setQuest(3, 5);
			break;
		}
		case 3:
		{
			quest_out = new killQuest;
			quest_out->setVals("Bar Run", 3, 1500, 9, 1);
			((killQuest*)quest_out)->setQuest(4, 10);
			break;
		}
		case 4:
		{
			quest_out = new fetchQuest;
			quest_out->setVals("Pecular Potions", 4, 1000, 0, 0);
			((fetchQuest*)quest_out)->setQuest(2, 3);
			break;
		}
	}
	return quest_out;
}

void quest::setVals(char* N, int ID, int GR, int IR, int IS)
{
	sprintf(name, "%s", N);
	id = ID;
	gold_reward = GR;
	item_reward = IR;
	item_stk = IS;
	reqs_met = false;
	quest_complete = false;
}
item* quest::getItemReward()
{
	if (item_reward == 0)
		return NULL;
	item* reward = getItem(item_reward);
	assert(reward != NULL);
	reward->setStock(item_stk);
	return reward;
}

//--------------------------------------------------------

void killQuest::setQuest(int eID, int KN)
{
	type = 1;
	enemyID = eID;
	killNum = KN;
	kills = 0;
}
void killQuest::checkQuestRequirements(item **inv, int size)
{
	//Does nothing
}
void killQuest::checkQuestRequirements(enemy **enm_lst, int numE)
{

	for (int i=0; i<numE; i++)
	{
		if ((enm_lst[i]->getID() == enemyID)&&(enm_lst[i]->isDead()))
		{
			kills++;		
		}
	}	
	if (kills >= killNum)
	{
		kills = killNum;
		if (!reqs_met)
			printf("Requrements have been met for quest: '%s'\n", name); 
		reqs_met = true;
	}

}
void killQuest::printQuestRequirements()
{
	enemy *temp_enm = getEnemy(enemyID);
	assert(temp_enm != NULL);

	printf("%s (Kill Quest)\n", name);
	printf("===============================\n");
	printf("Target: %s\n", temp_enm->getName());
	delete temp_enm; 
	printf("Kills: %d/%d\n", kills, killNum);
	printf("Reward: %d gold", gold_reward);
	if (item_reward != 0)
	{
		item *temp_itm = getItem(item_reward);
		assert(temp_itm);
		printf(", %d %s(s)", item_stk, temp_itm->getName());
		delete temp_itm;	
	}
	printf("\n");
	printf("Requments Met: ");
	if (reqs_met)
		printf("Yes\n");
	else
		printf("No\n");
	printf("\n");
}
void killQuest::printDescription()
{
	enemy *temp_enm = getEnemy(enemyID);
	assert(temp_enm != NULL);

	printf("'%s' Description:\n", name);
	printf("------------------------\n");
	printf("Find and kill %d %s(s) to complete this quest.\n\n",
			 killNum, temp_enm->getName());
	delete temp_enm;
}
void killQuest::saveQuest(FILE *fp)
{
	assert(fp != NULL);
//	fprintf(fp, "%d ", id); Not necessary
//Caller function saves quest ID
	if (reqs_met)
		fprintf(fp, "1 ");
	else
		fprintf(fp, "0 ");
	if (quest_complete)
		fprintf(fp, "1 ");
	else
		fprintf(fp, "0 ");
	fprintf(fp, "%d \n", kills);	
}
void killQuest::loadQuest(FILE *fp)
{
	assert(fp != NULL);
	int reqs, questCom;
	fscanf(fp, " %d %d %d ", &reqs, &questCom, &kills);
	reqs_met = reqs;
	questCom = quest_complete;
		
}

//----------------------------------------------

void fetchQuest::setQuest(int iID, int IN)
{
	type = 2;
	itemID = iID;
	fetchNum = IN;
	items = 0;
}
void fetchQuest::checkQuestRequirements(item **inv, int size)
{
	int num = 0;
	for (int i=0; i<size; i++)
	{
		if (inv[i] != NULL)
		{
			if (inv[i]->getID() == itemID)
			{
				num += inv[i]->getStock();
			}
		}
	}
	items = num;
	if (items >= fetchNum)
	{
		items = fetchNum;
//		if (!reqs_met)
//			printf("Requrements have been met for quest: '%s'\n", name);
		reqs_met = true;
	}
	else
	{
		reqs_met = false;
	}
}
void fetchQuest::checkQuestRequirements(enemy **enm_lst, int numE)
{
	//Does nothing
}
void fetchQuest::printQuestRequirements()
{
	item *temp_itm = getItem(itemID);
   assert(temp_itm != NULL);

   printf("%s (Fetch Quest)\n", name);
   printf("===============================\n");
   printf("Target: %s\n", temp_itm->getName());
   delete temp_itm;
   printf("Items found: %d/%d\n", items, fetchNum);
   printf("Reward: %d gold", gold_reward);
   if (item_reward != 0)
   {
      temp_itm = getItem(item_reward);
      assert(temp_itm);
      printf(", %d %s(s)", item_stk, temp_itm->getName());
      delete temp_itm;
   }
   printf("\n");
   printf("Requments Met: ");
   if (reqs_met)
      printf("Yes\n");
   else
      printf("No\n");
   printf("\n");
}
void fetchQuest::printDescription()
{
	item *temp_itm = getItem(itemID);
   assert(temp_itm != NULL);

   printf("'%s' Description:\n", name);
   printf("------------------------\n");
   printf("Find and return %d %s(s) to the quest hall to complete this quest.\n\n",
          fetchNum, temp_itm->getName());
   delete temp_itm;
}
void fetchQuest::saveQuest(FILE *fp)
{
	assert(fp != NULL);
// fprintf(fp, "%d ", id); Not necessary
//Caller function saves quest ID
	if (reqs_met)
		fprintf(fp, "1 ");
	else
		fprintf(fp, "0 ");
	if (quest_complete)
		fprintf(fp, "1 ");
	else
		fprintf(fp, "0 ");
	fprintf(fp, "\n");
//	fprintf("%d \n", items); Will use checkQuestRequirements to determine number of items collected
}
void fetchQuest::loadQuest(FILE *fp)
{
	assert(fp != NULL);
	int reqs, questCom;
	fscanf(fp, " %d %d ", &reqs, &questCom);
	reqs_met = reqs;
	questCom = quest_complete;

}


