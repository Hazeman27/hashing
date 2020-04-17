#ifndef SCHAINHASH_AUGMENTED_H
#define SCHAINHASH_AUGMENTED_H

#include <stdlib.h>
#include <stdbool.h>

struct bucket {
	int key;
	bool deleted;
	struct bucket *next;
};

struct htable {
	size_t size;
	struct bucket *buckets[];
};

extern struct htable *htable(const size_t size);

extern void free_bucket(struct bucket *new_bucket);

extern void free_table(struct htable *table);

extern float balance_factor(struct htable *table);

extern size_t hash(const int key, const size_t table_size);

extern struct htable *rehash_bucket(struct htable *table, struct bucket *new_bucket);

extern struct htable *rehash_table(struct htable *table);

extern struct htable *insert(struct htable *table, const int key);

extern struct htable *delete(struct htable *table, const int key);

extern struct bucket *search(struct htable *table, const int key);

extern void print_key(struct bucket *bucket);

extern void print_table(struct htable *table);



#endif
