#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "admin.h"
#include "gregutils.h"
#include "linked_list.h"
#include "player.h"
#include <math.h>

#define MINIMUM_PLAYERS 59
/*

27 Stakhanovite Workers 3StakW/IM (1)
9 Industrial Managers 3IMs/OPL (2)
7 Bedniaks (3)
5 NKVD Officers (4)
3 Economic Planners (5)
3 Oblast Party Leaders (6)
3 Ex-Kulaks (7)
1 Stalin (8)
1 Yagoda (9)
1 Yezhov (10)
*/

typedef struct player_type{
	char *type;
	int count;
} player_type;

player_type* create_player_type(char *type, int count){
	player_type *pt = malloc(sizeof(player_type));
	pt->type = type;
	pt->count = count;
	return pt;
}

int main(int argc, char **argv){
	//assign_players("students.txt");
	player *p = load_player("C:\\wamp\\bin\\apache\\apache2.2.22\\cgi-bin\\Yagoda\\Omer Dresel.txt");
	player_atr_increment(p, "Favor", -5);
	FILE *fp = fopen("C:\\wamp\\bin\\apache\\apache2.2.22\\cgi-bin\\Yagoda\\Omer Dresel.txt", "w");
	fprintf(fp, "%d\n", p->attributes->size);
	for(int i = 0; i < p->attributes->size; i++){
		fprintf(fp, "%s=%d\n", ((attribute*)(get_list_node_at(p->attributes, i))->data)->name, ((attribute*)(get_list_node_at(p->attributes, i))->data)->amount);
	}
	fprintf(fp, "%s\n%s\n", p->character, p->name);
	fclose(fp);
}

void free_player_type(void *ptype){
	//free(ptype);
}

int compare_player_type(void *a, void *b){
	return strcmp(((player_type*)a)->type, ((player_type*)b)->type);
}

void print_type(void *ptr){
	printf("%s", ((player_type*)ptr)->type);
}

bool assign_players(char *filename){
	char **students = split(read_text(filename), '\n');
	int num_students = 0;
	while(students[num_students]) num_students++;
	if(num_students < MINIMUM_PLAYERS){
		return false; // below minimum number of participants
	}
	int peasants = num_students - MINIMUM_PLAYERS;
	int bedniaks = peasants * 2 / 3; // 2:1 ratio between bediaks and ex_kulaks
	int ex_kulaks = peasants - bedniaks;
	linked_list *type_list = create_linked_list(compare_player_type, free_player_type, print_type);
	// number of each type of player in the game
	add_list_node(type_list, create_list_node(create_player_type("Stakhanovite Worker", 27)));
	add_list_node(type_list, create_list_node(create_player_type("Industrial Manager", 9)));
	add_list_node(type_list, create_list_node(create_player_type("Bedniak", 7 + bedniaks)));
	add_list_node(type_list, create_list_node(create_player_type("NKVD Officer", 5)));
	add_list_node(type_list, create_list_node(create_player_type("Economic Planner", 3)));
	add_list_node(type_list, create_list_node(create_player_type("Oblast Party Leader", 3)));
	add_list_node(type_list, create_list_node(create_player_type("Ex-Kulak", 3 + ex_kulaks)));
	add_list_node(type_list, create_list_node(create_player_type("Yagoda", 1)));
	add_list_node(type_list, create_list_node(create_player_type("Yezhov", 1)));
	
	
	for(int i = 0; students[i]; i++){
		int r = random(type_list->size);
		list_node *node = get_list_node_at(type_list, r);
		char *dir = malloc(sizeof(char) * 30);
		sprintf(dir, "\%s\\", ((player_type*)node->data)->type);
		struct stat st = {0};
		if (stat(dir, &st) == -1) {
			mkdir(dir);
		} 
		char **character_attirbute_names;
		if(strcmp(((player_type*)node->data)->type, "Stakhanovite Worker") == 0){
			character_attirbute_names = split("Motivation,5 Suspicion,2 Pressure,4 Fatigue,0", ' ');
		}else if(strcmp(((player_type*)node->data)->type, "Industrial Manager") == 0){
			character_attirbute_names = split("Pressure,4 Alliance,1 Support,4 Suspicion,4 Production,5", ' ');
		}else if(strcmp(((player_type*)node->data)->type, "Bedniak") == 0){
			character_attirbute_names = split("Hunger,4 Loyatly,3 Discontent,7 Pressure,6", ' ');
		}else if(strcmp(((player_type*)node->data)->type, "NKVD Officer") == 0){
			character_attirbute_names = split("Pressure,6 Suspicion,4 Credibility,6 Favor,5 Production,5", ' ');
		}else if(strcmp(((player_type*)node->data)->type, "Economic Planner") == 0){
			character_attirbute_names = split("Politics,5 Decision,5 Alignment,10 Suspicion,4", ' ');
		}else if(strcmp(((player_type*)node->data)->type, "Oblast Party Leader") == 0){
			character_attirbute_names = split("Pressure,4 Alliance,1 Support,4 Suspicion,4 Production,5", ' ');
		}else if(strcmp(((player_type*)node->data)->type, "Ex-Kulak") == 0){
			character_attirbute_names = split("Hunger,3 Loyatly,2 Discontent,5 Pressure,7", ' ');
		}else if(strcmp(((player_type*)node->data)->type, "Yagoda") == 0){
			character_attirbute_names = split("Favor,5 Credibility,6 Suspicion,6 Power,7 Pressure,7", ' ');
		}else if(strcmp(((player_type*)node->data)->type, "Yezhov") == 0){
			character_attirbute_names = split("Favor,7 Credibility,5 Suspicion,4 Power,3 Pressure,5", ' ');
		}
		linked_list *attributes = create_linked_list(attribute_compare, NULL, print_atr);
		int num_atrs = 0;
		for(int i = 0; character_attirbute_names[i]; i++){
			char **atr_info = split(character_attirbute_names[i], ',');
			attribute *a = attribute_create(atr_info[0], atoi(atr_info[1]));
			list_node *n = create_list_node(a);
			add_list_node(attributes, n);
			num_atrs++;
		}
		player *p = player_create(((player_type*)node->data)->type, students[i], attributes);
		
		char *filepath = calloc(sizeof(char) , 100);
		sprintf(filepath, "%s\%s.txt", dir, students[i]);
		FILE *fp = fopen(filepath, "w");
		if(!fp){
			printf("error in opening file\n");
		}else{ 	
			//printf("file opened\n");
		}
		fprintf(fp, "%d\n", num_atrs);
		player_print_attributes(p, fp);
		fprintf(fp, "%s\n%s\n", ((player_type*)node->data)->type, students[i]);
		free(filepath);
		free(dir);
		fclose(fp);
		
		((player_type*)(node->data))->count--;
		if(((player_type*)(node->data))->count == 0){
			delete_list_node(type_list, node);
		}
	}
	
	free_arr(students);
	return true;
}