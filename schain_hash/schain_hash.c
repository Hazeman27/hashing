/**
 * Implementation of separate chaining hashing
 */

#include "../include/schain_hash_augmented.h"
#include "../include/hashio.h"

static inline bool isprime(const size_t n)
{
	if (n == 2 || n == 3)
		return true;

	if (!(n % 2) || !(n % 3))
		return false;

	size_t coeff = 6;

	while (coeff * coeff - (coeff << 1) + 1 <= n) {

		if (!(n % (coeff - 1)))
			return false;

		if (!(n % (coeff + 1)))
			return false;

		coeff += 6;
	}

	return true;
}

static inline bool ispow2(const size_t n)
{
	return n && !(n & (n - 1));
}

static inline size_t nearest_prime(const size_t n)
{
	if (n > 0 && !ispow2(n - 1) && !ispow2(n + 1) && isprime(n))
		return n;
	
	return nearest_prime(n + 1);
}

static inline struct bucket *bucket(const struct bucket init)
{
	struct bucket *bucket = (struct bucket *) malloc(sizeof(struct bucket));

	if (!bucket)
		return NULL;

	return (*bucket = init, bucket);
}

struct htable *htable(size_t size)
{
	if (size <= 0)
		return NULL;

	size = nearest_prime(size);

	struct htable *table = (struct htable *) malloc(
		sizeof(struct htable) + (sizeof(struct bucket *) * size)		
	);

	if (!table)
		return NULL;

	return (table->size = size, table);
}

void free_bucket(struct bucket *bucket)
{
	if (!bucket)
		return;

	free_bucket(bucket->next);
	free(bucket);
}


void free_table(struct htable *table)
{
	for (size_t i = 0; i < table->size; i++)
		free_bucket(table->buckets[i]);

	free(table);
}

size_t bucket_depth(struct bucket *bucket)
{
	if (!bucket)
		return 0;

	return (bucket->deleted ? 0 : 1) + bucket_depth(bucket->next);
}

float balance_factor(struct htable *table)
{
	size_t entries_count = 0;

	for (size_t i = 0; i < table->size; i++)
		entries_count += bucket_depth(table->buckets[i]);

	return (float) entries_count / (float) table->size;
}

size_t hash(const int key, const size_t table_size)
{
	return key % table_size;
}

struct htable *rehash_bucket(struct htable *table, struct bucket *bucket)
{
	if (!bucket)
		return table;

	return insert(rehash_bucket(table, bucket->next), bucket->key);
}

struct htable *rehash_table(struct htable *table)
{
	struct htable *new_table = htable(table->size << 1);

	if (!new_table)
		return NULL;

	for (size_t i = 0; i < table->size; i++)
		new_table = rehash_bucket(new_table, table->buckets[i]);

	free_table(table);
	return new_table;
}

struct htable *delete(struct htable *table, const int key)
{
	if (!table)
		return NULL;
	
	struct bucket **indirect = &table->buckets[hash(key, table->size)];

	while (*indirect) {

		if ((*indirect)->key == key) {
			(*indirect)->deleted = true;
			return table;
		}

		indirect = &(*indirect)->next;
	}

	return table;
}

struct htable *insert(struct htable *table, const int key)
{
	if (!table)
		return NULL;

	struct bucket **indirect = &table->buckets[hash(key, table->size)];

	while (*indirect) {

		if ((*indirect)->key == key) {
			perror(EDUPVAL);
			return table;
		}

		if ((*indirect)->deleted) {

			(*indirect)->key = key;
			(*indirect)->deleted = false;
			
			return table;
		}

		indirect = &(*indirect)->next;
	}

	*indirect = bucket((struct bucket) { .key = key });
	
	if (balance_factor(table) > 0.5f)
		return rehash_table(table);
	
	return table;
}

struct bucket *search(struct htable *table, const int key)
{
	if (!table)
		return NULL;

	struct bucket **indirect = &table->buckets[hash(key, table->size)];

	while (*indirect) {

		if (!(*indirect)->deleted && (*indirect)->key == key)
			return *indirect;

		indirect = &(*indirect)->next;
	}

	return NULL;
}

void print_key(struct bucket *bucket)
{
	if (!bucket)
		return;

	printf(CLR_CYAN "Bucket key: " CLR_YELLOW "[%d]\n" CLR_RESET, bucket->key);
}

void print_table(struct htable *table)
{
	if (!table)
		return;

	printf(CLR_GREEN "\n>>> Printing Hash Table:\n\n" CLR_RESET);

	for (size_t i = 0; i < table->size; i++) {

		printf(CLR_BLUE "%ld: " CLR_RESET, i);
		struct bucket *current = table->buckets[i];

		if (!current) {
			printf(CLR_CYAN "~" CLR_RESET);
		} else {
			while (current) {

				if (current->deleted) {
					printf(CLR_RED "**DELETED** " CLR_RESET);
				} else {
					printf(CLR_CYAN "%d " CLR_RESET, current->key);
				}

				current = current->next;
			}
		}

		printf("\n");
	}

	printf(CLR_GREEN "---------------------------------\n\n" CLR_RESET);
}
