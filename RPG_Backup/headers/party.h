#pragma once
#include <stdio.h>
#include <stdlib.h>

class party;
class player;
class item;
class enemy;
class quest;

class party
{
	protected:
		int gold;

		player **ply_lst;
		int num_plys;
		int Max_members;

		item **inventory;
		int inv_size;

		quest *active_quest;

		bool game_over;

	public:
		party();
		~party();
		int getGold()
			{return gold;}
		player** getPlayers()
			{return ply_lst;}
		int getPartySize()
			{return num_plys;}
		int getMaxPartySize()
			{return Max_members;}
		item** getInventory()
			{return inventory;}
		int getInventorySize()
			{return inv_size;}
		bool isGameOver()
			{return game_over;}
		quest* getActiveQuest()
			{return active_quest;}		


		void recieveGold(int);
		void loseGold(int);
		bool goldCheck(int);

		bool checkGameOver();
	
		player* recievePlayer(player*);
		void compressParty();		
		player* removePlayer(int);
		void deletePlayer(int);

		void printPlayers();
		void printInventory();

		bool inventoryFull();
		bool partyFull();
		bool checkStock(int, int);		
		bool checkItem(int);
		char* getItemName(int);
		int getItemValue(int);

		void recieveItem(item*);
		item* removeItem(int);
		item* removeOneItem(int);
		item* removeItems(int, int);
		void discardItem(int);
		item* swapItem(item*);
		int findItemIndex(int);

		void equipGear(int, int);
		void unequipGear(int, int);

		void useItem(int, int);
//		void useItemBtl(int itm_idx, player **ply_lst, int num_plys, enemy *enm_lst, int num_enms, int targ);
		void viewItemInfo(int);

		void acceptQuest(quest*);
		void discardQuest();
		void deleteQuest();
		quest* swapQuest(quest*);
		void printQuest();
		void recieveQuestRewards();
		bool hasQuest();
		bool metQuestRequirements();
		bool questCompleted();
				


		void managePlayers();
			void manageInventory();
			void manageEquipment(int);
			void manageSkills(int);	
				void useSkill_OB(int);
				void useItem_OB();
				void viewItemInfo_OB();
				player* removePlayer_OB();
				void deletePlayer_OB();

		void partyMenu();
			void printPartyInfo();
			void switchPartyOrder();

		void saveParty();
		void loadParty();
};
