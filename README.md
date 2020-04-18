
- [About](#about)
	- [Compilation](#compilation)
	- [Unit tests](#unit-tests)
- [Separate Chaining](#separate-chaining)
	- [Introduction](#introduction)
	- [Implementation](#implementation)
- [Comparison](#comparison)
- [References](#references)

# About

This is part of the university project at FIIT STU, Bratislava, Slovakia. Goal is to compare different implementations of hashing.

This project comes with a small interaction program - `hashio`. It can be used to manually insert, search, and delete keys inside hash table, as well as print the entire table.

## Compilation

Project contains a `Makefile` to compile `hashio` program. Default setting is to compile it with *Separate Chaining* implementation, but you can change this behavior:

* You can manually specify default behavior in Makefile.
* You can pass `target=[implementation_name]` argument to `make` command.

If you want to compile program with different implementation, you also need to change `#include` statement inside `hashio.c` to match the name of the implementation.

Target name is the name of the folder that contains source files for the given implementation.

## Unit tests

If you wish to run unit tests, you can add scenarios inside *assert* file of the implementation and include its header in `hashio.c`. You will also be required to pass additional argument to `make` command in shell.

For example, if you've included `schain_hash_assert.h` header into `hashio.c` and wish to run tests. Then you can add function call inside `main`:

```c
/* hashio.c */

int main(void)
{
	run_internal_tests();
	...
}
```

This will run scenarios specified in the assert file.

To compile program with tests enabled, you can run shell command:

```
$ make assert=true  
```

# Separate Chaining

## Introduction

Separate Chaining is a resolution method for collisions inside hash table. In separate chaining, each bucket of the hash table is an independent list structure.

## Implementation

This project includes implementation of the separate chaining with simple linked list structure. It is based on this [wikipedia article][1]. Every collision is added to the end of the list. Implementation includes *insert*, *delete*, and *search* operations.

### Definition

Hash table consists of buckets. Each bucket has a list of entries. Single entry can be represented with a structure:

```c
struct entry {
	int key;
	bool deleted;
	struct entry *next;
};
```

Structure member `deleted` indicates whether the given entry has been deleted from the table.

With our bucket entries defined, we can represent hash table with a structure:

```c
struct htable {
	size_t size;
	struct entry **buckets;
}
```

Structure has an array of *buckets*, where each bucket is an array of *entries*. It also has a `size` member that stores the size of the buckets array.

### Hash function

Hash table requires *hash function* that will calculate the index for the given key. In our case, it is a simple modulo function:

```c
size_t hash(const int key, const size_t table_size)
{
	return key % table_size;
}
```

Advantage of this function is that it is very fast. But to mitigate high amount of collisions that it may produce, it requires a table size that is a *prime number*. And to further reduce collisions, prime number chosen as the table size, needs to be as far as possible from the nearest *power of two*.

All of the above can be written in a simple function:

```c
size_t nearest_prime(const size_t n)
{
	if (n > 0 && !ispow2(n - 1) && !ispow2(n + 1) && isprime(n))
		return n;
	
	return nearest_prime(n + 1);
}
```

To make the usage of the hash table easier, this function is called during the allocation to *tweak* the size.

### Insert

Insertion is a process of hashing the key and placing it into the table. After the hashing, 4 cases may occur:

* entry is empty;
* entry is deleted;
* entry is occupied;
* entry contains the same key (duplicate).

First two cases are simple: we just insert the new key into that entry.

In the third case, we start traversing the linked list of entries until we reach the end or find a deleted entry, then we insert the key.

In the fourth case, we raise an error and return from the function.

### Rehashing

After the insertion we need to check whether we neeed to rehash our table. *Rehashing* is a process, where new larger hash table is allocated, all keys from the old table are rehashed and placed into the new table, and the old table is freed. In this implementation, size of the new table is multiplied by 2 (*this size is further increased by finding the nearest prime number*). 

```c
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

```

To know when to rehash the table, we need to calculate the *load factor* of the table. Load factor is the ratio between the number of the elements table currently holds and the size of the table. When this ratio becomes larger then `0.5`, we rehash the table: 

```c	
if (load_factor(table) > 0.5f)
	return rehash_table(table);
```

### Delete

Deletion of the key is relatively simple:

* Hash the key and find its bucket.
* Find the key in the list of entries.
* If entry is found, mark it as *deleted*.

```c
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
```

### Search

Searching for the key is straightforward:

* Hash the key and find its bucket.
* Find the key in the list of entries.
* If entry is found, return it.

```c
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
```

# Other implementation
# Comparison
# References

* [en.wikipedia.org, Hash Table][1]

[1]: https://en.wikipedia.org/wiki/Hash_table 
