/* Nintendo Switch system helpers for the frontend (kept apart from the
   emulator's headers so libnx's names don't clash with them). */
#ifdef __SWITCH__
#include "switch_sys.h"

#include <switch.h>
#include <sys/stat.h>

void m2sw_init(void)
{
    /* the app's folders on the SD card, so the user can see where ROMs go */
    mkdir("sdmc:/switch", 0755);
    mkdir("sdmc:/switch/Model2EmuEx", 0755);
    mkdir("sdmc:/switch/Model2EmuEx/roms", 0755);
    mkdir("sdmc:/switch/Model2EmuEx/NVDATA", 0755);
}

void m2sw_pin_thread(int core)
{
    /* SDL starts its threads on the process's default core: spread them over
       the three cores an application gets (0-2) */
    svcSetThreadCoreMask(CUR_THREAD_HANDLE, core, 1u << core);
}

void m2sw_game_started(void)
{
    /* the i960 and TGP interpreters want every MHz: the CPU boost Nintendo's
       own games use while loading (1785 MHz), only while a game runs */
    appletSetCpuBoostMode(ApmCpuBoostMode_FastLoad);
    /* don't dim the screen or go to sleep in the middle of a race */
    appletSetMediaPlaybackState(true);
}

void m2sw_game_stopped(void)
{
    appletSetCpuBoostMode(ApmCpuBoostMode_Normal);
    appletSetMediaPlaybackState(false);
}
#endif
