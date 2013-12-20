#pragma once
#include <stdio.h>
#include <stdlib.h>

class player;
class party;

class inn
{
	int cost;
	party *Team;
	player **ply_lst;	
	int num_plys;

	public:
	inn();
	inn(int);
	void setInn(int);
	
	void enterInn(party*);
	

};
