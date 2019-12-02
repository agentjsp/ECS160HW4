#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define ROW_LENGTH 1024
#define MAX_TWEETS 20000

size_t tweeter_col = 99;

typedef struct tweeter{
	char *name;
	int count;
} tweeter;

tweeter list[MAX_TWEETS];

int add_name(size_t* list_size, char* name)
{
	printf("add_name: list size is currently %d\n", *list_size);
	for(int i = 0; i < *list_size; i++){
		// They are the same name
		printf("add_name: comparing names %s and %s at %d \n", list[i].name, name, i);
		if(strcmp(list[i].name, name) == 0){
			printf("add_name: name match\n");
			list[i].count++;
			return 1;
		}
	}

	if(*list_size < MAX_TWEETS){
		printf("add_name: adding list entry %s at %d\n", name, *list_size);
		list[*list_size].name = malloc(sizeof(char) * strlen(name));
		strcpy(list[*list_size].name, name);
		list[*list_size].count = 1;
		(*list_size)++;
	} else {
		printf("Invalid input format\n");
		exit(1);
	}
	return 0;
}	

int main(int argc, char** argv) {
	if (argc < 1) {
		printf("Missing filepath.\n");
		return 0;
	}

	FILE* fp = fopen(*++argv, "r");

	if (!fp) {
		printf("Invalid file path.\n");
		return 1;
	}

	size_t row_len = 1024, curr_row = 0, list_size = 0;
	char* row = malloc(row_len);

	//parse csv file line by line
	while (getline(&row, &row_len, fp) > -1) {
		printf("Current row: %s\n", row);
		char* row_cpy;
		char* token;
		size_t curr_col = 0;

		row_cpy = row;

		//parse line token by token
		while (token = strsep(&row_cpy, ",")) {
			printf("Current token: %s \n", token);
			if (curr_row == 0) { 
			//parse header row to find column indice of tweeter names
				if (strcmp(token, "name") == 0) {
					tweeter_col = curr_col;
				}
			} else {
				if (curr_col == tweeter_col) {
					printf("launching add_name\n");
					add_name(&list_size, token);
				}
			}
			curr_col++;
		}
		curr_row++;
	}

	free(row);

	for(int i = 0; i < list_size; i++){
		printf("name: %s count: %d\n", list[i].name, list[i].count);
	}

	return 0;
}