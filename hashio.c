
#include <stdbool.h>
#include <time.h>
#include "include/schain_hash.h"
#include "include/schain_hash_assert.h"
#include "include/hashio.h"

#define DATA_PATH "data/randnumbers"
#define OUT_PATH "data/timestamps"

#define BILLION 1000000000.0

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
	msg(CLR_YELLOW EXIT_MSG CLR_RESET);
	exit(code);
}

static inline void run_file_test(Table *table)
{
	FILE *input = fopen(DATA_PATH, "r");
	FILE *output = fopen(OUT_PATH, "w");

	if (!input || !output)
		return;

	int n;
	int counter = 0;
	double avg_time = 0.0; 

	__INIT_DELTA_TIME__;

	fprintf(output, "---- Inserting keys ----\n\n");

	while (fscanf(input, "%d", &n) == 1) {
		__START_DELTA_TIME__;

		*table = insert(*table, n);

		__CALC_DELTA_TIME__;
		__print_delta_time(output);

		avg_time += time_d;
		counter++;
	}

	avg_time /= (double) counter;

	fprintf(output, "\n\n==== Average insertion time: %lf seconds\n\n", avg_time);
	fprintf(output, "\n\n---- Searching nodes ----\n\n");

	rewind(input);

	avg_time = 0.0;
	counter = 0;

	while (fscanf(input, "%d", &n) == 1) {
		__START_DELTA_TIME__;

		search(*table, n);

		__CALC_DELTA_TIME__;
		__print_delta_time(output);

		avg_time += time_d;
		counter++;
	}

	avg_time /= (double) counter;
	fprintf(output, "\n\n==== Average search time: %lf seconds\n\n", avg_time);

	fclose(input);
	fclose(output);
	
	msg(TESTS_COMPLETE_MSG);
}

static inline void insert_keys(Table *table)
{
	int key;
	
	__INIT_DELTA_TIME__;

	while (scanf("%d", &key) == 1) {
		__START_DELTA_TIME__;
		
		*table = insert(*table, key);

		__CALC_DELTA_TIME__;
		__print_delta_time(stdout);
	}
}

static inline void delete_keys(Table *table)
{
	int key;
	
	__INIT_DELTA_TIME__;

	while (scanf("%d", &key) == 1) {
		__START_DELTA_TIME__;
		
		*table = delete(*table, key);

		__CALC_DELTA_TIME__;
		__print_delta_time(stdout);
	}
}

static inline void search_key(Table table)
{ 
	int key;

	__INIT_DELTA_TIME__;

	if (scanf("%d", &key) != 1)
		return;
	getchar();
	
	__START_DELTA_TIME__;

	Bucket bucket = search(table, key);
	
	__CALC_DELTA_TIME__;
	__print_delta_time(stdout);

	if (bucket) {
		print_key(bucket);
	} else {
		msg(KEY_NFOUND_MSG "\n");
	}
}

static inline void handle_cmd(const char cmd, Table *table)
{
	switch (cmd) {
		case 'h'	: msg(COMMANDS);			break;
		case 'i'	: insert_keys(table); 			break;
		case 'd'	: delete_keys(table);			break;
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
	run_internal_tests();
	
	char input;
	size_t table_size;

	msg(WELCOME_MSG);
	msg(CLR_YELLOW "Enter table size: " CLR_RESET);
	
	if (scanf("%ld", &table_size) != 1 || table_size <= 0)
		return -1;

	Table table = htable(table_size);

	while (scanf("%c", &input) == 1)
		handle_cmd(input, &table);
	
	return 0;
}
