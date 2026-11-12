/*
 * Model2-NX menus (the Switch build, and desktop builds with NX_UI=1),
 * laid out as SuperModel NX's: the game list with its logo header and
 * covers, the settings tabs (+), and the pause menu opened with - and +
 * during a game. The launcher itself keeps launcher.h's interface.
 */
#ifndef M2_LAUNCHER_NX_H
#define M2_LAUNCHER_NX_H

#ifdef __cplusplus
extern "C" {
#endif

enum { M2NX_NONE, M2NX_RESET, M2NX_EXIT, M2NX_CANCEL };

/* Pause menu. Opened by the game loop (which pauses the emulation); the
   loop hands it the pad buttons pressed while it is open (SDL game
   controller button numbers) and asks each frame whether something was
   chosen: m2nx_menu_take returns the choice once no button is held any
   more (so the game doesn't see them) and closes the menu. */
void m2nx_menu_open(void);
int  m2nx_menu_is_open(void);
void m2nx_menu_button(int sdl_button);
int  m2nx_menu_take(int buttons_held);

/* A short message at the bottom of the screen for a couple of seconds. */
void m2nx_message(const char *text);

/* Draws the pause menu, the message and the frame rate (if show_fps) over
   the frame just drawn, before the buffer swap. */
void m2nx_overlay_draw(int width, int height, float fps, int show_fps);
/* End of a game: frees the overlay's ImGui context. */
void m2nx_overlay_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif
