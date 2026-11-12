/* Nintendo Switch system helpers (only built with __SWITCH__). */
#ifndef M2_SWITCH_SYS_H
#define M2_SWITCH_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

void m2sw_init(void);           /* create the app's folders on the SD card */
void m2sw_game_started(void);   /* CPU boost on, no auto-sleep */
void m2sw_game_stopped(void);   /* back to normal */
void m2sw_pin_thread(int core); /* run the calling thread on that core (0-2) */

#ifdef __cplusplus
}
#endif

#endif
