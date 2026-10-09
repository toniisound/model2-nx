// license:GPL-3.0-or-later
/*
 * Model 2 NX - Sega Model 2 for Nintendo Switch
 *
 * sqlite_switch.c
 *
 * The Switch build compiles SQLite with SQLITE_OS_OTHER=1 (see the Makefile):
 * no Unix VFS, since newlib lacks the file locking it relies on. SQLite then
 * expects the program to provide these two functions, which would normally
 * register a VFS. None is registered: SQLite is only reached from MAME's Lua
 * engine (plugins, which mame.ini turns off), and opening a database simply
 * fails.
 */

int sqlite3_os_init(void)
{
	return 0;   // SQLITE_OK
}

int sqlite3_os_end(void)
{
	return 0;   // SQLITE_OK
}
