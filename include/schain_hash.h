#ifndef SCHAIN_HASH_H
#define SCHAIN_HASH_H

#include <unistd.h>

typedef struct htable *Table;
typedef struct bucket *Bucket;

extern Table htable(const size_t size);

extern void free_table(Table table);

extern float balance_factor(Table table);

extern Table insert(Table table, const int key);

extern Table delete(Table table, const int key);

extern Bucket search(Table table, const int key);

extern void print_key(Bucket bucket);

extern void print_table(Table table);

#endif
