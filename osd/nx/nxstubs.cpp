// license:GPL-3.0-or-later
/*
 * Model 2 NX - "not supported" definitions for the OSD modules that
 * osd_common_t registers but this build leaves out.
 *
 * MAME's osdobj_common.cpp registers every module it knows about by name
 * (REGISTER_MODULE), so each one needs a definition even when its source file
 * is not built: BGFX, the Qt/ImGui/GDB-stub debuggers, TAP/TUN and pcap
 * networking and the network output server. These stand-ins say "not
 * supported", and MAME falls back to the next module of each kind (SDL 2D
 * renderer, no debugger, no network, console output).
 */

#include "modules/osdmodule.h"
#include "modules/debugger/debug_module.h"
#include "modules/netdev/netdev_module.h"
#include "modules/output/output_module.h"
#include "modules/render/render_module.h"

namespace osd {
namespace {

MODULE_NOT_SUPPORTED(video_bgfx_nx,    OSD_RENDERER_PROVIDER, "bgfx")
MODULE_NOT_SUPPORTED(debug_qt_nx,      OSD_DEBUG_PROVIDER,    "qt")
MODULE_NOT_SUPPORTED(debug_imgui_nx,   OSD_DEBUG_PROVIDER,    "imgui")
MODULE_NOT_SUPPORTED(debug_gdbstub_nx, OSD_DEBUG_PROVIDER,    "gdbstub")
MODULE_NOT_SUPPORTED(taptun_nx,        OSD_NETDEV_PROVIDER,   "taptun")
MODULE_NOT_SUPPORTED(pcap_nx,          OSD_NETDEV_PROVIDER,   "pcap")
MODULE_NOT_SUPPORTED(output_network_nx, OSD_OUTPUT_PROVIDER,  "network")

} // anonymous namespace
} // namespace osd

MODULE_DEFINITION(RENDERER_BGFX,  osd::video_bgfx_nx)
MODULE_DEFINITION(DEBUG_QT,       osd::debug_qt_nx)
MODULE_DEFINITION(DEBUG_IMGUI,    osd::debug_imgui_nx)
MODULE_DEFINITION(DEBUG_GDBSTUB,  osd::debug_gdbstub_nx)
MODULE_DEFINITION(NETDEV_TAPTUN,  osd::taptun_nx)
MODULE_DEFINITION(NETDEV_PCAP,    osd::pcap_nx)
MODULE_DEFINITION(OUTPUT_NETWORK, osd::output_network_nx)

#ifdef MODEL2NX_NO_FONT_SDL
// PC build without SDL_ttf/fontconfig: MAME uses its built-in UI font.
#include "modules/font/font_module.h"
namespace osd { namespace { MODULE_NOT_SUPPORTED(font_sdl_nx, OSD_FONT_PROVIDER, "sdl") } }
MODULE_DEFINITION(FONT_SDL, osd::font_sdl_nx)
#endif
