#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_LENGTH 1024
#define MAX_TWEETS 20000

typedef struct tweeter{
	char *name;
	int count;
} tweeter;

void add_name(tweeter *list, int list_size, char *name)
{
	for(int i = 0; i < list_size; i++){
		// They are the same name
		if(strcmp(list[i].name, name) == 0){
			list[i].count++;
			return;
		}
	}

	if(list_size < MAX_TWEETS){
		list[list_size + 1].name = name;
		list[list_size + 1].count = 0;
	}
	else{
		printf("Invalid input format\n");
		exit(1);
	}	
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

	size_t row_len = 1024;
	char* row = malloc(row_len);

	//parse csv file line by line
	while (getline(&row, &row_len, fp) > -1) {
		printf("%s", row);
	}

	free(row);

	return 0;
}
