#ifndef _PLAYER_H
#define _PLAYER_H 1
#include "linked_list.h"
#include <stdio.h>

typedef struct attribute{
	char *name;
	int amount;
} attribute;

typedef struct player{
	char *name;
	char *character;
	linked_list *attributes;
} player;
int attribute_compare(void *a, void *b);
void print_atr(void *ptr);
attribute* attribute_create(char*, int);
player* player_create(char*, char*, linked_list*);
player* load_player(char*);
bool player_atr_increment(player*, char*, int);
bool player_contains_atr(player*, attribute*);
void player_print_attributes(player*, FILE*);
#endif
