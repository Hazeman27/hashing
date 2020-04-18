#ifndef cuckoo_hash_H
#define cuckoo_hash_H

typedef struct ckhash_cell *Entry;
typedef struct ckhash_table *Table;

#define htable(size) ckh_alloc_table((size))

#define insert(table, key, value) ckh_insert((table), (key), (value))

#define search(table, key) ckh_get((table), (key))

#define delete(table, key) ckh_delete((table), (key))

#define free_table(table) ckh_destruct_table((table))

#define print_entry(entry) ckh_print_cell((entry))

#define print_table(table) ckh_print((table))

Table ckh_alloc_table(int table_size);

int ckh_insert(Table D, const unsigned char *key, int value);

int ckh_lookup(Table D, const unsigned char *key);

int ckh_delete(Table D, const unsigned char *key);

Entry ckh_get(Table D, const unsigned char *key);

Table ckh_destruct_table(Table D);

void ckh_print_cell(Entry entry);

void ckh_print(Table D);

#endif /* __cuckoo_hash_H */
