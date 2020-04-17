/**
 * Implementation of separate chaining hashing
 */

#include "../include/schain_hash_augmented.h"

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

	size = nearest_prime(size);

	struct htable *table = (struct htable *) malloc(
		sizeof(struct htable) + (sizeof(struct entry *) * size)		
	);

	if (!table)
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
		free_entry(table->entries[i]);

	free(table);
}

float balance_factor(struct htable *table)
{
	size_t entries_count = 0;

	for (size_t i = 0; i < table->size; i++) {

		struct entry **indirect = &table->entries[i];

		while (*indirect) {
			
			if (!(*indirect)->deleted)
				entries_count++;

			indirect = &(*indirect)->next;
		}
	}

	return (float) entries_count / (float) table->size;
}

size_t hash(const int key, const size_t table_size)
{
	return key % table_size;
}

struct htable *rehash_entry(struct htable *table, struct entry *entry)
{
	if (!entry)
		return table;

	return insert(rehash_entry(table, entry->next), entry->key);
}

struct htable *rehash_table(struct htable *table)
{
	struct htable *new_table = htable(table->size << 1);

	if (!new_table)
		return NULL;

	for (size_t i = 0; i < table->size; i++)
		new_table = rehash_entry(new_table, table->entries[i]);

	free_table(table);
	return new_table;
}

struct htable *insert(struct htable *table, const int key)
{
	struct entry **indirect = &table->entries[hash(key, table->size)];

	while (*indirect) {

		if ((*indirect)->deleted) {

			(*indirect)->key = key;
			(*indirect)->deleted = false;
			
			return table;
		}

		indirect = &(*indirect)->next;
	}

	*indirect = entry((struct entry) { .key = key });
	
	if (balance_factor(table) > 0.5f)
		return rehash_table(table);
	
	return table;
}

void print_table(struct htable *table)
{
	if (!table)
		return;

	printf("\n>>> Printing Hash Table:\n");

	for (size_t i = 0; i < table->size; i++) {

		printf("%ld: ", i);
		struct entry *current = table->entries[i];

		if (!current) {
			printf("~");
		} else {
			while (current) {

				if (current->deleted) {
					printf("**DELETED** ");
				} else {
					printf("%d ", current->key);
				}

				current = current->next;
			}
		}

		printf("\n");
	}

	printf(">>>\n");
}
