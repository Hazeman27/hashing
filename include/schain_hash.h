#ifndef SCHAIN_HASH_H
#define SCHAIN_HASH_H

#include <unistd.h>

typedef struct htable *Table;
typedef struct entry *Entry;

extern Table htable(size_t size);

extern void free_table(Table table);

extern Table insert(Table table, const char *key, int value);

extern Table delete(Table table, const char *key);

extern Entry search(Table table, const char *key);

extern void print_entry(Entry entry);

extern void print_table(Table table);

#endif
