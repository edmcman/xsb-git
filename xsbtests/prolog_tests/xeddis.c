#include <stdio.h>
#include <string.h>

#include "cinterf.h"
/*
extern char* ptoc_string(int);
extern void  ctop_string(int,char *);
extern void  ctop_int(int,int);
 */

int min_ed_dis(void)
{
  int len1,len2, i, j;
  int ins,del,rep;
  int cost[200][200];
  char *string1, *string2;

  string1 = ptoc_string(1);
  string2 = ptoc_string(2);
  len1 = strlen(string1);
  len2 = strlen(string2);
  for (i = 0; i < 200; i++) cost[i][0]= i;
  for (i = 0; i < 200; i++) cost[0][i]= i;
  for (i=1; i<=len1; i++) {
    for (j=1; j<=len2; j++) {
/*      printf("%c %c \n",string1[i-1],string2[j-1]);*/
      del = cost[i-1][j]+1;
      ins = cost[i][j-1]+1;
      if (string1[i-1] == string2[j-1]) 
	rep = cost[i-1][j-1];
      else rep = cost[i-1][j-1] + 1;
     cost[i][j] = mymin(del,ins,rep);
/*      cost[i][j] = del<ins ? (del<rep ? del : rep) : (ins<rep ? ins : rep);*/
    }
  }   
/*  for (i=0; i<=len1; i++) {
    for (j=0; j<=len2; j++) {
      printf(" %d",cost[i][j]);
    }
    printf("\n");
  }*/
/*  return cost[len1][len2];*/
    ctop_int(3,cost[len1][len2]);
}

int mymin(a,b,c)
int a,b,c;
{
  if (a < b) {
    if (a < c) return a; else return c;
  }
  else {
    if (b < c) return b; else return c;
  }
}

substring(void)
{
char *string1,*string2;
int length, i;

string1 = ptoc_string(1);
string2 = ptoc_string(2);

length = strlen(string2);

i = 0;

while (string1[i] == string2[i] && i < length) i++;

ctop_int(3,i);
}
