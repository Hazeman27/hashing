#ifndef HASHIO_H
#define HASHIO_H

#include <stdio.h>

#define CLR_RED     	"\x1b[31m"
#define CLR_GREEN  	"\x1b[32m"
#define CLR_YELLOW	"\x1b[33m"
#define CLR_BLUE    	"\x1b[34m"
#define CLR_MAGENTA 	"\x1b[35m"
#define CLR_CYAN    	"\x1b[36m"
#define CLR_RESET   	"\x1b[0m"

#define UNKNOWN_CMD_ERR "Unknown command"
#define UNDEF_FUNC_T	"No defined function for the type"
#define KEY_NFOUND_MSG	"Key not found."
#define EXIT_MSG 	"Exiting...\n"

#define ENOMEM 		"Insufficent memory, cannot allocate node"
#define EDUPVAL 	"Duplicate value insertion. Operation rejected"

#define COMMANDS CLR_CYAN "Commands:\n\th - print commands\n\ti [...values] - insert keys\n\td [...values] - delete keys\n\ts - search for key\n\tt - run insert tests from ./data/randnumbers file\n\tp - print hash table\n\tq - exit program\n--------------------------\n" CLR_RESET

#define TESTS_COMPLETE_MSG CLR_YELLOW "\n>> Tests complete. Results written to the ./data/timestampts file --\n" CLR_RESET
#define WELCOME_MSG CLR_MAGENTA "-- Welcome to hash tables interaction tool!\n" CLR_RESET COMMANDS
	
#define EXIT_FUNC (printf(CLR_BLUE "--Exiting %s\n" CLR_RESET, __func__))
#define perror(error) (printf(CLR_RED "--Error: %s--" CLR_YELLOW " in %s\n" CLR_RESET, error, __func__))

#define msg(...) (printf(__VA_ARGS__))

#endif
