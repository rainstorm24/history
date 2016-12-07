#include "gregutils.h"
#include <time.h>

char* read_text(char *filename){
	FILE *fp = fopen(filename, "r");
	if(!fp) return NULL;
	fseek(fp, 0, SEEK_END);
	size_t length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *data = calloc(sizeof(char), (length + 1));
	fread(data, sizeof(char), length, fp);
	fclose(fp);
	data[length] = '\0';
	return data;
}

char** split(char *src, char delimiter){
	int element_count = 1;
	for(int i = 0; src[i]; i++){
		if(src[i] == delimiter) element_count++;
	}
	char **arr = malloc(sizeof(char*) * (element_count + 1));
	for(int i = 0, index = 0; i < element_count; i++){
		int word_length = 0;
		while(src[index + word_length] && src[index + word_length] != delimiter) word_length++;
		arr[i] = malloc(sizeof(char) * (word_length + 1));
		for(int j = 0; j < word_length; j++){
			arr[i][j] = src[index + j];
		}
		arr[i][word_length] = '\0';
		index += word_length + 1; // +1 to skip delimiter
	}
	arr[element_count] = NULL;
	return arr;	
}

void free_arr(void **arr){
	for(int i = 0; *(arr + i); i++){
		free(*(arr + i));
	}
	free(arr);
}

int random(int max){
	srand(time(NULL));
	return rand() % max;
}