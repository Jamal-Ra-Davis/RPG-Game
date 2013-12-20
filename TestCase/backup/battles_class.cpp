#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

class player
{
	char name[32];
	int MHealth;
	int Health;
	int att;
	int def;
	int level;
	int exp;
	bool dead;
	int exp_req[99];

	int att_btl, def_btl;
	
	public:
		player(){};
		player(char*, int);
		player(char*, int, int, int);
		void setPlayer(char*, int);
		void setPlayer(char*, int, int, int);
		void setPlayer(FILE*);

		char* getName();
		int getHealth();
		int getMHealth();
                int getAtt();
		int getAtt_btl();
                int getDef();
		int getDef_btl();
                bool isDead();//Check if health is < 0, sets and returns death status

                int takeDamage(int);//Returns 1 if you try to damage when dead, 0 if things work right
                int removeDamage(int);//Returns 1 if you try to heal when dead, 0 if things work right
		void Defend();
		void statusReset();

		int recieveExp(int);//Returns 1 if exp recieved when dead, 0 if successfully got exp
			void setExp();
			void levelUp();
			void statUp(int);

		void saveInfo(FILE*);
};
player::player(char* NAME, int CLASS)
{
	sprintf(name, "%s", NAME);
	dead = false;
	level = 1;
        exp = 0;
	//Default warrior
	MHealth = 50;
	att = 6;
	def = 4;
	if (CLASS == 1)//Defender
	{
		MHealth = 50;
		att = 4;
		def = 6;
	}
	else if (CLASS == 2)//Mage
	{
		MHealth = 70;
		att = 4;
		def = 4;
	}
	
	
	Health = MHealth;
	setExp();
}
player::player(char* NAME, int MAX_HEALTH, int ATT, int DEF)
{
	sprintf(name, "%s", NAME);
	dead = false;
	level = 1;
        exp = 0;
	MHealth = MAX_HEALTH;
        att = ATT;
        def = DEF;
	Health = MHealth;
	
	att_btl = att;
	def_btl = def;
	setExp();
}
void player::setPlayer(char* NAME, int CLASS)
{
	sprintf(name, "%s", NAME);
        dead = false;
	level = 1;
        exp = 0;
        //Default warrior
        MHealth = 50;
        att = 6;
        def = 4;
        if (CLASS == 1)//Defender
        {
                MHealth = 50;
                att = 4;
                def = 6;
        }
        else if (CLASS == 2)//Mage
        {
                MHealth = 70;
                att = 4;
                def = 4;
        }

	att_btl = att;
        def_btl = def;
        Health = MHealth;
	setExp();
}
void player::setPlayer(char* NAME, int MAX_HEALTH, int ATT, int DEF)
{
	sprintf(name, "%s", NAME);
        dead = false;
	level = 1;
        exp = 0;
        MHealth = MAX_HEALTH;
        att = ATT;
        def = DEF;
        Health = MHealth;

	att_btl = att;
        def_btl = def;
	setExp();
}
void player::setPlayer(FILE* FP)
{
	int temp;
	fscanf(FP, " '%[^']' %d %d %d %d %d %d ", name, &temp, &level, &exp, &MHealth, &att, &def);
	dead = temp;
	Health = MHealth;
	att_btl = att;
	def_btl = def;
	setExp();
}
char* player::getName()
{
	return name;
}
int player::getHealth()
{
	return Health;
}
int player::getMHealth()
{
	return MHealth;
}
int player::getAtt()
{
	return att;
}
int player::getAtt_btl()
{
	return att_btl;
}
int player::getDef()
{
	return def;
}
int player::getDef_btl()
{
	return def_btl;
}
bool player::isDead()
{
	if (Health <= 0)
        {
                Health = 0;
                dead = true;
        }
        return dead;
}
int player::takeDamage(int damage)
{
	if (dead)
		return 1;
	if (damage >= Health)
		printf("%s took %d damage.\n%s is dead.\n", name, Health, name);
	Health -= damage;
	isDead();
	if (Health > 0)
		printf("%s took %d damage.\n", name, damage);
	return 0;
}
int player::removeDamage(int heal_pts)
{
	if (dead)
		return 1;
	Health += heal_pts;
	if(Health >= MHealth)
		Health = MHealth;
	return 0;
}
void player::Defend()
{
	def_btl = 2*def;
	printf("%s's defense doubled for a turn\n", name);
}
void player::statusReset()
{
	def_btl = def;
	att_btl = att;
}
int player::recieveExp(int exp_pts)
{
	if (dead)
	{
		printf("%s is dead, and recieved no experience.\n", name);
		return 1;
	}
	printf("%s recieved %d experience.\n", name, exp_pts);
	exp += exp_pts;
	levelUp();
	return 0;
}
void player::setExp()
{
	int levelExp;
	levelExp = 300;
	exp_req[0] = 300;

	for (int i=1; i<99; i++)
	{
		levelExp = (int)(levelExp*1.1);
		exp_req[i] = exp_req[i-1] + levelExp;
	}
}
void player::levelUp()
{
	int temp_level = 1;
//	int exp_req[99];
//	exp_req[0] = 300;
//	for (int i=1; i<99; i++)
//	{
//		exp_req[i] = (int)(exp_req[i-1]*1.1)+exp_req[i-1];
//	}	
	int i;	
	for (i=0; i<99; i++)
	{
		if (exp < exp_req[i])
			break;
	}
	temp_level = i+1;
/*
	if (exp >= 300)
		temp_level = 2;
	if (exp >= 500)
		temp_level = 3;
	if (exp >= 700)
		temp_level = 4;
	if (exp >= 900)
		temp_level = 5;
	if (exp >= 1500)
		temp_level = 6; 

*/	
	if (temp_level > level)
	{
		printf("Level up! %s's level has increased by %d.\n", name, temp_level - level);
		statUp(temp_level - level);
		level = temp_level;
	}
}
void player::statUp(int num_lvl)
{
	for (int i=0; i<num_lvl; i++)
	{
		int att_buff, def_buff, Health_buff;
		att_buff = rand()%2;
		def_buff = rand()%2;
		Health_buff = (int)(MHealth*(.1 + (rand()%3 - 1)*.025));
		
		if ((att_buff+def_buff+Health_buff) <= 0)
		{
			i--;
		}
		else
		{
			if (att_buff > 0)
			{
				printf("Attack has increased by %d. Attack is now %d.\n", att_buff, att_buff+att);
				att += att_buff;
			}
			if (def_buff > 0)
                        {
				printf("Defense has increased by %d. Defense is now %d.\n", def_buff, def_buff+def);
                                def += def_buff;
                        }
			if (Health_buff > 0)
                        {
				printf("Max Health has increased by %d. Max Health is now %d.\n", Health_buff, Health_buff+MHealth);
                                MHealth += Health_buff;
                        }
			
		}
	}

	Health = MHealth;	
	
}
void player::saveInfo(FILE* FP)
{
	fprintf(FP, "'%s' %d %d %d %d %d %d \n", name, dead, level, exp, MHealth, att, def);
/*
	sprintf(name, "%s", NAME);
        dead = false;
        level = 1;
        exp = 0;
        MHealth = MAX_HEALTH;
        att = ATT;
        def = DEF;
        Health = MHealth;

        att_btl = att;
        def_btl = def;
*/
}

