#include <assert.h>
#include <stdarg.h>
#include "../include/schain_hash_augmented.h"

static struct htable *insert_test(struct htable *table, const int key)
{
	table = insert(table, key);
	assert(table != NULL && search(table, key)->key == key);

	return table;
}

static struct htable *delete_test(struct htable *table, const int key)
{
	if (table == NULL) {
		assert(delete(table, key) == NULL);
		return NULL;
	}

	table = delete(table, key);
	assert(table != NULL && search(table, key) == NULL);

	return table;
}

static struct htable *search_test(struct htable *table, const int key)
{
	if (table == NULL) {
		assert(search(table, key) == NULL);
		return NULL;
	}

	search(table, key);
	assert(table != NULL);

	return table; 
}

static void run_test(struct htable *(*test)(struct htable *, const int),
		struct htable **table, size_t amount, ...)
{
	assert(amount > 0);

	va_list args;
	va_start(args, amount);
	
	while (amount-- > 0)
		*table = test(*table, va_arg(args, int));

	va_end(args);
}

void run_internal_tests()
{
	struct htable *table = htable(20);

	run_test(insert_test, &table, 9, 15, -7, 17, 3, 0, 15, 24361, 346, -5461);
	run_test(delete_test, &table, 3, 3, 0, 346);
	run_test(search_test, &table, 5, 3, -7, 346, 17, -7);

	free_table(table);
}


