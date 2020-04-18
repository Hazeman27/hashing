#include <assert.h>
#include <stdarg.h>
#include "../include/schain_hash_augmented.h"

static struct htable *insert_test(struct htable *table, const char *key, int value)
{
	table = insert(table, key, value);
	assert(table != NULL && equals(search(table, key)->key, key));

	return table;
}

static struct htable *delete_test(struct htable *table, const char *key)
{
	if (table == NULL) {
		assert(delete(table, key) == NULL);
		return NULL;
	}

	table = delete(table, key);
	assert(table != NULL && search(table, key) == NULL);

	return table;
}

static struct htable *search_test(struct htable *table, const char *key)
{
	if (table == NULL) {
		assert(search(table, key) == NULL);
		return NULL;
	}

	search(table, key);
	assert(table != NULL);

	return table; 
}

static void run_test(struct htable *(*test)(struct htable *, const char *),
		struct htable **table, size_t amount, ...)
{
	assert(amount > 0);

	va_list args;
	va_start(args, amount);
	
	while (amount-- > 0)
		*table = test(*table, va_arg(args, char *));

	va_end(args);
}

void run_internal_tests()
{
	struct htable *table = htable(20);
	
	table = insert_test(table, "4fgml534a", 13657);
	table = insert_test(table, "avs3512", -657);
	table = insert_test(table, "a47hjj", 17);
	table = insert_test(table, "a47hjj", 17);
	table = insert_test(table, "bjte", 14326157);
	table = insert_test(table, "rttudsj", 44157);
	table = insert_test(table, "vczxa", 7922);
	table = insert_test(table, "nhkeeq", -12613);
	table = insert_test(table, "nhkeeq", -12613);
	table = insert_test(table, "yiewrundc", 171);
	table = insert_test(table, "dgariwv", 639);
	table = insert_test(table, "4yiet7iw", 91345);
	table = insert_test(table, "qwt358sdny", 71);
	table = insert_test(table, "qwt358sdny", 71);
	table = insert_test(table, "0jfsls1", -526);
	table = insert_test(table, "bnvbkwr68mb", 0);
	table = insert_test(table, "hjeolwrf", 6788);
	table = insert_test(table, "erq", 3613);

	run_test(delete_test, &table, 5, "erq", "nhkeeq", "bjte", "avs3512", "abc");
	run_test(search_test, &table, 5, "hjeolwrf", "0jfsls1", "abc", "bjte", "aaaa");

	free_table(table);
}


