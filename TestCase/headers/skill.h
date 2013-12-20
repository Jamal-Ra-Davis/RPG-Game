#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using std::string;

class skill;
class player;
class enemy;

skill* getSkill(int id);


class skill
{
	protected:
		char name[32];
		int mp_cost;
		int id;
		int type;
		skill *next;
	public:
		void set_vals(char*, int, int, int);
		char* get_name()
			{return name;}
		int get_MPcost()
			{return mp_cost;}
		int getID()
			{return id;}	
		int getType()
			{return type;}
		skill* getNext()
			{return next;}
		void setNext(skill* in)
			{next = in;}
		//Overload this for in and out of battle, difference being the enemy info
		virtual void use(player **ply_lst, int numP, int caster, int targ) =0;
		virtual void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
							  int caster, int targ) =0;
		virtual void printInfo() =0;
};
class HealSpell: public skill
{
	int heal_pts;

	public:
		void set_pts(int);
		void use(player **ply_lst, int numP, int caster, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo();
};
class AttSpell: public skill
{
//Only works when it has all the enemy info, print error when using outside of btl
	int att_pts;

	public:
		void set_pts(int);
      void use(player **ply_lst, int numP, int caster, int targ);
      void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo();
};
class AttSpellAll: public skill
{
	int att_pts;

	public:
		void set_pts(int);
		void use(player **ply_lst, int numP, int caster, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo();
};
class RevSpell: public skill
{
	int restore_pts;

	public:
		void set_pts(int);
		void use(player **ply_lst, int numP, int caster, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo();
};
class MpDrain: public skill
{
	int drain_pts;

	public:
		void set_pts(int);
		void use(player **ply_lst, int numP, int caster, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo();
};
class MpGift: public skill
{
	int gift_pts;
	
	public:
		void set_pts(int);
		void use(player **ply_lst, int numP, int caster, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo();
};
class PhySkill: public skill
{
	int phys_pts;

	public:
		void set_pts(int);
		void use(player **ply_lst, int numP, int caster, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo();
};

//----------------------------------------------------------------------------------

class E_AttSpell: public skill
{
	int att_pts;

	public:
		void set_pts(int);
		void use(player **ply_lst, int numP, int caster, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo() {};
};
class E_AttSpellAll: public skill
{
	int att_pts;

	public:
		void set_pts(int);
		void use(player **ply_lst, int numP, int caster, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo() {};
};

class E_CripplingShot: public skill
{
	
	public:
		void use(player **ply_lst, int numP, int caster, int targ);
		void use(player **ply_lst, int numP, enemy *enm_lst, int numE,
					int caster, int targ);
		void printInfo() {};
};






