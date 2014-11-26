#pragma once
#include <stdio.h>
#include <stdlib.h>

class questHall;
class quest;
class party;

questHall *getQuestHall(int);//Needs to be completed in questHall.cpp

class questHall
{
    int id;
	char name[32];
	quest **quest_list;
	int num_quests;
	int member_price;

	party *team;


	public:
		questHall();
		~questHall();

		questHall(int, char*, int);
		questHall(int, char*, int, int*);
		void setQuestHall(int, char*, int);
		void setQuestHall(int, char*, int, int*);	

        int getID()
            {return id;}
        void setID(int ID)
            {id = ID;}
		char* getName()
			{return name;}
        void setName(char* NAME)
            {sprintf(name, "%s", NAME);}
        int getNumQuests()
            {return num_quests;}
        int getMemberPrice()
            {return member_price;}
        void setMemeberPrice(int m_p)
            {member_price = m_p;}
		
		void printQuests();
//		void checkQuest(int);
		void enterQuestHall(party*);
//		void acceptQuests();
		void getNewMember();
		void recruitNewPlayer();
		void selectQuest();

        void saveQuestHall(FILE*);
        void loadQuestHall(FILE*);	
};




