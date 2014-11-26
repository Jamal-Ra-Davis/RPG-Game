#include "../headers/questHall.h"
#include "../headers/quest.h"
#include "../headers/party.h"
#include "../headers/Safe_Input.h"
#include "../headers/player.h"
#include "../headers/FileFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

questHall* getQuestHall(int id)
{
    questHall *output = NULL;

    int cnt = lineCount("./files/QuestHall_List.txt");
    item *temp;
    FILE *fp;
    char *line;

    int ID;
    char NAME[32];
    int NUM_QUESTS;
    int *QUESTS;
    int MEMBER_PRICE;

    if ((id > cnt)||(id == 0))
    {
        fprintf(stderr, "Questhall id did not match file's\n");
        return NULL;
    }

    line = getWholeLine("./files/QuestHall_List.txt", id);
    fp = fopen("./files/tempQuestHall.txt", "w");
    
    printf("QuestHall_List.txt line: %s", line);

    fprintf(fp, "%s \n", line);
    fclose(fp);
    fp = fopen("./files/tempQuestHall.txt", "r");
    free(line);

    fscanf(fp, "%d '%[^']' %d %d ", &ID, NAME, &MEMBER_PRICE, &NUM_QUESTS);
    if (ID != id)
        fprintf(stderr, "You dun goofed with the item fetching!\n");
    QUESTS = new int[NUM_QUESTS];
    for (int i=0; i<NUM_QUESTS; i++)
    {
        int temp;
        fscanf(fp, " %d ", &temp);
        QUESTS[i] = temp;
    }
    output = new questHall(ID, NAME, NUM_QUESTS, QUESTS);
    delete[] QUESTS;
    fclose(fp);

    return output;
}

