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



battle::battle(enemy **ENM_IN, int N_ENMS, party *party_in)
{
	enm_lst = ENM_IN;
	num_enms = N_ENMS;

	characters = party_in;

	ply_lst = characters->getPlayers();
	num_plys = characters->getPartySize();

	inv = characters->getInventory();
	num_itms = characters->getInventorySize();

	turn_cnt = 1;
	flee_success = false;
    flee_chance = 65;
}
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
			char HEALTH[32], MP[32], LEVEL[32];
			sprintf(HEALTH, "Health: %d/%d", enm_lst[i]->getHealth(),
					  enm_lst[i]->getMax_Health());
			sprintf(MP, "MP: %d/%d", enm_lst[i]->getMP(), enm_lst[i]->getMax_MP());
            sprintf(LEVEL, "Lvl: %d", enm_lst[i]->getLevel());
/*
			printf("%-20s  Health: %4d/%4d  MP: %d/%d\n", enm_lst[i]->getName(), 
			enm_lst[i]->getHealth(), enm_lst[i]->getMax_Health(),
			enm_lst[i]->getMP(), enm_lst[i]->getMax_MP());
*/
			printf("%d. %-22s %-8s %-20s %-20s\n", i+1, enm_lst[i]->getName(), LEVEL, HEALTH, MP);
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
		if(enm_lst[i]->isDead() == false)
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
		switch(sel)
		{
			case 1:
			{
				//Attack
                int attack_status = player_Attack(i);
                if (attack_status)
                    i--;
				break;
			}
			case 2:
			{
				//Defend
                int defend_status = player_Defend(i);
                if (defend_status)
                    i--;
				break;
			}
			case 3:
			{
				//Use special skill
                int skill_status = player_Skill(i);
                if (skill_status)
                    i--;
				break;
			}
			case 4:
			{
                //Use item
                int item_status = player_Item(i);
                if (item_status)
                    i--;
				break;
			}
			case 5:
			{
                //Flee
                int flee_status = player_Flee(i);
                if (flee_status)
                    i--;
				break;
			}
			case 6:
			{
				//Do nothing
                printf("%s is biding their time...\n", ply_lst[i]->getName());
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

/*
Return values: 0 = No issue continue on, 1 = error happened need to reset
               -1 = User cancelled action need to reset
*/
int battle::player_Attack(int i)
{
    //Get target selection
    int sel_tar;
    printf("Select Target:\n");
    for (int j=0; j<num_enms; j++)
    {
        printf("%d. %s\n", j+1, enm_lst[j]->getName());
    }
    printf("\n%d. Cancel Action\n", num_enms+1);
    sel_tar = getSel(num_enms+1);
    
    if (sel_tar == num_enms+1)
    {   
        //User chose to cancel action
        printf("Cancelling action...\n");
        return -1;
    }
    
    if (enm_lst[sel_tar-1]->isDead())
    {
        //Player tried to attack dead enemy
        printf("%s is already dead and cannot be attacked\n", 
               enm_lst[sel_tar-1]->getName());
        return 1;
    }

    int enemyDefense = enm_lst[sel_tar-1]->getDef_btl();
    int playerAttack = ply_lst[i]->getAtt_btl();
    int damage = Attack(enemyDefense, playerAttack);
    enm_lst[sel_tar-1]->takeDamage(damage);
    return 0;
}
int battle::player_Defend(int i)
{
    ply_lst[i]->Defend();
    return 0;
}
int battle::player_Skill(int i)
{
    //Use special skill
    int skill_sel = 4;
    ply_lst[i]->printSkills();
    printf("%d. Cancel action\n", ply_lst[i]->getSkill_cnt()+1);
    skill_sel = getSel(ply_lst[i]->getSkill_cnt()+1);

    printf("\n");
    if (skill_sel == (ply_lst[i]->getSkill_cnt()+1))
    {
        //Player cancelled action
        printf("Cancelling action...\n");
        return -1;
    }
    if (!ply_lst[i]->skillPass(skill_sel-1))
    {
        //Failed skillPass function
        printf("Invalid skill selection...\n");
        return 1;
    }
    
    if (!ply_lst[i]->hasSkillMP(skill_sel-1))
    {
        //Player didn't have enough MP for skill
        printf("%s does not have enough MP to use %s...\n", ply_lst[i]->getName(),
               ply_lst[i]->getBattleSkillName(skill_sel-1));
        return 1;
    }
    
 
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
            printf("%d. Cancel target selection\n", num_plys+1);
            target = getSel(num_plys+1);
    
            if (target >= num_plys+1)
            {
                printf("Cancelling target selection...\n");
                return -1;
            }
            break;
        }
        case 2://target single enemy
        {
            for (int j=0; j<num_enms; j++)
            {
                printf("%d. %s\n", j+1, enm_lst[j]->getName());
            }   
            printf("%d. Cancel target selection\n", num_enms+1);
            target = getSel(num_enms+1);

            if (target >= num_enms+1)
            {
                printf("Cancelling target selection...\n");
                return -1;
            }

            if (enm_lst[target-1]->isDead())
            {
                //Tried to use skill on dead enemy
                printf("%s is already dead and cannot be attacked\n",
                        enm_lst[target-1]->getName());
                return 1;
            }
            break;
        }
        case 3://Target all enemies
        {
            printf("Attack all enemies with %s?\n", ply_lst[i]->getBattleSkillName(skill_sel-1));
            int selection = getSel("1. Yes", "2. No");
            if (selection != 1)
                return -1;
            break;
        }
        default://1. Target all players, 3. Target all enemies
        {
            break;
        }
    }

    ply_lst[i]->useSkillBtl(skill_sel-1, ply_lst, num_plys, enm_lst,
                            num_enms, i, target-1);
    return 0;
}
int battle::player_Item(int i)
{
    int isel;
    printInventory(inv, num_itms);
    printf("%d. Cancel Action\n", num_itms+1);
    isel = getSel(num_itms+1);
    if ((isel-1) >= num_itms)
    {
        //Cancelled action
        printf("Cancelling action...\n");
        return -1;
    }
    if (inv[isel-1] == NULL)
    {
        printf("You did not select an item...\n");
        return 1;
    }


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
            printf("%d. Cancel target selection\n", num_plys+1);
            target = getSel(num_plys+1);
            //target = getSel(num_plys);

            if (target >= num_plys+1)
            {
                printf("Cancelling target selection...\n");
                return -1;
            }
            break;
        }
        case 2://target single enemy
        {
            for (int j=0; j<num_enms; j++)
            {
                printf("%d. %s\n", j+1, enm_lst[j]->getName());
            }
            printf("%d. Cancel target selection\n", num_enms+1);
            target = getSel(num_enms+1);
            //target = getSel(num_enms);
           
            if (target >= num_enms+1)
            {
                printf("Cancelling target selection...\n");
                return -1;
            }
 
            if (enm_lst[target-1]->isDead())
            {
                //Tried to use skill on dead enemy
                printf("%s is already dead and cannot be attacked\n",
                        enm_lst[target-1]->getName());
                return 1;
            }
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

    printf("%s used %s...\n", ply_lst[i]->getName(), inv[isel-1]->getName());
    inv[isel-1]->use(ply_lst, num_plys, enm_lst, num_enms, target-1);
    printf("You have %d %s left in inventory\n", inv[isel-1]->getStock(), inv[isel-1]->getName());
    if (inv[isel-1]->getStock() == 0)
    {
        delete inv[isel-1];
        inv[isel-1] = NULL;
    }
    if (equipFlag)
    {
        //Tried to use a piece of equipment
        return 1;
    }

    return 0;
}
int battle::player_Flee(int i)
{
    int flee = rand()%100;
    if (flee < flee_chance)//This has been revised to make flee_chance the percentage chance to flee
    {
        printf("Flee successful...\n");
        flee_success = true;
        //break;
    }
    else
        printf("You could not flee from battle...\n");

    return 0;
}
int battle::enemyPhase()
{
	int check=-1;
	for (int i=0; i<num_enms; i++)
	{
		if (enm_lst[i]->isDead())//If Enemy is dead, skip thier turn
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
                int attack_status = enemy_Attack(i);
                if (attack_status)
                    i--;
				break;
			}
			case 1://defend
			{
                int defend_status = enemy_Defend(i);
                if (defend_status)
                    i--;
				break;
			}
			case 2://special skill
			{
                int skill_status = enemy_Skill(i);
                if (skill_status)
                    i--;
				break;
			}
			case 3://do nothing
			{
				printf("%s chose to do nothing...\n", 
				       enm_lst[i]->getName());
				break;
			}
		}
		check = checkVictory();
		if (check != -1)
			return check;
	}
	return check;
}
int battle::enemy_Attack(int i)
{
    int ply_sel = rand()%num_plys;
    if (ply_lst[ply_sel]->isDead())
    {
        //Enemy tried to attack a player that is already dead
        return 1;
    }
    
    printf("%s attacked %s\n", enm_lst[i]->getName(), ply_lst[ply_sel]->getName());
    
    int playerDefense = ply_lst[ply_sel]->getDef_btl();
    int enemyAttack = enm_lst[i]->getAtt_btl();
    int damage = Attack(playerDefense, enemyAttack);
    ply_lst[ply_sel]->takeDamage(damage);

    return 0;
}
int battle::enemy_Defend(int i)
{
    printf("%s chose to defend...\n", enm_lst[i]->getName());
    enm_lst[i]->Defend();
    return 0;
}
int battle::enemy_Skill(int i)
{
    int skill_sel;
    skill_sel = rand()%enm_lst[i]->getNum_skills();
    if (!enm_lst[i]->skillPass(skill_sel))
    {
        //Enemy did not select valid skill
        return 1;
    }
    if (!enm_lst[i]->hasSkillMP(skill_sel))
    {
        //Enemy didn't have enough MP for skill
        return 1;
    }

    int ply_sel = rand()%num_plys;
    if (ply_lst[ply_sel]->isDead())
    {
        //Enemy tried to attack dead player
        return 1;
    }    

    printf("%s chose to use skill on %s...\n", enm_lst[i]->getName(), ply_lst[ply_sel]->getName());
    enm_lst[i]->useSkillBtl(skill_sel, ply_lst, num_plys, enm_lst,
                           num_enms, i, ply_sel);
}

void battle::resetPlayers()
{
	for (int i=0; i<num_plys; i++)
		ply_lst[i]->statusReset();
}
void battle::resetEnemies()
{
	for (int i=0; i<num_enms; i++)
		enm_lst[i]->statusReset();
}
void battle::battleAwards()
{
	cout << "Victory!!" << endl << "Recieving battle rewards..." << endl;
	int exp_sum = 0;
	int gold_sum = 0;
	

	for (int i=0; i<num_enms; i++)
	{
		exp_sum += enm_lst[i]->getExp();
		gold_sum += enm_lst[i]->getGoldVal();
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





