// license:GPL-3.0-or-later
/*
 * Model 2 NX - Sega Model 2 for Nintendo Switch
 *
 * linenoise_stub.c
 *
 * MAME's Lua engine binds linenoise (line editing for MAME's interactive Lua
 * console, -console). That needs a terminal (termios), which the Switch does
 * not have, so these stand-ins do nothing and read no input. Only the
 * functions the Lua binding calls are here.
 */

#include <stddef.h>

#include "linenoise.h"

linenoiseCompletionCallback *linenoiseSetCompletionCallback(linenoiseCompletionCallback *comp, void *userdata)
{
	(void)comp;
	(void)userdata;
	return NULL;
}

void linenoiseAddCompletion(linenoiseCompletions *comp, const char *str)
{
	(void)comp;
	(void)str;
}

char *linenoise(const char *prompt)
{
	(void)prompt;
	return NULL;   // end of input
}

void linenoiseClearScreen(void)
{
}

int linenoiseHistoryAdd(const char *line)
{
	(void)line;
	return 0;
}

int linenoiseHistorySetMaxLen(int len)
{
	(void)len;
	return 0;
}

int linenoiseHistorySave(const char *filename)
{
	(void)filename;
	return -1;
}

int linenoiseHistoryLoad(const char *filename)
{
	(void)filename;
	return -1;
}

char **linenoiseHistory(int *len)
{
	if (len)
		*len = 0;
	return NULL;
}
