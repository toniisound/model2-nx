/*
 * Model 2 NX - minimal <sys/utsname.h> for the Switch (newlib has none).
 *
 * asmjit's cpuinfo.cpp includes it on every non-Windows ARM target, but only
 * calls uname() on Linux. Declared here so the include works; never called.
 */

#ifndef MODEL2NX_SYS_UTSNAME_H
#define MODEL2NX_SYS_UTSNAME_H

#ifdef __cplusplus
extern "C" {
#endif

struct utsname
{
	char sysname[65];
	char nodename[65];
	char release[65];
	char version[65];
	char machine[65];
};

int uname(struct utsname *buf);

#ifdef __cplusplus
}
#endif

#endif /* MODEL2NX_SYS_UTSNAME_H */
