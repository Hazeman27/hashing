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

static inline struct entry *entry(const struct entry init)
{
	struct entry *entry = (struct entry *) malloc(sizeof(struct entry));

	if (!entry)
		return NULL;

	return (*entry = init, entry);
}

struct htable *htable(size_t size)
{
	if (size <= 0)
		return NULL;
	
	struct htable *table = (struct htable *) malloc(sizeof(struct htable));
	
	size = nearest_prime(size);
	table->buckets = (struct entry **) malloc(sizeof(struct entry *) * size);

	if (!table || !table->buckets)
		return NULL;

	return (table->size = size, table);
}

void free_entry(struct entry *entry)
{
	if (!entry)
		return;

	free_entry(entry->next);
	free(entry);
}


void free_table(struct htable *table)
{
	for (size_t i = 0; i < table->size; i++)
		free_entry(table->buckets[i]);

	free(table);
}

size_t bucket_depth(struct entry *entry)
{
	if (!entry)
		return 0;

	return (entry->deleted ? 0 : 1) + bucket_depth(entry->next);
}

float load_factor(struct htable *table)
{
	size_t buckets_count = 0;

	for (size_t i = 0; i < table->size; i++)
		buckets_count += bucket_depth(table->buckets[i]);

	return (float) buckets_count / (float) table->size;
}

size_t hash(const int key, const size_t table_size)
{
	return key % table_size;
}

struct htable *rehash_entry(struct htable *table, struct entry *entry)
{
	if (!entry)
		return table;
		
	table = insert(table, entry->key);

	return rehash_entry(table, entry->next);
}

struct htable *rehash_table(struct htable *table)
{
	struct htable *new_table = htable(table->size << 1);

	if (!new_table)
		return NULL;

	for (size_t i = 0; i < table->size; i++)
		new_table = rehash_entry(new_table, table->buckets[i]);

	free_table(table);
	return new_table;
}

struct htable *delete(struct htable *table, const int key)
{
	if (!table)
		return NULL;
	
	struct entry **indirect = &table->buckets[hash(key, table->size)];

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

	struct entry **indirect = &table->buckets[hash(key, table->size)];

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

	*indirect = entry((struct entry) { .key = key });
	
	if (load_factor(table) > 0.5f)
		return rehash_table(table);
	
	return table;
}

struct entry *search(struct htable *table, const int key)
{
	if (!table)
		return NULL;

	struct entry **indirect = &table->buckets[hash(key, table->size)];

	while (*indirect) {

		if (!(*indirect)->deleted && (*indirect)->key == key)
			return *indirect;

		indirect = &(*indirect)->next;
	}

	return NULL;
}

void print_key(struct entry *entry)
{
	if (!entry)
		return;

	printf(CLR_CYAN "Entry key: " CLR_YELLOW "[%d]\n" CLR_RESET, entry->key);
}

void print_table(struct htable *table)
{
	if (!table)
		return;

	printf(CLR_GREEN "\n>>> Printing Hash Table:\n\n" CLR_RESET);

	for (size_t i = 0; i < table->size; i++) {

		printf(CLR_BLUE "%ld: " CLR_RESET, i);
		struct entry *current = table->buckets[i];

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
