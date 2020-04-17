- [About](#about)
	- [Compilation](#compilation)
- [Separate Chaining](#separate-chaining)

# About

This is part of the university project at FIIT STU, Bratislava, Slovakia. Goal is to compare different implementations of hashing.

This project comes with a small interaction program - `hashio`. It can be used to manually insert, search, and delete keys in hash table, as well as print the entire table.

## Compilation

Project contains a `Makefile` to compile `hashio` program. Default setting is to compile it with *Separate Chaining* implementation, but you can change this behavior:

* You can manually specify default behavior in Makefile.
* You can pass `target=[file_name]` argument to `make` command.

If you want to compile program with different implementation, then you also need to change `#include` statement inside `hashio.c` to match implementation that you want to compile.

Target names are folder names that contain source files of the given implementation.

If you wish to run unit tests, you can add scenarios inside *assert* file of the tree and include its header in `hashio.c`. You will also be required to pass additional argument to `make` command in shell.

For example, if you've included `schain_hash_assert.h` header into `hashio.c` and wish to run tests. Then you can add function call inside `main`:

```c
/* hashio.c */

int main(void)
{
	run_internal_tests();
	...
}
```

This will run scenarios specified in assert file.

To compile program with tests enabled, you can run shell command:

```
$ make assert=true  
```

# Separate Chaining