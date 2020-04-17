#ifndef SCHAIN_HASH_H
#define SCHAIN_HASH_H

#include <unistd.h>

typedef struct htable *HTable;

extern HTable htable(const size_t size);

extern void free_table(HTable table);

extern float balance_factor(HTable table);

extern HTable insert(HTable table, const int key);

extern HTable delete(HTable table, const int key);

extern void print_table(HTable table);

#endif
