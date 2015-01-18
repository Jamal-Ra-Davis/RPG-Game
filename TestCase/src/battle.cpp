#include "../headers/battle.h"
#include "../headers/player.h"
#include "../headers/enemy.h"
#include "../headers/item.h"
#include "../headers/Safe_Input.h"
#include "../headers/party.h"
#include "../headers/quest.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;



battle::battle(enemy *ENM_IN, int N_ENMS, party *party_in)
{
	enm_lst = ENM_IN;
	num_enms = N_ENMS;

	characters = party_in;

	ply_lst = characters->getPlayers();
	num_plys = characters->getPartySize();

	inv = characters->getInventory();
	num_itms = characters->getInventorySize();

//	enm_lvl = ENM_LEVEL;
	turn_cnt = 1;
	flee_success = false;
}
/*
void battle::set_enmLvl()
{
	for (int i=0; i<num_enms; i++)
	{
		enm_lst[i].setLevel(enm_lvl);
	}
}
*/
int battle::battleLoop()//2. Party Fled, 1. Party Dead; 0. Players Win 
{
	int chk = -1;
	while(1)
	{
		//Display battle info
		printf("\nTurn: %d\n", turn_cnt);
		cout << "Players" << endl;
		cout << "-----------------------------" << endl;
		for (int i=0; i<num_plys; i++)
		{
			char HEALTH[32], MP[32];
			sprintf(HEALTH, "Health: %d/%d", ply_lst[i]->getHealth(),
					  ply_lst[i]->getMax_Health_btl());
			sprintf(MP, "MP: %d/%d", ply_lst[i]->getMP(), ply_lst[i]->getMax_MP_btl());
/*
			printf("%-20s  Health: %4d/%4d  MP: %d/%d\n", ply_lst[i]->getName(), 
			ply_lst[i]->getHealth(), ply_lst[i]->getMax_Health(),
			ply_lst[i]->getMP(), ply_lst[i]->getMax_MP());
*/
			printf("%d. %-26s %-20s %-20s\n", i+1, ply_lst[i]->getName(), HEALTH, MP);
		}
		cout << "\nEnemies" << endl;
		cout << "-----------------------------" << endl;
		for (int i=0; i<num_enms; i++)
		{
			char HEALTH[32], MP[32];
			sprintf(HEALTH, "Health: %d/%d", enm_lst[i].getHealth(),
					  enm_lst[i].getMax_Health());
			sprintf(MP, "MP: %d/%d", enm_lst[i].getMP(), enm_lst[i].getMax_MP());
/*
			printf("%-20s  Health: %4d/%4d  MP: %d/%d\n", enm_lst[i].getName(), 
			enm_lst[i].getHealth(), enm_lst[i].getMax_Health(),
			enm_lst[i].getMP(), enm_lst[i].getMax_MP());
*/
			printf("%d. %-26s %-20s %-20s\n", i+1, enm_lst[i].getName(), HEALTH, MP);
		}
		cout << endl;

		resetPlayers();
		int check = playerPhase();
		if (check != -1)
		{
			chk = check;
			break;
			//return check;
		}

		printf("\n");

		resetEnemies();
		check = enemyPhase();
		if (check != -1)
		{
			chk = check;
			break;
			//return check;
		}

		turn_cnt++;
	}
	checkQuest();
	return chk;
}
int battle::checkVictory()//1 = Players lost, -1 = Neither lost, 0 = Players win
{
	bool PartyAlive = false;
	bool EnmsAlive = false;

	for(int i=0; i<num_plys; i++)
	{
		if (ply_lst[i]->isDead() == false)
			PartyAlive = true;
	}
	for(int i=0; i<num_enms; i++)
	{
		if(enm_lst[i].isDead() == false)
			EnmsAlive = true;
	}

	if (PartyAlive == false)
	{
		printf("You have been defeated...\n");
			return 1;//You lose
	}
	if (EnmsAlive == false)
	{
		printf("Enemies have been defeated. Victory...\n");
			return 0;//Players defeated enemies and won
	}
	if (flee_success)
	{
		printf("Your party fleed from the battle...\n");
		return 2;//flee successful
	}
	return -1;
}
int battle::Attack(int target_def, int source_att)
{
	double precise_damage;

//      printf("Target def: %d, Source att: %d\n", target_def, source_att);
        //Come up with some better battle calculations later
	precise_damage = source_att*((source_att/(double)target_def) +2.25);
	return (int)(precise_damage+0.5);
}
int battle::playerPhase()
{
	int check=-1;
	for (int i=0; i<num_plys; i++)
	{
		if (ply_lst[i]->isDead())
		{
			printf("%s is dead and cannot take action.\n", ply_lst[i]->getName());
			continue;
		}

		printf("%s     Health: %d/%d     MP: %d/%d\n", ply_lst[i]->getName(),
				 ply_lst[i]->getHealth(), ply_lst[i]->getMax_Health_btl(),
				 ply_lst[i]->getMP(), ply_lst[i]->getMax_MP_btl());
		cout << "---------------" << endl;
		int sel;
		sel = getSel("1. Attack", "2. Defend", "3. Use Skill", "4. Use Item",
						 "5. Flee", "6. Do Nothing");
//		printf("1. Attack\n2. Defend\n3. Use Skill\n4. Use Item\n5. Flee\n6. Do Nothing\n");
//		scanf("%d", &sel);
		switch(sel)
		{
			case 1:
			{
				//Attack
				int sel_tar;
				printf("Select Target:\n");
				for (int j=0; j<num_enms; j++)
				{
					// cout << j+1 << ". " << enm_lst[j].getName() << endl;
					printf("%d. %s\n", j+1, enm_lst[j].getName());
				}
				printf("\n%d. Cancel Action\n", num_enms+1);
				sel_tar = getSel(num_enms+1);
//				scanf("%d", &sel_tar);
				if (sel_tar < num_enms+1)
				{
					if (enm_lst[sel_tar-1].takeDamage(Attack(enm_lst[sel_tar-1].getDef_btl(),
															 ply_lst[i]->getAtt_btl())) == 1)
					{
						printf("%s is already dead and cannot be attacked\n",
								 enm_lst[sel_tar-1].getName());
						i--;
					}
				}
				else
				{	
					printf("Cancelling action...\n");
					i--;
				}
				break;
			}
			case 2:
			{
				//Defend
				ply_lst[i]->Defend();
				break;
			}
			case 3:
			{
				//Use special skill
				int skill_sel = 4;
				ply_lst[i]->printSkills();
				printf("%d. Cancel action\n", ply_lst[i]->getSkill_cnt()+1);			
				skill_sel = getSel(ply_lst[i]->getSkill_cnt()+1);

				printf("\n");
//				scanf("%d", &skill_sel);
				if (skill_sel == (ply_lst[i]->getSkill_cnt()+1))
				{
					printf("Cancelling action...\n");
					i--;
				}
				else
				{
					if (ply_lst[i]->skillPass(skill_sel-1))
					{
//						int target;
//						printf("Select target: ");
	//					scanf("%d", &target); 
//						target = getInt();
						if ((skill_sel-1) < ply_lst[i]->getNum_skills())
						{
							int info = ply_lst[i]->skillType(skill_sel-1);
							int target = 0;
							printf("Select target:\n");
							switch (info)
							{
								case 0://target single player
								{
									for (int j=0; j<num_plys; j++)
									{
										printf("%d. %s\n", j+1, ply_lst[j]->getName());
									}
									target = getSel(num_plys);
									break;
								}
								case 2://target single enemy
								{
									for (int j=0; j<num_enms; j++)
                                    {
                                        printf("%d. %s\n", j+1, enm_lst[j].getName());
                                    }
                                    target = getSel(num_enms);
									break;
								}
								default://1. Target all players, 3. Target all enemies
								{
									break;
								}
							}
							ply_lst[i]->useSkillBtl(skill_sel-1, ply_lst, num_plys, enm_lst,
												  	  	  num_enms, i, target-1);
						}
						else
						{
							printf("Cancelling action...\n");
							i--;
						}
					}
                    else//Failed skillPass function
					{
						printf("Invalid skill selection...\n");
						i--;
					}
				}
				break;
			}
			case 4:
			{
				//Use item
				int isel;
				/*
				printf("Items:\n");
				for (int k=0; k<num_itms; k++)
				{
					
					if (inv[k] != NULL)
						printf("%d. %-15s   x%d\n", k+1, inv[k]->getName(), inv[k]->getStock());
					else
						printf("%d. <Empty Slot>\n", k+1);
				}
				*/
				printInventory(inv, num_itms);
				printf("%d. Cancel Action\n", num_itms+1);
				printf("\n");
//				scanf("%d", &isel);
				isel = getSel(num_itms+1);
				if (((isel-1) < num_itms)&&(inv[isel-1] != NULL))
				{
					int info = inv[isel-1]->getType();
					int target = 0;
					bool equipFlag = false;
					printf("Select target: \n");
					switch (info)
					{
						case 0://target single player
						{
							for (int j=0; j<num_plys; j++)
							{
								printf("%d. %s\n", j+1, ply_lst[j]->getName());
							}
							target = getSel(num_plys);
							break;
						}
						case 2://target single enemy
						{
							for (int j=0; j<num_enms; j++)
							{
								printf("%d. %s\n", j+1, enm_lst[j].getName());
							}
							target = getSel(num_enms);
							break;
						}
						case 4:
						{
							equipFlag = true;
							break;
						}
						default://1. Target all players, 3. Target all enemies
						{
							break;
						}
					}

//					scanf("%d", &target);
					printf("%s used %s...\n", ply_lst[i]->getName(), inv[isel-1]->getName());
					inv[isel-1]->use(ply_lst, num_plys, enm_lst, num_enms, target-1);	
					printf("You have %d %s left in inventory\n", inv[isel-1]->getStock(), inv[isel-1]->getName());
					if (inv[isel-1]->getStock() == 0)
					{
						delete inv[isel-1];
						inv[isel-1] = NULL;	
					}
					if (equipFlag)
						i--;
				}
				else
					i--;
				break;
			}
			case 5://flee
			{	
				int flee = rand()%100;
				if (flee > 65)
				{
					printf("Flee successful...\n");
					flee_success = true;
					break;
				}
				else
				{
					printf("You could not flee from battle...\n");
				}
				break;
			}
			case 6:
			{
				//Do nothing
				break;
			}
		}
		//Check victory; return
		check = checkVictory();
		if (check != -1)
			return check;
	}
   return check;
}
int battle::enemyPhase()
{
	int check=-1;
	for (int i=0; i<num_enms; i++)
	{
		if (enm_lst[i].isDead())//If Enemy is dead, skip thier turn
			continue;
		int act_sel = rand()%4;
		
        //No idea what this spc variable is supposed to be used for, this condition NEVER happens
		int spc = rand()%100;
		if (spc > 99)
			act_sel = 2;

		switch(act_sel)
		{
			case 0://attack
			{
				int ply_sel = rand()%num_plys;
				if (!ply_lst[ply_sel]->isDead())
				printf("%s attacked %s\n", enm_lst[i].getName(),
						 ply_lst[ply_sel]->getName());

				if (ply_lst[ply_sel]->takeDamage(Attack(ply_lst[ply_sel]->getDef_btl(),
					 enm_lst[i].getAtt_btl())) == 1)
				{
					i--;
				}
				break;
			}
			case 1://defend
			{
				printf("%s chose to defend...\n", enm_lst[i].getName());
				enm_lst[i].Defend();
				break;
			}
			case 2://special skill
			{
				printf("%s chose to use skill...", enm_lst[i].getName());	
				int skill_sel;
				skill_sel = rand()%enm_lst[i].getNum_skills();
				if (enm_lst[i].skillPass(skill_sel) == true)
				{
					int ply_sel = rand()%num_plys;
					printf(" on %s\n", ply_lst[ply_sel]->getName());
					if (!ply_lst[ply_sel]->isDead())
					{
						enm_lst[i].useSkillBtl(skill_sel, ply_lst, num_plys, enm_lst,
													  num_enms, i, ply_sel); 
					}
					else
						i--;
				}
				else
					i--;
				break;
			}
			case 3://do nothing
			{
				printf("%s chose to do nothing...\n", 
				enm_lst[i].getName());
				break;
			}
		}
		check = checkVictory();
		if (check != -1)
			return check;
	}
	return check;
}
void battle::resetPlayers()
{
	for (int i=0; i<num_plys; i++)
		ply_lst[i]->statusReset();
}
void battle::resetEnemies()
{
	for (int i=0; i<num_enms; i++)
		enm_lst[i].statusReset();
}
void battle::battleAwards()
{
	cout << "Victory!!" << endl << "Recieving battle rewards..." << endl;
	int exp_sum = 0;
	int gold_sum = 0;
	

	for (int i=0; i<num_enms; i++)
	{
		exp_sum += enm_lst[i].getExp();
		gold_sum += enm_lst[i].getGoldVal();
	}

	int alive_cnt = 0;
	for (int i=0; i<num_plys; i++)
	{
		if (!ply_lst[i]->isDead())
			alive_cnt++;
	}

	exp_sum = exp_sum/alive_cnt;
	for (int i=0; i<num_plys; i++)
	{
		ply_lst[i]->recieveExp(exp_sum);
	}

	printf("\nRecived %d gold from defeated enemies...\n\n", gold_sum);
	characters->recieveGold(gold_sum);
/*
	int sel;
	printf("Would you like to save your players' stats?\n");
	printf("1. Yes\n2. No\n");
	scanf("%d", &sel);
	if (sel == 1)
		saveStats();
*/
}
void battle::saveStats()
{
	FILE* fp;
	fp = fopen("./files/Battle_saveFile.txt", "w");
	printf("Saving player information...\n");
	for (int i=0; i<num_plys; i++)
	{
		printf("%s's def: %d\n", ply_lst[i]->getName(), ply_lst[i]->getDef());
		ply_lst[i]->saveInfo(fp);
	}
	fclose(fp);
}
void battle::checkQuest()
{
	quest *qst = characters->getActiveQuest();
	if (qst == NULL)
		return;
	qst->checkQuestRequirements(enm_lst, num_enms);
}





