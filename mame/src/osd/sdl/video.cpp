// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, R. Belmont
//============================================================
//
//  video.cpp - SDL video handling
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

#include "window.h"

// MAMEOS headers
#include "osdsdl.h"
#include "modules/lib/osdlib.h"
#include "modules/monitor/monitor_module.h"
#include "modules/render/render_module.h"

// MAME headers
#include "emu.h"
#include "emuopts.h"
#include "main.h"
#include "rendutil.h"
#include "uiinput.h"

#include <SDL2/SDL.h>

#ifdef SDLMAME_SWITCH
#include "SwitchPlatform.h"   // Model 2 NX
#include "nxlog.h"
#endif


//============================================================
//  GLOBAL VARIABLES
//============================================================

osd_video_config video_config;


//============================================================
//  PROTOTYPES
//============================================================

static void get_resolution(const char *defdata, const char *data, osd_window_config *config, int report_error);


//============================================================
//  video_init
//============================================================

bool sdl_osd_interface::video_init()
{
	int index;

	// extract data from the options
	extract_video_config();

	// we need the beam width in a float, contrary to what the core does.
	video_config.beamwidth = options().beam_width_min();

	// initialize the window system so we can make windows
	if (!window_init())
		return false;

	// create the windows
	for (index = 0; index < video_config.numscreens; index++)
	{
		osd_window_config conf;
		get_resolution(options().resolution(), options().resolution(index), &conf, true);

		// create window ...
		auto win = std::make_unique<sdl_window_info>(machine(), *m_render, index, m_monitor_module->pick_monitor(reinterpret_cast<osd_options &>(options()), index), &conf);
		if (win->window_init())
			return false;

		s_window_list.emplace_back(std::move(win));
	}

	if (m_render->is_interactive())
	{
		SDL_Window *const sdlwindow = dynamic_cast<sdl_window_info &>(*osd_common_t::s_window_list.front()).platform_window();
		SDL_RaiseWindow(sdlwindow);

#ifdef SDLMAME_MACOSX
		// ensure focus is acquired before the input modules start polling
		process_events();
		if (!has_focus())
			osd_printf_verbose("Window did not acquire input focus\n");
#endif
	}

	return true;
}

//============================================================
//  video_exit
//============================================================

void sdl_osd_interface::video_exit()
{
	window_exit();
}

//============================================================
//  update
//============================================================

void sdl_osd_interface::update(bool skip_redraw)
{
	osd_common_t::update(skip_redraw);

#ifdef SDLMAME_SWITCH
	// Model 2 NX: - and + together leave the game (back to the game list),
	// or MAME itself from the game list, whatever the input mapping is.
	if (SwitchExitComboPressed())
		machine().schedule_exit();
#endif

#ifdef SDLMAME_SWITCH
	// Model 2 NX: frame timing report in model2nx.log every ~3 seconds:
	// frames per second, the longest gap between two frames, and how much of
	// each frame is spent drawing (MAME's renderer and the buffer swap).
	static osd_ticks_t s_report_start = 0, s_last_update = 0, s_max_gap = 0, s_draw = 0;
	static unsigned s_updates = 0, s_drawn = 0;
	osd_ticks_t const tps = osd_ticks_per_second();
	osd_ticks_t const now = osd_ticks();
	if (!s_report_start)
		s_report_start = now;
	if (s_last_update && now - s_last_update > s_max_gap)
		s_max_gap = now - s_last_update;
	s_last_update = now;
	s_updates++;
#endif

	// if we're not skipping this redraw, update all windows
	if (!skip_redraw)
	{
#ifdef SDLMAME_SWITCH
		osd_ticks_t const draw_start = osd_ticks();
#endif
//      profiler_mark(PROFILER_BLIT);
		for (auto const &window : osd_common_t::window_list())
			window->update();
//      profiler_mark(PROFILER_END);
#ifdef SDLMAME_SWITCH
		s_draw += osd_ticks() - draw_start;
		s_drawn++;
#endif
	}

#ifdef SDLMAME_SWITCH
	if (now - s_report_start >= 3 * tps)
	{
		double const secs = double(now - s_report_start) / double(tps);
		NxLogPrintf("perf: %.1f updates/s, %.1f drawn/s, longest gap %.1f ms, draw %.2f ms/frame\n",
				s_updates / secs, s_drawn / secs,
				1000.0 * double(s_max_gap) / double(tps),
				s_drawn ? 1000.0 * double(s_draw) / double(tps) / s_drawn : 0.0);
		s_report_start = now;
		s_updates = s_drawn = 0;
		s_max_gap = s_draw = 0;
	}
#endif

	// if we're running, disable some parts of the debugger
	if ((machine().debug_flags & DEBUG_FLAG_OSD_ENABLED) != 0)
		debugger_update();
}

//============================================================
//  extract_video_config
//============================================================

void sdl_osd_interface::extract_video_config()
{
	video_config.perftest    = options().video_fps();

	// global options: extract the data
	video_config.windowed      = options().window();
	video_config.prescale      = options().prescale();
	video_config.filter        = options().filter();
	video_config.numscreens    = options().numscreens();
	#ifdef SDLMAME_X11
	video_config.restrictonemonitor = !options().use_all_heads();
	#endif

	// if we are in debug mode, never go full screen
	if (machine().debug_flags & DEBUG_FLAG_OSD_ENABLED)
		video_config.windowed = true;

	video_config.switchres     = options().switch_res();
	video_config.centerh       = options().centerh();
	video_config.centerv       = options().centerv();
	video_config.waitvsync     = options().wait_vsync();
	video_config.syncrefresh   = options().sync_refresh();
	if (!video_config.waitvsync && video_config.syncrefresh)
	{
		osd_printf_warning("-syncrefresh specified without -waitvsync. Reverting to -nosyncrefresh\n");
		video_config.syncrefresh = 0;
	}

	if (video_config.prescale < 1 || video_config.prescale > 20)
	{
		osd_printf_warning("Invalid prescale option, reverting to '1'\n");
		video_config.prescale = 1;
	}

	// misc options: sanity check values

	// global options: sanity check values
	if (video_config.numscreens < 1 || video_config.numscreens > MAX_VIDEO_WINDOWS)
	{
		osd_printf_warning("Invalid numscreens value %d; reverting to 1\n", video_config.numscreens);
		video_config.numscreens = 1;
	}
}


//============================================================
//  get_resolution
//============================================================

static void get_resolution(const char *defdata, const char *data, osd_window_config *config, int report_error)
{
	config->width = config->height = config->depth = config->refresh = 0;
	if (strcmp(data, OSDOPTVAL_AUTO) == 0)
	{
		if (strcmp(defdata, OSDOPTVAL_AUTO) == 0)
			return;
		data = defdata;
	}

	if (sscanf(data, "%dx%dx%d", &config->width, &config->height, &config->depth) < 2 && report_error)
		osd_printf_error("Illegal resolution value = %s\n", data);

	const char * at_pos = strchr(data, '@');
	if (at_pos)
		if (sscanf(at_pos + 1, "%d", &config->refresh) < 1 && report_error)
			osd_printf_error("Illegal refresh rate in resolution value = %s\n", data);
}