class enemy
{
	char name[32];
	//Base Stats
	int MHealth_b;
	int att_b;
	int def_b;
	//Regular Stats
	int MHealth;
	int Health;
	int att;
	int def;
	int Level;
	bool dead;
	int exp;

	int att_btl, def_btl;
	public:
		enemy(){};
		enemy(char* NAME, int MH_B, int ATT_B, int DEF_B);
		void setEnemy(char* NAME, int MH_B, int ATT_B, int DEF_B);
		void setLevel(int LEVEL);

		char* getName();
		int getHealth();
		int getMHealth();
		int getAtt();
		int getAtt_btl();
		int getDef();
		int getDef_btl();
		bool isDead();//Check if health is < 0, sets and returns death status

		int takeDamage(int);//Returns 1 if you try to damage when dead, 0 if things work right
		int removeDamage(int);//Returns 1 if you try to heal when dead, 0 if things work right
		void Defend();
		void statusReset();

		int getExp();
	

};
enemy::enemy(char* NAME, int MH_B, int ATT_B, int DEF_B)
{
	sprintf(name, "%s", NAME);
	dead = false;
	MHealth_b = MH_B;
	att_b = ATT_B;
	def_b = DEF_B;
}
void enemy::setEnemy(char* NAME, int MH_B, int ATT_B, int DEF_B)
{
	sprintf(name, "%s", NAME);
	dead = false;
        MHealth_b = MH_B;
        att_b = ATT_B;
        def_b = DEF_B;
}
void enemy::setLevel(int LEVEL)
{
	Level = LEVEL;
	//Scale stats according to level
		//Currently placeholder, put better equation in later
	att = (int)(att_b*(1.5*Level));
	def = (int)(def_b*(1.5*Level));

	att_btl = att;
	def_btl = def;	

	MHealth = (int)(MHealth_b*(1.5*Level));
	Health = MHealth;
	exp = (Level*50) + (int)(MHealth*.2) + (int)((att+def)*5);
}
char* enemy::getName()
{
	return name;
}
int enemy::getHealth()
{
	return Health;
}
int enemy::getMHealth()
{
	return MHealth;
}
int enemy::getAtt()
{
	return att;
}
int enemy::getAtt_btl()
{
	return att_btl;
}
int enemy::getDef()
{
	return def;
}
int enemy::getDef_btl()
{
	return def_btl;
}
bool enemy::isDead()
{
	if (Health <= 0)
	{
		Health = 0;
		dead = true;
	}
	return dead;
}
int enemy::takeDamage(int damage)
{
	if(dead)
		return 1;
	if (damage >= Health)
		printf("%s took %d damage.\n%s is dead.\n", name, Health, name); 
	Health -= damage;
	isDead();
	if (Health > 0)
		printf("%s took %d damage.\n", name, damage);
	return 0;
}
int enemy::removeDamage(int heal_pts)
{
	if(dead)
		return 1;
	Health += heal_pts;
	if (Health >= MHealth)
		Health = MHealth;
	return 0;
}
void enemy::Defend()
{
        def_btl = 2*def;
        printf("%s's defense doubled for a turn\n", name);
}
void enemy::statusReset()
{
        def_btl = def;
        att_btl = att;
}
int enemy::getExp()
{
	return exp;	
}

