#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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