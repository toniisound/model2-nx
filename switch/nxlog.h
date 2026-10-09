/*
 * Model 2 NX - Sega Model 2 for Nintendo Switch
 * Copyright (C) ToniiSound
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 */

/*
 * nxlog.h
 *
 * Log file for the Switch build: sdmc:/switch/model2nx/model2nx.log.
 *
 * Every line is appended with its own open/write/close, so whatever was
 * logged before a crash is on the SD card (a redirected stdout loses its
 * unwritten data when the program dies). Slow, so meant for messages, not
 * for anything called every frame.
 */

#ifndef MODEL2NX_NXLOG_H
#define MODEL2NX_NXLOG_H

// Empties the log. Call once at start-up.
void NxLogReset();

// Appends printf-style text (add the '\n' yourself).
void NxLogPrintf(const char *format, ...) __attribute__((format(printf, 1, 2)));

// Copies all of MAME's console messages (errors, warnings, info and verbose)
// to the log, on top of MAME's own output. Call once MAME's OSD object
// exists (it registers its own output handler when constructed), and
// NxLogStopMame() before it is destroyed.
void NxLogStartMame();
void NxLogStopMame();

#endif // MODEL2NX_NXLOG_H
