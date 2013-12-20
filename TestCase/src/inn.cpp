#include <stdio.h>
#include <stdlib.h>
#include "../headers/inn.h"
#include "../headers/player.h"
#include "../headers/party.h"
#include "../headers/Safe_Input.h"

inn::inn()
{
	Team = NULL;
	ply_lst = NULL;
	num_plys = 0;
}
inn::inn(int A)
{
	setInn(A);
}
void inn::setInn(int A)
{
	cost = A;
	Team = NULL;
	ply_lst = NULL;
	num_plys = 0;
}
void inn::enterInn(party *P_in)
{
	Team = P_in;
	ply_lst = Team->getPlayers();
	num_plys = Team->getPartySize();

	bool inn_loop = true;
	while (inn_loop)
	{
		printf("Welcome to the inn...\n");
		int sel = getSel("1. Spend the night", "2. Leave");
		if (sel == 1)
		{
			printf("It will cost %d gold, is this ok?\n", cost);
			int confirm = getSel("1. Yes", "2. No");
			if (confirm == 1)
			{
				if (Team->goldCheck(cost))
				{
					Team->loseGold(cost);
					for (int i=0; i<num_plys; i++)
					{
						if (ply_lst[i]->isDead())
						{
							ply_lst[i]->revive(ply_lst[i]->getMax_Health_btl());
						}
						else
						{
							ply_lst[i]->restoreHealth(ply_lst[i]->getMax_Health_btl());
						}
					
						ply_lst[i]->restoreMP(ply_lst[i]->getMax_MP_btl());
					}

					printf("Your party's Health and MP has been restored...\n");
				}
				else
				{
					printf("You do not have enough gold...\n");
				}	
			}
			else
			{
				printf("Cancelling action...\n");
			}
		}
		else
		{
			printf("Come again soon...\n");
			inn_loop = false;
		}
	}
}
