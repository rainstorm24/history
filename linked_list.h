/* Author Greg Johnson */
/* 9.28.16 */
#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct list_node{
	void *data;
	struct list_node *parent, *child;
} list_node;

typedef int (*comparison_func) (void*, void*);
typedef void (*free_func) (void*);

typedef struct linked_list{
	comparison_func compare;
	free_func free_node, print;
	//char *print_format;
	list_node *head;
	unsigned int size;
} linked_list;

linked_list* create_linked_list(comparison_func, free_func, free_func);

list_node* create_list_node(void*);
list_node* get_list_node(linked_list*, void*);
list_node* get_list_node_at(linked_list*, int);

void add_list_node(linked_list*, list_node*);
void print_list(linked_list*);
void empty_list(linked_list*);

bool insert_list_node(linked_list*, list_node*, list_node*);
bool contains_list_node(linked_list*, list_node*);
bool delete_list_node(linked_list*, list_node*);

#endif