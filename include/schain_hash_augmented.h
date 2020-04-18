#ifndef SCHAINHASH_AUGMENTED_H
#define SCHAINHASH_AUGMENTED_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

struct entry {
	char *key;
	int value;
	bool deleted;
	struct entry *next;
};

struct htable {
	size_t size;
	struct entry **buckets;
};

extern bool equals(const char *str_a, const char *str_b);

extern struct htable *htable(size_t size);

extern void free_table(struct htable *table);

extern struct htable *rehash_entry(struct htable *table, struct entry *new_entry);

extern struct htable *rehash_table(struct htable *table);

extern struct htable *insert(struct htable *table, const char *key, int value);

extern struct htable *delete(struct htable *table, const char *key);

extern struct entry *search(struct htable *table, const char *key);

extern void print_entry(struct entry *entry);

extern void print_table(struct htable *table);



#endif
