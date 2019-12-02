#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#define ROW_LENGTH 1024
#define MAX_TWEETS 20000

size_t tweeter_col = 99;
bool has_quotes;

typedef struct tweeter{
	char *name;
	int count;
} tweeter;

tweeter list[MAX_TWEETS];

int add_name(size_t* list_size, char* name)
{
	for(int i = 0; i < *list_size; i++){
		// They are the same name
		if(strcmp(list[i].name, name) == 0){
			list[i].count++;
			return 1;
		}
	}

	if(*list_size < MAX_TWEETS){
		list[*list_size].name = malloc(sizeof(char) * strlen(name));
		strcpy(list[*list_size].name, name);
		list[*list_size].count = 1;
		(*list_size)++;
	} else {
		printf("Invalid Input Format\n");
		exit(1);
	}
	return 0;
}

char* unquote(char* name)
{	
	int str_len = strlen(name);

	if(name == NULL){
		printf("Invalid Input Format\n");
		exit(1);
	}

	// Make sure it ends in quotes if it should	
	if(has_quotes){
		if((name[0] != '\"') || (name[str_len - 1] != '\"')){
                	printf("Invalid Input Format\n");
                	exit(1);
        	}
	}

	if(!has_quotes){
		if((name[0] == '\"') && (name[str_len - 1] == '\"')){
                	printf("Invalid Input Format\n");
                	exit(1);
        	}
		return name;
	}


	// Case where name is just "" and it has quotes
	if(str_len - 2 <= 0){
		printf("Invalid Input Format\n");
		exit(1);
	}

	// Get the name between the quotes
	char *new_name = malloc(str_len - 2);
	for(int i = 0; i < (str_len - 2); i++){
		new_name[i] = name[i + 1];
	}

	return new_name; 
}

int process_header(char *row_cpy)
{
	if(row_cpy == NULL){
		printf("Invalid Input Format\n");
		exit(1);
	}

	int name_occurence = 0;
	int curr_col = 0;
	char* token;
	while ((token = strsep(&row_cpy, ","))) {
        	if (strcmp(token, "name") == 0) {
			tweeter_col = curr_col;
			name_occurence++;
			has_quotes = false;
		}

		if(strcmp(token, "\"name\"") == 0){
			tweeter_col = curr_col;
			name_occurence++;
			has_quotes = true;
		}
		curr_col++;
	}

	if(name_occurence > 1){
		printf("Invalid Input Format\n");
		exit(1);
	}

	return curr_col;
}

void tweeter_sort(int list_size)
{
	for (int i = 0; i < list_size; i++){                    
		for (int j = 0; j < list_size; j++){             
			if (list[j].count < list[i].count){               
				tweeter tmp = list[i];         
				list[i] = list[j];           
				list[j] = tmp;            
			}
		}
	}
}

void tweeter_print(int list_size)
{
	int counter;
	if(list_size < 10){
		counter = list_size;
	}
	else{
		counter = 10;
	}

	for(int i = 0; i < counter; i++){
		printf("%s: %d\n", list[i].name, list[i].count);
	}
	return;
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
	int total_col = 0;
	bool empty_line = false;
	
	// Process the header to get tweeter column
	if(getline(&row, &row_len, fp) > -1){
		char* row_cpy = row;
		total_col = process_header(row_cpy);
		
	}
	else{
		printf("Invalid Input Format\n");
		exit(1);
	}

	//parse csv file line by line
	while (getline(&row, &row_len, fp) > -1) {
		
		// empty line found
		if(row[strspn(row, " \n")] == '\0'){
			empty_line = true;
			continue;
		}

		char* row_cpy;
		char* token;
		size_t curr_col = 0;

		row_cpy = row;

		//parse line token by token
		while ((token = strsep(&row_cpy, ","))) {
				
			if (curr_col == tweeter_col){
				add_name(&list_size, unquote(token));
			}
			curr_col++;
		}
		curr_row++;
		// count of parsed field doesn't match header count
		if(curr_col != total_col){
			printf("Invalid Input Format\n");
			exit(1);
		}

		// found more content after finding empty line
		if(empty_line){
			printf("Invalid Input Format\n");
                	exit(1);
		}
	}

	free(row);

	tweeter_sort(list_size);

	tweeter_print(list_size);

	return 0;
}
