#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using std::string;

class item;
class quest;
class enemy;

quest* getQuest(int);

class quest
{
	protected:
		char name[32];
		int id;
		int type;
		int gold_reward;
		int item_reward;//make sure item reward is never negative
		int item_stk;

		bool reqs_met;
		bool quest_complete;		

	public:
		void setVals(char* N, int ID, int GR, int IR, int IS);
		char* getName()
			{return name;}
		int getID()
			{return id;}
		int getType()
			{return type;}
		int getGoldReward()
			{return gold_reward;}
		int getItemRewardID()
			{return item_reward;}
		bool metRequirements()
			{return reqs_met;}
		bool isQuestComplete()
			{return quest_complete;}		
		bool hasItemReward()
			{return item_reward;}

		void setQuestComplete(bool QC)
			{quest_complete = QC;}
		void completedQuest()
			{quest_complete = true;}
		item* getItemReward();		

		virtual void checkQuestRequirements(item **inv, int size) =0;
		virtual void checkQuestRequirements(enemy *enm_lst, int numE) =0;
		virtual void printQuestRequirements() =0;
		virtual void printDescription() =0;
		virtual void saveQuest(FILE *fp) =0;
		virtual void loadQuest(FILE *fp) =0;
};

class killQuest : public quest
{
	int enemyID;
	int killNum;
	int kills;

	public:
		void setQuest(int, int);
		void checkQuestRequirements(item **inv, int size);
		void checkQuestRequirements(enemy *enm_lst, int numE);
		void printQuestRequirements();
		void printDescription();
		void saveQuest(FILE *fp);
		void loadQuest(FILE *fp);
};

class fetchQuest : public quest
{
	int itemID;
	int fetchNum;
	int items;
		
	public:
		int getItemID()
			{return itemID;}
		int getFetchNumber()
			{return fetchNum;}
		void setQuest(int, int);
		void checkQuestRequirements(item **inv, int size);
		void checkQuestRequirements(enemy *enm_lst, int numE);
		void printQuestRequirements();
		void printDescription();
		void saveQuest(FILE *fp);
		void loadQuest(FILE *fp);
};