questHall::questHall()
{
    id = 1;
    sprintf(name, "%s", "Quest Hall");
    num_quests = 0;
    member_price = 500;

	quest_list = NULL;
	team = NULL;
}
questHall::~questHall()
{
	for (int i=0; i<num_quests; i++)
	{
		if (quest_list[i] != NULL)
			delete quest_list[i];
	}
	delete [] quest_list;
}
questHall::questHall(int ID, char *NAME, int size)
{
	setQuestHall(ID, NAME, size);
}
questHall::questHall(int ID, char *NAME, int size, int *IDs)
{
	setQuestHall(ID, NAME, size, IDs);
}
void questHall::setQuestHall(int ID, char *NAME, int size)
{
    id = ID;
	team = NULL;
	member_price = 500;
	
	sprintf(name, "%s", NAME);
	num_quests = size;
	
	quest_list = new quest*[num_quests];
	for (int i=0; i<num_quests; i++)
	{
		quest_list[i] = NULL;
		quest_list[i] = getQuest(i+1);
	}
}
void questHall::setQuestHall(int ID, char *NAME, int size, int *IDs)
{
    id = ID;
	team = NULL;
	member_price = 500;

	sprintf(name, "%s", NAME);
	num_quests = size;
	
	quest_list = new quest*[num_quests];
	for (int i=0; i<num_quests; i++)
	{
		quest_list[i] = NULL;
		quest_list[i] = getQuest(IDs[i]);
	}
}
void questHall::printQuests()
{
	printf("%s quests\n", name);
	printf("--------------------------------------------\n");
	printf("ID   Name                  Completed\n");
   printf("===  ====================  ==========\n");
	for (int i=0; i<num_quests; i++)
	{
		printf("%2d.  %-20s  ", i+1, quest_list[i]->getName());
		if (quest_list[i]->isQuestComplete())
			printf("Yes\n");
		else
			printf("No\n");
	}
	printf("\n");
}
/*
void questHall::checkQuest(int sel)
{

}
*/
void questHall::enterQuestHall(party *in)
{
	team = in;
	bool in_hall = true;
	while (in_hall)
	{
		printf("%s\n", name);
		printf("---------------------\n");	
		int hall_sel = getSel("1. View Quest List", "2. Select Quest",
									 "3. Quit Quest", "4. Obtain Quest Rewards",
									 "5. Recruit Party Memeber", "6. Leave Hall");
		switch(hall_sel)
		{
			case 1:
			{
				printQuests();
				break;
			}
			case 2:
			{
				selectQuest();
				break;
			}
			case 3:
			{
				team->discardQuest();
				break;
			}
			case 4:
			{
				if (team->hasQuest())
				{
					quest *activeQuest = team->getActiveQuest();
					printf("Obtain rewards for completing '%s'?\n", activeQuest->getName());
					int sel = getSel("1. Yes", "2. No");
					if (sel == 1)
					{
						activeQuest->checkQuestRequirements(team->getInventory(), team->getInventorySize());
						if (activeQuest->metRequirements())
						{
							int id = activeQuest->getID();
							activeQuest->completedQuest();
							team->recieveQuestRewards();
							for (int i=0; i<num_quests; i++)
							{
								if (id == quest_list[i]->getID())
								{
									quest_list[i]->completedQuest();
								}
							}
						}
						else
							printf("You have not met requirements to complete '%s'...\n",
									 activeQuest->getName());
					}
					else
						printf("Cancelling action...\n");
				}
				else
					printf("You do not have a quest to get rewards from...\n");
				break;
			}
			case 5:
			{
				getNewMember();
				break;
			}
			case 6:
			{
				printf("Leaving %s...\n", name);
				in_hall = false;
				team = NULL;
				break;
			}
		}
	}
}
/*
void questHall::acceptQuests()
{

}
*/
void questHall::getNewMember()
{
	bool newMem = true;
	while (newMem)
	{
		printf("New party member will cost %d gold to recruit...\n", member_price);
		int memSel = getSel("1. Make new player", "2. Make random player", "3. Cancel");
		switch(memSel)
		{
			case 1:
			{
				recruitNewPlayer();
				break;
			}
			case 2:
			{
				if (team->getGold() >= member_price)
				{

				}
				else
					printf("You do not have enough gold to recruit a new party member...\n");
				break;
			}
			case 3:
			{
				newMem = false;
				printf("Cancelling action...\n");
				break;
			}
		}
	}
}
void questHall::recruitNewPlayer()
{
	if (team->getGold() >= member_price)
	{
		player *player_n = getNewPlayer();
		if (player_n != NULL)
		{
			printf("Are you sure you want to add %s to your party?\n", player_n->getName());
			int sel = getSel("1. Yes", "2. No");
			if (sel == 1)
			{
				player *test = team->recievePlayer(player_n);
				if (test == NULL)
				{
					printf("Paid %s %d gold to join your party...\n", 
							 player_n->getName(), member_price);
					team->loseGold(member_price);
				}
				else
				{
					printf("Cancelling transaction...\n");
					delete player_n;
				}
						
			}
			else
			{
				printf("Cancelling action...\n");
				delete player_n;
			}
		}
	}
	else
		printf("You do not have enough gold to recruit a new party member...\n");
}
void questHall::selectQuest()
{
	bool select_quest = true;
	while (select_quest)
	{
		printQuests();
		printf("\n%d. Cancel\n", num_quests+1);
		int quest_sel = getSel(num_quests+1);
		
		if (quest_sel < num_quests+1)
		{
			bool loop = true;
			while(loop)
			{
				int idx = quest_sel-1;
				printf("'%s' Options\n", quest_list[idx]->getName());
				printf("-------------------------\n");
				int opt_sel = getSel("1. Check Quest", "2. Accept Quest",
											"3. Cancel");	
				switch(opt_sel)
				{
					case 1:
					{
						quest_list[idx]->printQuestRequirements();
						quest_list[idx]->printDescription();
						break;
					}
					case 2:
					{
						team->acceptQuest(getQuest(quest_list[idx]->getID()));
						break;
					}
					case 3:
					{
						printf("Cancelling action...\n");
						loop = false;
						break;
					}
				}
			}
		}
		else
		{
			select_quest = false;
			printf("Cancelling action...\n");
		}	
	}	
}
void questHall::saveQuestHall(FILE *fp)
{
    //FILE *fp;
    //printf("Saving quest hall info...\n");
    //fp = fopen("./files/QuestHall_save.txt", "w");
    assert(fp != NULL);
    fprintf(fp, "%d ", member_price);
    //fprintf(fp, "'%s' %d %d ", name, member_price, num_quests);
    for (int i=0; i<num_quests; i++) 
    {
   //     fprintf(fp, "%d ", quest_list[i]->getID());
        if (quest_list[i]->isQuestComplete())
            fprintf(fp, "1 ");
        else
            fprintf(fp, "0 ");
    }
    //fclose(fp);
}
void questHall::loadQuestHall(FILE *fp)
{
    //FILE *fp;
    //printf("Loading quest hall info...\n");
    //fp = fopen("./files/QuestHall_save.txt", "r");
    assert(fp != NULL);
    fscanf(fp, " %d ", &member_price);
    assert(num_quests > 0);
    for (int i=0; i<num_quests; i++)
    {
        int questComplete = 0;
        fscanf(fp, " %d ", &questComplete);
        if (questComplete)
            quest_list[i]->completedQuest();
    }
}



	
