#ifndef SCHAINHASH_AUGMENTED_H
#define SCHAINHASH_AUGMENTED_H

#include <stdlib.h>
#include <stdbool.h>

struct entry {
	int key;
	bool deleted;
	struct entry *next;
};

struct htable {
	size_t size;
	struct entry **buckets;
};

extern struct htable *htable(size_t size);

extern void free_entry(struct entry *new_entry);

extern void free_table(struct htable *table);

extern float balance_factor(struct htable *table);

extern size_t hash(const int key, const size_t table_size);

extern struct htable *rehash_entry(struct htable *table, struct entry *new_entry);

extern struct htable *rehash_table(struct htable *table);

extern struct htable *insert(struct htable *table, const int key);

extern struct htable *delete(struct htable *table, const int key);

extern struct entry *search(struct htable *table, const int key);

extern void print_key(struct entry *entry);

extern void print_table(struct htable *table);



#endif
