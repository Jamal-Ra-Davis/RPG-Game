#include "../headers/Safe_Input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <limits>
#include <string>
using namespace std;

char* getString()
{
//	fflush(stdin);
	char *out = (char*)malloc(128*sizeof(char));
//	scanf("%[^\n]", test);
//	fgets(test, 128*sizeof(char), stdin);
//	printf("!! %s !!\n", test);
	string test;
//	cin.getline(test, 128);
	getline(cin, test);	
//	string x;
//	cin.clear();
//	cin >> noskipws >> x;

//	std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

//		cin.ignore();
//		getline(cin, x);	
//	cin.getline(test,100);
//	cout << "Cout: " << x << endl;
//	sprintf(test, "%s", x.c_str());
	sprintf(out, "%s", test.c_str());
	return out;
}

//---------------------------------------------------------

int getInt()
{
/*
   int val;
   char *dummy = (char*)malloc(100*sizeof(char));
   int test;

   scanf("%s", dummy);
   test = sscanf(dummy, "%d", &val);

   while (test != 1)
   {  
      printf("Invalid entry, try again.\n");
      scanf("%s", dummy);
      test = sscanf(dummy, "%d", &val);
   }
   free(dummy);

   return val;
*/

	int val;
	string t;
	bool isNumber = false;
	while(!isNumber)
	{
		getline(cin, t);
		isNumber = true;
		for(int x=0; x<t.length(); x++)
		{
			if(!isdigit(t[x]))
			{
				isNumber = false;
				printf("Invalid entry, try again.\n");
			}
		}
	}
	stringstream ss;
	ss << t;
	ss >> val;
	return val;	
}

//---------------------------------------------------------

int getSel(int limit)
{
   int val;
   while (1)
   { 
      val = getInt();
      if ((val >= 1)&&(val <= limit))
         break;
      else
         printf("Invalid entry, try again.\n");
   }
   return val;
}
int getSel(char *s1, int limit)
{
	int val;
   while (1)
   {
      printf("%s\n", s1);
      val = getInt();
      if ((val >= 1)&&(val <= limit))
         break;
      else
         printf("Invalid entry, try again.\n");
   }
   return val;
}
int getSel(char *s1, char *s2)
{
   int val;
   while (1)
   {
      printf("%s\n%s\n", s1, s2);
      val = getInt();
      if ((val >= 1)&&(val <= 2))
         break;
      else
         printf("Invalid entry, try again.\n");
   }
   return val;
}
int getSel(char *s1, char *s2, char *s3)
{
   int val;
   while (1)
   {
      printf("%s\n%s\n%s\n", s1, s2, s3);
      val = getInt();
      if ((val >= 1)&&(val <= 3))
         break;
      else
         printf("Invalid entry, try again.\n");
   }
   return val;
}
int getSel(char *s1, char *s2, char *s3, char *s4)
{
   int val;
   while (1)
   { 
      printf("%s\n%s\n%s\n%s\n", s1, s2, s3, s4);
      val = getInt();
      if ((val >= 1)&&(val <= 4))
         break;
      else
         printf("Invalid entry, try again.\n");
   }
   return val;
}
int getSel(char *s1, char *s2, char *s3, char *s4, char *s5)
{
   int val;
   while (1)
   {    
      printf("%s\n%s\n%s\n%s\n%s\n", s1, s2, s3, s4, s5);
      val = getInt();
      if ((val >= 1)&&(val <= 5))
         break; 
      else      
         printf("Invalid entry, try again.\n");
   }            
   return val;  
}
int getSel(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6)
{
   int val;
   while (1)
   { 
      printf("%s\n%s\n%s\n%s\n%s\n%s\n", s1, s2, s3, s4, s5, s6);
      val = getInt();
      if ((val >= 1)&&(val <= 6))
         break;
      else
         printf("Invalid entry, try again.\n");
   }
   return val;
}
int getSel(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7)
{
   int val;
   while (1)
   { 
      printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n", s1, s2, s3, s4, s5, s6, s7);
      val = getInt();
      if ((val >= 1)&&(val <= 7))
         break;
      else
         printf("Invalid entry, try again.\n");
   }
   return val;
}
int getSel(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7,
			  char *s8)
{
   int val;
   while (1)
   { 
      printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", s1, s2, s3, s4, s5, s6, s7, s8);
      val = getInt();
      if ((val >= 1)&&(val <= 8))
         break;
      else
         printf("Invalid entry, try again.\n");
   }
   return val;
}
int getSel(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7,
           char *s8, char *s9)
{
   int val;
   while (1)
   { 
      printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", s1, s2, s3, s4, s5, s6, s7,
				 s8, s9);
      val = getInt();
      if ((val >= 1)&&(val <= 9))
         break;
      else
         printf("Invalid entry, try again.\n");
   }
   return val;
}
int getSel(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7,
           char *s8, char *s9, char *s10)
{
   int val;
   while (1) 
   {    
      printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", s1, s2, s3, s4, s5, s6, s7, 
             s8, s9, s10);
      val = getInt();
      if ((val >= 1)&&(val <= 10))
         break; 
      else      
         printf("Invalid entry, try again.\n");
   }
   return val;
}


