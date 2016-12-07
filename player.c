#include "player.h"
int attribute_compare(void *a, void *b){
	return strcmp(((attribute*)a)->name, ((attribute*)b)->name);
}

void print_atr(void *ptr){
	printf("%s:%d", ((attribute*)ptr)->name, ((attribute*)ptr)->amount);
}
attribute* attribute_create(char *name, int amount){
	attribute *a = malloc(sizeof(attribute));
	a->name = name;
	a->amount = amount;
	return a;
}

player* player_create(char *name, char* character, linked_list *attributes){
	player *p = malloc(sizeof(player));
	p->name = name;
	p->character = character;
	p->attributes = attributes;
	return p;
}

bool player_contains_atr(player *p, attribute *a){
	list_node *n = p->attributes->head;
	while(n){
		if(p->attributes->compare(a, n->data) == 0){
			return true;
		}
		n = n->child;
	}
	return false;
}

bool player_add_atr(player *p, char *atr, int amnt){
	attribute *a = attribute_create(atr, amnt);
	add_list_node(p->attributes, a);
}

bool player_set_atr(player *p, char *atr, int amnt){
	attribute *a = attribute_create(atr, 0);
	if(player_contains_atr(p, a)){
		((attribute*)get_list_node(p->attributes, a)->data)->amount = amnt;
		return true;
	}else{
		return false;
	}
	free(a);
}

player* load_player(char *filename){
	char *text = read_text(filename);
	char **lines = split(text, '\n');
	int num_atrs = atoi(lines[0]);
	linked_list *attributes = create_linked_list(attribute_compare, NULL, print_atr);
	for(int i = 0; i < num_atrs; i++){
		char **atr_info = split(lines[i + 1], '=');
		attribute *a = attribute_create(atr_info[0], atoi(atr_info[1]));
		list_node *n = create_list_node(a);
		add_list_node(attributes, n);
	}
	return player_create(lines[num_atrs + 2], lines[num_atrs + 1], attributes);
}

bool player_atr_increment(player *p, char *atr, int amnt){
	attribute *a = attribute_create(atr, 0);
	if(player_contains_atr(p, a)){
		((attribute*)get_list_node(p->attributes, a)->data)->amount += amnt;
		return true;
	}else{
		return false;
	}
	free(a);
}

void player_print_attributes(player *p, FILE *fp){
	list_node *n = p->attributes->head;
	while(n){
		fprintf(fp, "%s=%d\n", ((attribute*)n->data)->name, ((attribute*)n->data)->amount);
		n = n->child;
	}
}