class battle
{
	enemy *enm_lst;
	int num_enms;
	player *ply_lst;
	int num_plys;
	int enm_lvl;	
	int turn_cnt;	

	public:
	battle(enemy*, int, int, player*, int);
	void set_enmLvl();
	int battleLoop();
		int checkVictory();
		int Attack(int, int);
		int playerPhase();
		int enemyPhase();
		void resetPlayers();
		void resetEnemies();
	void battleAwards();
		void saveStats();

};
battle::battle(enemy* ENM_IN,int N_ENMS,int ENM_LEVEL,player* PLAYERS, int N_PLYS)
{
	enm_lst = ENM_IN;
//	for (int i=0; i<N_EMNS; i++)
//	{
//		enm_list[i] = ENM_IN[i];
//	}
	num_enms = N_ENMS;

	ply_lst = PLAYERS;
//	for (int i=0; i<N_PLYS; i++)
//	{
//		ply_lst[i] = PLAYERS[i];
//	}
	num_plys = N_PLYS;

	enm_lvl = ENM_LEVEL;
	turn_cnt = 1;
}
void battle::set_enmLvl()
{
	for (int i=0; i<num_enms; i++)
	{
		enm_lst[i].setLevel(enm_lvl);
	}	
}
int battle::battleLoop()//1. Party Dead; 0. Players Win 
{
	while(1)
	{
		//Display battle info
		printf("\nTurn: %d\n", turn_cnt);
		cout << "Players" << endl;
		cout << "-----------------------------" << endl;
		for (int i=0; i<num_plys; i++)
		{
			printf("%-20s  Health: %d/%d\n", ply_lst[i].getName(), ply_lst[i].getHealth(), ply_lst[i].getMHealth());
		}
		cout << "\nEnemies" << endl;
                cout << "-----------------------------" << endl;
		for (int i=0; i<num_enms; i++)
                {
			printf("%-20s  Health: %d/%d\n", enm_lst[i].getName(), enm_lst[i].getHealth(), enm_lst[i].getMHealth());
                }
		cout << endl;


		resetPlayers();
		int check = playerPhase();
		if (check != -1)
			return check;

		printf("\n");	

		resetEnemies();
		check = enemyPhase();
		if (check != -1)
			return check;

		turn_cnt++;
	}
}
int battle::checkVictory()//1 = Players lost, -1 = Neither lost, 0 = Players win
{
	bool PartyAlive = false;
	bool EnmsAlive = false;

	for(int i=0; i<num_plys; i++)
        {
                if(ply_lst[i].isDead() == false)
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
	return -1;
}
int battle::Attack(int target_def, int source_att)
{
        double precise_damage;
	
//	printf("Target def: %d, Source att: %d\n", target_def, source_att);
        //Come up with some better battle calculations later
        precise_damage = source_att*((source_att/(double)target_def) +2.25);
        return (int)(precise_damage+0.5);
}
int battle::playerPhase()
{
	int check=-1;
        for (int i=0; i<num_plys; i++)
        {
		if (ply_lst[i].isDead()){
//			cout << ply_lst[i].getName() << " is dead and cannot take action." << endl;
			printf("%s is dead and cannot take action.\n", ply_lst[i].getName());
			continue;
		}
		
//		cout << ply_lst[i].getName() << endl;
		printf("%s\n", ply_lst[i].getName());
		cout << "---------------" << endl;
                int sel;
                printf("1. Attack\n2. Defend\n3. Do Nothing\n");
                scanf("%d", &sel);
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
                                scanf("%d", &sel_tar);

                                if (enm_lst[sel_tar-1].takeDamage(Attack(enm_lst[sel_tar-1].getDef_btl(), ply_lst[i].getAtt_btl())) == 1)
				{
					printf("%s is already dead and cannot be attacked\n", enm_lst[sel_tar-1].getName());
					//cout << enm_lst[sel_tar-1].getName() << " is already dead, and cannot be attacked." << endl;
					i--;
				}
                                break;
                        }
                        case 2:
                        {
                                //Defend
				ply_lst[i].Defend();
                                break;
                        }
                        case 3:
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
		if (enm_lst[i].isDead())
			continue;
		int act_sel = rand()%3;
		switch(act_sel)
		{
			case 0://attack
			{
				int ply_sel = rand()%num_plys;
				if (!ply_lst[ply_sel].isDead())
					printf("%s attacked %s\n", enm_lst[i].getName(), ply_lst[ply_sel].getName());
				//	cout << enm_lst[i].getName() << " attacked " << ply_lst[ply_sel].getName() << endl;
				if (ply_lst[ply_sel].takeDamage(Attack(ply_lst[ply_sel].getDef_btl(), enm_lst[i].getAtt_btl())) == 1)
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
			case 2://do nothing
			{
				printf("%s chose to do nothing...\n", enm_lst[i].getName()); 
			//	cout << enm_lst[i].getName() << " chose to do nothing..." << endl;
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
		ply_lst[i].statusReset();
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
	for (int i=0; i<num_enms; i++)
		exp_sum += enm_lst[i].getExp();

	for (int i=0; i<num_plys; i++)
	{
		ply_lst[i].recieveExp(exp_sum);
	}

	int sel;
	printf("Would you like to save your players' stats?\n");
	printf("1. Yes\n2. No\n");
	scanf("%d", &sel);
	if (sel == 1)
		saveStats();
}
void battle::saveStats()
{
	FILE* fp;
	fp = fopen("Battle_saveFile.txt", "w");
	printf("Saving player information...\n");
	for (int i=0; i<num_plys; i++)
	{
		printf("%s's def: %d\n", ply_lst[i].getName(), ply_lst[i].getDef());
		ply_lst[i].saveInfo(fp);
		
	}
	fclose(fp);
}

int main()
{
	player Characters[3];
	enemy BadGuys[4];
	
	int sel;
	printf("1. Start New Battle\n2. Load Player Stats\n");
	scanf("%d", &sel);
	if (sel == 2)
	{
		FILE *fp;
		fp = fopen("Battle_saveFile.txt", "r");
		Characters[0].setPlayer(fp);
                Characters[1].setPlayer(fp);
                Characters[2].setPlayer(fp);
		fclose(fp);
	}
	else
	{
		Characters[0].setPlayer("Mr. Warriorguy", 0);
		Characters[1].setPlayer("Senor Toruga", 0);
		Characters[2].setPlayer("Miss MagicTits", 2);
	}

	BadGuys[0].setEnemy("Evil Frog", 20, 5, 5);
	BadGuys[1].setEnemy("Man-eating Goat", 65, 3, 7);
	BadGuys[2].setEnemy("Wolf", 50, 7, 3);
	BadGuys[3].setEnemy("Bar Wench", 100, 4, 4);

	battle RandomBattle(BadGuys, 4, 1, Characters, 3);
	RandomBattle.set_enmLvl();
	int Vic = RandomBattle.battleLoop();
	if (Vic == 0)
		RandomBattle.battleAwards();
	else if (Vic == 1)
		printf("GAME OVER!\n");

	return 0;
}
