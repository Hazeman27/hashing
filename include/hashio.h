#ifndef HASHIO_H
#define HASHIO_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define CLR_RED     	"\x1b[31m"
#define CLR_GREEN  	"\x1b[32m"
#define CLR_YELLOW	"\x1b[33m"
#define CLR_BLUE    	"\x1b[34m"
#define CLR_MAGENTA 	"\x1b[35m"
#define CLR_CYAN    	"\x1b[36m"
#define CLR_RESET   	"\x1b[0m"

#define UNKNOWN_CMD_ERR "Unknown command"
#define COMMANDS CLR_CYAN "Commands:\n\th - print commands\n\ti [...values] - insert keys\n\td [...values] - delete keys\n\ts - search for key\n\tt - run insert tests from ./data/randnumbers file\n\tp - print hash table\n\tq - exit program\n--------------------------\n" CLR_RESET

#define WELCOME_MSG CLR_MAGENTA "-- Welcome to hash tables interaction tool!\n" CLR_RESET COMMANDS
	
#endif
