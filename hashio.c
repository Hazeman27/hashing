#include <stdbool.h>
#include <time.h>
#include "include/schain_hash.h"
#include "include/hashio.h"

#define OUT_PATH "data/timestamps"

#define BILLION 1000000000.0

#define KEY_MAX_L 12
#define VALUE_MAX 100000

#define __INIT_DELTA_TIME__ 		\
	double time_d;			\
	struct timespec start, end;	\

#define __START_DELTA_TIME__			\
	time_d = 0.0;				\
	clock_gettime(CLOCK_REALTIME, &start);	\

#define __CALC_DELTA_TIME__				\
	clock_gettime(CLOCK_REALTIME, &end);		\
	time_d = ((end).tv_sec - (start).tv_sec) + 	\
		((end).tv_nsec - (start).tv_nsec) 	\
			/ BILLION

#define DELTA_TIME_FORMAT ">> %s: %lf seconds --\n"

#define __print_delta_time(stream)				\
	fprintf((stream), DELTA_TIME_FORMAT, __func__, time_d);	\


static inline void exit_program(Table table, int code)
{
	free_table(table);
	exit(code);
}

static inline void run_file_test(Table *table)
{
	FILE *output = fopen(OUT_PATH, "w");

	if (!output)
		return;

	size_t iter_count, i, j;

	double avg_insert_time;
	double avg_search_time;

	double avg_time = 0.0;	

	__INIT_DELTA_TIME__;
	
	printf(CLR_CYAN "Enter iterations count: " CLR_RESET);

	if (scanf("%ld", &iter_count) != 1) {
		errno = EPERM;
		perror("Only numbers are allowed");
		return;
	}
	
	if (iter_count < 0) {
		errno = 1;
		perror("Only positive numbers are allowed");
		return;
	}

	char *keys[iter_count];
	
	fprintf(output, "---- Inserting keys ----\n\n");

	for (i = 0; i < iter_count; i++) {

		size_t key_length = rand() % KEY_MAX_L + 2;
		char key[KEY_MAX_L];

		for (j = 0; j < key_length; j++)
			key[j] = 'a' + rand() % 25;
		
		key[j] = '\0';
		keys[i] = key;

		__START_DELTA_TIME__;

		*table = insert(*table, key, rand() % VALUE_MAX);

		__CALC_DELTA_TIME__;
		__print_delta_time(output);

		avg_time += time_d;
	}

	avg_insert_time = avg_time / (double) i;

	avg_time = 0.0;
	fprintf(output, "\n\n---- Searching nodes ----\n\n");
	
	for (i = 0; i < iter_count; i++) {
		__START_DELTA_TIME__;

		search(*table, keys[i]);

		__CALC_DELTA_TIME__;
		__print_delta_time(output);

		avg_time += time_d;
	}

	avg_search_time = avg_time / (double) i;
	
	fprintf(output, "\n\n==== Average search time: %lf seconds\n\n", avg_search_time);
	fprintf(output, "\n\n==== Average insertion time: %lf seconds\n\n", avg_insert_time);

	fclose(output);
	printf(CLR_CYAN "Random insertion and search test is complete\n" CLR_RESET);
}

static inline void insert_key(Table *table)
{
	char key[KEY_MAX_L];
	int value;
	
	getchar();	
	__INIT_DELTA_TIME__;

	if (scanf("%s %d", key, &value) == 2) {
		getchar();
		__START_DELTA_TIME__;
		
		*table = insert(*table, key, value);

		__CALC_DELTA_TIME__;
		__print_delta_time(stdout);
	}
}

static inline void delete_key(Table *table)
{
	char key[KEY_MAX_L];
	
	__INIT_DELTA_TIME__;

	if (scanf("%s", key) == 1) {
		__START_DELTA_TIME__;
		
		*table = delete(*table, key);

		__CALC_DELTA_TIME__;
		__print_delta_time(stdout);
	}
}

static inline void search_key(Table table)
{ 
	char key[KEY_MAX_L];

	__INIT_DELTA_TIME__;

	if (scanf("%s", key) != 1)
		return;
	getchar();
	
	__START_DELTA_TIME__;

	Entry entry = search(table, key);
	
	__CALC_DELTA_TIME__;
	__print_delta_time(stdout);

	if (entry) {
		print_entry(entry);
	} else {
		printf("Entry not found...\n");
	}
}

static inline void handle_cmd(const char cmd, Table *table)
{
	switch (cmd) {
		case 'h'	: printf(COMMANDS);			break;
		case 'i'	: insert_key(table); 			break;
		case 'd'	: delete_key(table);			break;
		case 's'	: search_key(*table);			break;
		case 'p'	: print_table(*table); 			break;
		case 'q'	: exit_program(*table, EXIT_SUCCESS);	break;
		case 't'	: run_file_test(table);			break;
		case '\n'	: 					break;
		default		: perror(UNKNOWN_CMD_ERR); 		break;
	}
}

int main(void)
{
	//run_internal_tests();

	char input;
	size_t table_size;

	printf(WELCOME_MSG);
	printf(CLR_YELLOW "Enter table size: " CLR_RESET);
	
	if (scanf("%ld", &table_size) != 1 || table_size <= 0)
		return -1;

	Table table = htable(table_size);

	while (scanf("%c", &input) == 1)
		handle_cmd(input, &table);
	
	return 0;
}
