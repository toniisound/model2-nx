/*
 * Model2-NX launcher and in-game overlay, laid out as SuperModel NX's menu
 * (toniisound/supermodel-nx, Src/OSD/SDL/Gui.cpp and Src/OSD/Switch/
 * InGameMenu.cpp): a logo header, the game list with the cover of the game
 * under the cursor, the settings behind +, and the pause menu behind - and +.
 *
 * Built for the Switch, or on a desktop with `make NX_UI=1` (to try it).
 */
#include "launcher.h"
#include "launcher_nx.h"

#include "../m2/m2glapi.h"
#include "../m2/m2rom.h"
#include "dirlist.h"
#include "imgui_impl_m2gl.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"

#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <deque>
#include <map>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <vector>

#ifdef __SWITCH__
#include <switch.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_NO_STDIO_WARNINGS
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include "stb_image.h"
#pragma GCC diagnostic pop

#define APP_NAME    "Model2-NX"
#define APP_VERSION "1.0"

#ifdef __SWITCH__
#define BASE_DIR "sdmc:/switch/Model2-NX"
#else
#define BASE_DIR "."
#endif

static bool g_ready;

/* ================================================================ helpers */

static bool file_exists(const std::string &path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

static bool checkbox(const char *label, int *v)
{
    bool b = *v != 0;
    if (!ImGui::Checkbox(label, &b))
        return false;
    *v = b;
    return true;
}

static bool combo(const char *label, int *cur, const char *const *items, int n)
{
    return ImGui::Combo(label, cur, items, n);
}

/* ================================================================ games */

struct game_entry {
    const m2_game *g;
    std::string name, title, parent, board;
};

static std::vector<game_entry> g_all;         /* Model 2 / 2A games, by ROM name */
static std::set<std::string> g_installed;     /* zips in the ROM folder */
static std::string g_scanned;

static bool supported(const m2_game *g)
{
    /* the SHARC of Model 2B / 2C isn't emulated: only Model 2 and 2A */
    return g->board && (!strcmp(g->board, "Model 2") || !strcmp(g->board, "Model 2A"));
}

static void scan(const char *romdir)
{
    if (g_all.empty()) {
        int n = 0;
        const m2_game *list = m2_game_list(&n);
        for (int i = 0; i < n; i++)
            if (supported(&list[i])) {
                game_entry e;
                e.g = &list[i];
                e.name = list[i].name;
                e.title = list[i].title;
                e.parent = list[i].parent && strcmp(list[i].parent, list[i].shared ? list[i].shared : "")
                         ? list[i].parent : "";
                e.board = list[i].board;
                g_all.push_back(e);
            }
        std::sort(g_all.begin(), g_all.end(),
                  [](const game_entry &a, const game_entry &b) { return a.name < b.name; });
    }
    g_installed.clear();
    for (const game_entry &e : g_all)
        if (file_exists(std::string(romdir) + "/" + e.name + ".zip"))
            g_installed.insert(e.name);
    g_scanned = romdir;
}

/* ================================================================ logo and covers */

struct picture {
    GLuint tex = 0;
    int w = 0, h = 0;
};

static GLuint upload_rgba(const unsigned char *px, int w, int h)
{
    GLuint t = 0;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, px);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    return t;
}

/* The logo comes from inside the .nro (RomFS, Assets/logo.bmp in the
   project; 32-bit BMP for transparency). Without it the name is written. */
static picture g_logo;
static bool g_logo_tried;

static void load_logo()
{
    g_logo_tried = true;
#ifdef __SWITCH__
    const char *path = "romfs:/Assets/logo.bmp";
    romfsInit();
#else
    const char *path = "Assets/logo.bmp";
#endif
    SDL_Surface *bmp = SDL_LoadBMP(path);
    if (!bmp)
        return;
    SDL_Surface *rgba = SDL_ConvertSurfaceFormat(bmp, SDL_PIXELFORMAT_ABGR8888, 0);   /* R,G,B,A bytes */
    SDL_FreeSurface(bmp);
    if (!rgba)
        return;
    std::vector<unsigned char> px((size_t)rgba->w * rgba->h * 4);
    for (int y = 0; y < rgba->h; y++)
        memcpy(&px[(size_t)y * rgba->w * 4], (const unsigned char *)rgba->pixels + (size_t)y * rgba->pitch,
               (size_t)rgba->w * 4);
    g_logo.tex = upload_rgba(px.data(), rgba->w, rgba->h);
    g_logo.w = rgba->w;
    g_logo.h = rgba->h;
    SDL_FreeSurface(rgba);
}

static void free_logo()
{
    if (g_logo.tex)
        glDeleteTextures(1, &g_logo.tex);
    g_logo = picture();
    g_logo_tried = false;
}

/* Covers, provided by the player: Covers/<ROM set>.png (or .jpg / .jpeg) in
   the app's folder, e.g. Covers/daytona.png. A clone without its own cover
   uses its parent's. None are included. */
static std::map<std::string, picture> g_covers;   /* tried sets, with or without a cover */
static std::deque<std::string> g_cover_order;     /* loaded textures, oldest first */
static const size_t k_max_covers = 12;
static std::string g_cover_game, g_cover_parent;  /* game under the list cursor */

static picture load_cover(const std::string &set)
{
    picture c;
    for (const char *ext : { ".png", ".jpg", ".jpeg", ".PNG", ".JPG" }) {
        const std::string path = std::string(BASE_DIR "/Covers/") + set + ext;
        int w, h, n;
        unsigned char *px = stbi_load(path.c_str(), &w, &h, &n, 4);
        if (!px)
            continue;
        while (w > 1024 || h > 1024) {   /* halve big pictures (box filter) */
            const int nw = w / 2, nh = h / 2;
            for (int y = 0; y < nh; y++)
                for (int x = 0; x < nw; x++) {
                    const unsigned char *a = px + ((2 * y) * w + 2 * x) * 4, *b = a + w * 4;
                    unsigned char *d = px + (y * nw + x) * 4;
                    for (int k = 0; k < 4; k++)
                        d[k] = (unsigned char)((a[k] + a[k + 4] + b[k] + b[k + 4] + 2) / 4);
                }
            w = nw;
            h = nh;
        }
        c.tex = upload_rgba(px, w, h);
        c.w = w;
        c.h = h;
        stbi_image_free(px);
        break;
    }
    return c;
}

static const picture &get_cover(const std::string &set)
{
    auto it = g_covers.find(set);
    if (it != g_covers.end())
        return it->second;
    picture c = load_cover(set);
    if (c.tex) {   /* keep a few, so going back up the list is instant */
        g_cover_order.push_back(set);
        if (g_cover_order.size() > k_max_covers) {
            auto old = g_covers.find(g_cover_order.front());
            if (old != g_covers.end()) {
                glDeleteTextures(1, &old->second.tex);
                g_covers.erase(old);
            }
            g_cover_order.pop_front();
        }
    }
    return g_covers[set] = c;
}

static void free_covers()
{
    for (auto &c : g_covers)
        if (c.second.tex)
            glDeleteTextures(1, &c.second.tex);
    g_covers.clear();
    g_cover_order.clear();
}

static void draw_cover_panel(const ImVec2 &size)
{
    ImGui::BeginChild("Cover", size, true,
                      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav);
    const ImVec2 avail = ImGui::GetContentRegionAvail();
    const picture *c = NULL;
    if (!g_cover_game.empty()) {
        c = &get_cover(g_cover_game);
        if (!c->tex && !g_cover_parent.empty())
            c = &get_cover(g_cover_parent);
    }
    if (c && c->tex) {   /* fitted, in proportion, centred */
        float w = avail.x, h = w * float(c->h) / float(c->w);
        if (h > avail.y) { h = avail.y; w = h * float(c->w) / float(c->h); }
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (avail.x - w) * 0.5f,
                                   ImGui::GetCursorPosY() + (avail.y - h) * 0.5f));
        ImGui::Image((ImTextureID)(intptr_t)c->tex, ImVec2(w, h));
    } else if (!g_cover_game.empty()) {
        const char *text = "No cover";
        const std::string hint = "Covers/" + g_cover_game + ".png";
        const float line = ImGui::GetTextLineHeightWithSpacing();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (avail.y - line * 2.0f) * 0.5f);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (avail.x - ImGui::CalcTextSize(text).x) * 0.5f);
        ImGui::TextDisabled("%s", text);
        ImGui::SetCursorPosX(ImGui::GetStyle().WindowPadding.x +
                             std::max(0.0f, (avail.x - ImGui::CalcTextSize(hint.c_str()).x) * 0.5f));
        ImGui::TextDisabled("%s", hint.c_str());
    }
    ImGui::EndChild();
}

/* ================================================================ screens */

static void draw_header(bool &toggle_settings)
{
    if (!g_logo_tried)
        load_logo();
    const float header_h = 130.0f;
    ImGui::BeginChild("Header", ImVec2(0.0f, header_h), false,
                      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav);
    const float avail = ImGui::GetContentRegionAvail().x;
    if (g_logo.tex) {
        const float max_w = avail * 0.9f;
        float h = header_h - 10.0f, w = h * float(g_logo.w) / float(g_logo.h);
        if (w > max_w) { w = max_w; h = w * float(g_logo.h) / float(g_logo.w); }
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (avail - w) * 0.5f, (header_h - h) * 0.5f));
        ImGui::Image((ImTextureID)(intptr_t)g_logo.tex, ImVec2(w, h));
    } else {
        ImGui::PushFont(NULL, ImGui::GetStyle().FontSizeBase * 2.0f);
        const ImVec2 size = ImGui::CalcTextSize(APP_NAME);
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (avail - size.x) * 0.5f, (header_h - size.y) * 0.5f));
        ImGui::TextUnformatted(APP_NAME);
        ImGui::PopFont();
    }
    ImGui::EndChild();

    const char *hint = "(+) Settings";   /* its own line under the logo, on the right */
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(hint).x);
    ImGui::TextDisabled("%s", hint);
    if (ImGui::IsItemClicked())
        toggle_settings = true;
}

/* rows shown: every supported game, or only those in the ROM folder */
static std::vector<const game_entry *> shown_games(const m2_options *o)
{
    std::vector<const game_entry *> v;
    for (const game_entry &e : g_all)
        if (!o->only_installed || g_installed.count(e.name))
            v.push_back(&e);
    return v;
}

/* row the pad cursor is on; the cursor never leaves the rows (no focus on
   the list's window itself): when no row has it, it goes back to this one */
static int g_cursor_row = -1;
static bool g_refocus;

static void draw_game_list(const std::vector<const game_entry *> &games, int &selected, bool &chosen, bool focus)
{
    /* the list on the left, the cover of the game under the cursor on the right */
    const float cover_w = std::floor(ImGui::GetContentRegionAvail().x * 0.30f);
    const float list_w = ImGui::GetContentRegionAvail().x - cover_w - ImGui::GetStyle().ItemSpacing.x;
    ImGui::BeginChild("TableRegion", ImVec2(list_w, 0.0f), ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened);
    if (focus)
        g_cursor_row = selected >= 0 ? selected : 0;
    if (g_cursor_row >= (int)games.size())
        g_cursor_row = (int)games.size() - 1;
    const bool put_cursor = focus || g_refocus;
    bool any_focused = false;
    ImGui::PushFont(NULL, ImGui::GetStyle().FontSizeBase * 0.85f);   /* a bit smaller, so the columns fit */

    const ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit |
                                  ImGuiTableFlags_ScrollY;
    if (ImGui::BeginTable("Games", 3, flags, ImVec2(0.0f, ImGui::GetContentRegionAvail().y))) {
        /* TITLE takes the room left; ROM and BOARD are as wide as their contents */
        ImGui::TableSetupColumn("TITLE", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("ROM", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("BOARD", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupScrollFreeze(0, 1);   /* the header row stays on screen */
        /* plain header row: labels only, so the pad cursor can't land on them */
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        for (int col = 0; col < ImGui::TableGetColumnCount(); col++) {
            ImGui::TableSetColumnIndex(col);
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImGuiCol_TableHeaderBg));
            ImGui::TextUnformatted(ImGui::TableGetColumnName(col));
        }

        for (int row = 0; row < (int)games.size(); row++) {
            const game_entry &g = *games[row];
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (!g_installed.count(g.name))
                ImGui::TextDisabled("%s", g.title.c_str());
            else
                ImGui::Text("%s", g.title.c_str());
            ImGui::TableSetColumnIndex(1);
            if (put_cursor && row == g_cursor_row)
                ImGui::SetKeyboardFocusHere();   /* the pad cursor on the selected game */
            if (ImGui::Selectable(g.name.c_str(), selected == row, ImGuiSelectableFlags_SpanAllColumns)) {
                chosen = true;   /* A starts the game straight away */
                selected = row;
            }
            if (ImGui::IsItemFocused()) {
                g_cursor_row = row;
                any_focused = true;
            }
            if (ImGui::IsItemFocused() || ImGui::IsItemHovered() ||
                (g_cover_game.empty() && row == std::max(selected, 0))) {
                g_cover_game = g.name;
                g_cover_parent = g.parent;
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                chosen = true;
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", g.board.c_str());
        }
        ImGui::EndTable();
    }
    ImGui::PopFont();
    ImGui::EndChild();
    /* the cursor left the rows (B, or past the ends): put it back next frame */
    g_refocus = !any_focused && !games.empty() && !ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId);
    ImGui::SameLine();
    draw_cover_panel(ImVec2(cover_w, 0.0f));
}

static void draw_credits()
{
    ImGui::BeginChild("CreditsText", ImVec2(0.0f, 0.0f), false);
    ImGui::PushFont(NULL, ImGui::GetStyle().FontSizeBase * 1.5f);
    ImGui::TextUnformatted(APP_NAME " " APP_VERSION);
    ImGui::PopFont();
    ImGui::TextUnformatted("Nintendo Switch fork created by ToniiSound");

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    ImGui::TextDisabled("Based on");
    ImGui::BulletText("Model2EmuEx");
    ImGui::Indent();
    ImGui::TextWrapped("by octavioCuatrochio: the Ghidra decompilation of ElSemi's emulator and its "
                       "portable C port this fork is built on. github.com/octavioCuatrochio/Model2EmuEx");
    ImGui::Unindent();
    ImGui::BulletText("Sega Model 2 Emulator 1.1a");
    ImGui::Indent();
    ImGui::TextWrapped("by ElSemi: the original Model 2 emulator for Windows.");
    ImGui::Unindent();

    ImGui::Spacing();
    ImGui::TextDisabled("Libraries and tools");
    ImGui::BulletText("devkitPro and libnx (Switch homebrew toolchain and library)");
    ImGui::BulletText("SDL2, Mesa (OpenGL)");
    ImGui::BulletText("Dear ImGui by Omar Cornut");
    ImGui::BulletText("Musashi 68000 core by Karl Stenerud");
    ImGui::BulletText("ymfm by Aaron Giles");
    ImGui::BulletText("zlib");
    ImGui::BulletText("stb_image by Sean Barrett (game covers)");

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    ImGui::TextWrapped("Free software, with no warranty.");
    ImGui::TextWrapped("Sega, Model 2 and the game titles are trademarks of their owners. No ROMs are "
                       "included; use only games you own.");
    ImGui::EndChild();
}

/* ---------------------------------------------------------------- settings tabs */

static bool tab_general(m2_options *o, int &selected)
{
    bool c = false;
    c |= checkbox("Show FPS on screen", &o->show_fps);
    if (checkbox("Only show games found in ROMs folder", &o->only_installed)) {
        selected = -1;   /* row numbers refer to the other list now */
        c = true;
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(%d found)", (int)g_installed.size());
    c |= checkbox("Daytona USA: two players, linked (split screen)", &o->coop);
    {
        static const char *const splits[] = { "Side by side", "One above the other" };
        int s = o->split_side ? 0 : 1;
        if (combo("Linked play layout", &s, splits, 2)) { o->split_side = s == 0; c = true; }
    }

    ImGui::Spacing();
    if (ImGui::Button("Load Defaults"))
        ImGui::OpenPopup("Confirm Load Defaults");
    if (ImGui::BeginPopupModal("Confirm Load Defaults", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Restore the default settings (video, audio and core)?");
        ImGui::Separator();
        if (ImGui::Button("Yes", ImVec2(120, 0))) {
            char romdir[sizeof o->romdir];
            snprintf(romdir, sizeof romdir, "%s", o->romdir);
            m2opt_defaults(o);
            snprintf(o->romdir, sizeof o->romdir, "%s", romdir);
            c = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
    return c;
}

static bool tab_core(m2_options *o)
{
    bool c = false;
    c |= checkbox("Pipeline (emulate on a separate core)", &o->pipelined);
    c |= checkbox("Frame limiter (57.5 FPS)", &o->frame_cap);
    c |= checkbox("VSync", &o->vsync);
    return c;
}

static bool tab_video(m2_options *o)
{
    bool c = false;
    {
        static const char *const scales[] = { "Auto (fits the screen)", "1x (496x384, fastest)",
                                              "2x (992x768)", "3x (1488x1152, best quality)" };
        int s = o->scale >= 0 && o->scale <= 3 ? o->scale : 0;
        ImGui::SetNextItemWidth(ImGui::CalcTextSize(scales[3]).x + 60.0f);
        if (combo("Resolution", &s, scales, 4)) { o->scale = s; c = true; }
    }
    {
        static const char *const wides[] = { "Off (4:3)", "16:9" };
        int w = o->wide == M2_WIDE_RATIO || o->wide == M2_WIDE_FILL ? 1 : 0;
        if (combo("Widescreen", &w, wides, 2)) {
            o->wide = w ? M2_WIDE_RATIO : M2_WIDE_DEFAULT;
            o->wide_ratio = 16.0 / 9.0;
            c = true;
        }
    }
    {
        static const char *const tfilters[] = { "Nearest (as the hardware)", "Bilinear", "Trilinear" };
        int f = o->tex_filter >= 0 && o->tex_filter <= 2 ? o->tex_filter : 0;
        if (combo("Texture filter", &f, tfilters, 3)) { o->tex_filter = f; c = true; }
    }
    {
        static const char *const filters[] = { "Smooth", "Sharp" };
        int f = o->smooth ? 0 : 1;
        if (combo("Scaling filter", &f, filters, 2)) { o->smooth = f == 0; c = true; }
    }
    {
        static const char *const meshes[] = { "Blend (50% see-through)", "Checker (as the hardware)" };
        int m = o->mesh_blend ? 0 : 1;
        if (combo("Shadows and fences", &m, meshes, 2)) { o->mesh_blend = m == 0; c = true; }
    }
    {
        static const char *const aspects[] = { "Keep (bands)", "Stretch", "Crop" };
        int a = o->aspect >= 0 && o->aspect <= 2 ? o->aspect : 0;
        if (combo("Linked play picture shape", &a, aspects, 3)) { o->aspect = a; c = true; }
    }
    c |= ImGui::SliderFloat("Saturation", &o->saturation, 0.5f, 2.0f, "%.2f");
    float g = o->gamma[0];
    if (ImGui::SliderFloat("Gamma", &g, 0.5f, 2.0f, "%.2f")) {
        o->gamma[0] = o->gamma[1] = o->gamma[2] = g;
        c = true;
    }
    return c;
}

static bool tab_audio(m2_options *o)
{
    return ImGui::SliderInt("Volume", &o->volume, 0, 200, "%d%%");
}

/* ================================================================ launcher */

int m2launch_init(SDL_Window *win, SDL_GLContext ctx, int es)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    io.Fonts->AddFontDefaultVector();
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.FontScaleMain = 1.5f;   /* readable on a TV and on the handheld screen */
    style.ScaleAllSizes(1.5f);
    if (!ImGui_ImplSDL2_InitForOpenGL(win, ctx) || !ImGui_ImplM2GL_Init(es != 0))
        return -1;
    g_ready = true;
    return 0;
}

void m2launch_shutdown(void)
{
    if (!g_ready)
        return;
    free_logo();
    free_covers();
    ImGui_ImplM2GL_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    g_ready = false;
}

static void save_shot(const char *path, int w, int h)   /* debugging: M2EMU_GUI_SHOT */
{
    std::vector<unsigned char> px((size_t)w * h * 4);
    FILE *f = fopen(path, "wb");
    if (!f)
        return;
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
    fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (int y = h - 1; y >= 0; y--)
        for (int x = 0; x < w; x++)
            fwrite(&px[((size_t)y * w + x) * 4], 1, 3, f);
    fclose(f);
}

int m2launch_run(SDL_Window *win, m2_options *o, const char *cfg_path, char *game, int game_size,
                 const char *message)
{
    SDL_SetWindowTitle(win, APP_NAME);
    SDL_GL_SetSwapInterval(1);
    scan(o->romdir);
    if (g_installed.empty())
        o->only_installed = 0;   /* nothing found: show the whole list */

    static bool show_settings = false;
    static bool focus_pending = true;
    focus_pending = true;
    std::vector<const game_entry *> games = shown_games(o);
    int selected = -1;
    for (int i = 0; i < (int)games.size(); i++)
        if (games[i]->name == game)
            selected = i;

    std::string popup_msg = message ? message : "";
    bool popup_start = !popup_msg.empty();
    std::string missing_list, missing_title;

    /* debugging: M2EMU_GUI_SHOT=file.ppm:frames, M2EMU_GUI_TAB=games|general|core|video|audio|credits,
       M2EMU_NX_MENU=1 (the pause menu over the list) */
    char shot_path[512] = "";
    int shot_frames = 0, frame = 0;
    if (getenv("M2EMU_GUI_SHOT")) {
        snprintf(shot_path, sizeof shot_path, "%s", getenv("M2EMU_GUI_SHOT"));
        char *cl = strrchr(shot_path, ':');
        if (cl) { *cl = 0; shot_frames = atoi(cl + 1); }
    }
    const char *want_tab = getenv("M2EMU_GUI_TAB");
    if (want_tab && strcmp(want_tab, "games"))
        show_settings = true;
    if (getenv("M2EMU_NX_MENU"))
        m2nx_menu_open();

    int result = -1;
    while (result < 0) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT)
                result = 0;   /* HOME closes the program */
        }

        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGuiIO &io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("Custom Window", NULL, ImGuiWindowFlags_NoTitleBar);

        bool toggle_settings = false, changed = false, chosen = false;
        const bool popup_open = ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId);
        if (!popup_open && (ImGui::IsKeyPressed(ImGuiKey_GamepadStart, false) || ImGui::IsKeyPressed(ImGuiKey_F1, false)))
            toggle_settings = true;

        if (!show_settings) {
            draw_header(toggle_settings);
            draw_game_list(games, selected, chosen, focus_pending);
            focus_pending = false;

            /* don't leave the menu for a game whose ROMs aren't all there: say which */
            if (chosen && selected >= 0) {
                const game_entry &g = *games[selected];
                const char *dirs[] = { o->romdir, NULL };
                char buf[2048] = "";
                int total = 0;
                if (!g_installed.count(g.name)) {
                    missing_title = "This game is not on the SD card.";
                    missing_list = "ROMs/" + g.name + ".zip";
                    chosen = false;
                } else if (m2_check_roms(g.g, dirs, &total)) {
                    m2_missing_roms(g.g, dirs, buf, sizeof buf);
                    missing_title = "Some ROM files of this game are missing or don't match:";
                    missing_list = buf;
                    chosen = false;
                }
                if (!chosen)
                    ImGui::OpenPopup("ROM not found");
            }
            if (ImGui::BeginPopupModal("ROM not found", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextUnformatted(missing_title.c_str());
                ImGui::TextDisabled("%s", missing_list.c_str());
                ImGui::Separator();
                if (ImGui::Button("OK", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
            if (popup_start) {
                ImGui::OpenPopup("Couldn't start the game");
                popup_start = false;
            }
            if (ImGui::BeginPopupModal("Couldn't start the game", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextUnformatted(popup_msg.c_str());
                ImGui::Separator();
                if (ImGui::Button("OK", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        } else {
            if (focus_pending) {
                ImGui::SetKeyboardFocusHere();
                focus_pending = false;
            }
            if (ImGui::Button("Back"))
                toggle_settings = true;
            ImGui::SameLine();
            ImGui::TextDisabled("(+) Back to game list");
            ImGui::Spacing();

            /* the five tabs share the full width */
            const int tab_count = 5;
            const float tab_w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemInnerSpacing.x * (tab_count - 1)) /
                                tab_count;
            auto full_width_tab = [tab_w]() { ImGui::SetNextItemWidth(tab_w); };
            auto flag = [&](const char *name) {
                return frame == 0 && want_tab && !strcmp(want_tab, name) ? ImGuiTabItemFlags_SetSelected : 0;
            };
            if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_FittingPolicyResizeDown)) {
                full_width_tab();
                if (ImGui::BeginTabItem("General", NULL, flag("general"))) {
                    if (tab_general(o, selected)) {
                        changed = true;
                        games = shown_games(o);
                    }
                    ImGui::EndTabItem();
                }
                full_width_tab();
                if (ImGui::BeginTabItem("Core", NULL, flag("core"))) {
                    changed |= tab_core(o);
                    ImGui::EndTabItem();
                }
                full_width_tab();
                if (ImGui::BeginTabItem("Video", NULL, flag("video"))) {
                    changed |= tab_video(o);
                    ImGui::EndTabItem();
                }
                full_width_tab();
                if (ImGui::BeginTabItem("Audio", NULL, flag("audio"))) {
                    changed |= tab_audio(o);
                    ImGui::EndTabItem();
                }
                full_width_tab();
                if (ImGui::BeginTabItem("Credits", NULL, flag("credits"))) {
                    draw_credits();
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }

        if (toggle_settings) {
            show_settings = !show_settings;
            focus_pending = true;
            if (!show_settings)
                m2opt_save(o, cfg_path, game);   /* settings kept when going back to the list */
        }
        ImGui::End();

        ImGui::Render();
        int w, h;
        SDL_GL_GetDrawableSize(win, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.0f, 0.5f, 192 / 255.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplM2GL_RenderDrawData(ImGui::GetDrawData());
        if (m2nx_menu_is_open())
            m2nx_overlay_draw(w, h, 57.5f, 1);

        if (chosen && selected >= 0) {
            snprintf(game, (size_t)game_size, "%s", games[selected]->name.c_str());
            m2opt_save(o, cfg_path, game);
            result = 1;
        } else if (changed) {
            m2opt_save(o, cfg_path, game);
        }
        if (++frame >= shot_frames && shot_frames && shot_path[0]) {
            save_shot(shot_path, w, h);
            result = 0;
        }
        SDL_GL_SwapWindow(win);
    }
    if (result == 0)
        m2opt_save(o, cfg_path, game);
    return result;
}

/* ================================================================ pause menu and overlay */

namespace {
struct menu_item { const char *label; int action; };
const menu_item k_items[] = {
    { "Reset",  M2NX_RESET },
    { "Exit",   M2NX_EXIT },
    { "Cancel", M2NX_CANCEL },
};
const int k_item_count = (int)(sizeof k_items / sizeof k_items[0]);

ImGuiContext *s_ctx;
bool s_open;
int s_selected;
int s_pending = M2NX_NONE;   /* chosen, waiting for the buttons to be released */
std::string s_message;
Uint64 s_message_end;

bool ensure_context()
{
    if (s_ctx)
        return true;
    ImGuiContext *previous = ImGui::GetCurrentContext();
    s_ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(s_ctx);
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.Fonts->AddFontDefaultVector();
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.FontScaleMain = 1.6f;
    style.ScaleAllSizes(1.6f);
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    ImGui_ImplM2GL_InitContext();
    ImGui::SetCurrentContext(previous);
    return true;
}
}

void m2nx_menu_open(void)
{
    s_open = true;
    s_selected = 0;
    s_pending = M2NX_NONE;
}

int m2nx_menu_is_open(void) { return s_open; }

void m2nx_menu_button(int b)
{
    if (!s_open || s_pending != M2NX_NONE)
        return;
    if (b == SDL_CONTROLLER_BUTTON_DPAD_UP)
        s_selected = (s_selected + k_item_count - 1) % k_item_count;
    else if (b == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
        s_selected = (s_selected + 1) % k_item_count;
    else if (b == SDL_CONTROLLER_BUTTON_A)
        s_pending = k_items[s_selected].action;
    else if (b == SDL_CONTROLLER_BUTTON_B)
        s_pending = M2NX_CANCEL;
}

int m2nx_menu_take(int buttons_held)
{
    if (!s_open || s_pending == M2NX_NONE || buttons_held)
        return M2NX_NONE;
    const int a = s_pending;
    s_pending = M2NX_NONE;
    s_open = false;
    return a;
}

void m2nx_message(const char *text)
{
    s_message = text;
    s_message_end = SDL_GetPerformanceCounter() + 2 * SDL_GetPerformanceFrequency();
}

void m2nx_overlay_draw(int width, int height, float fps, int show_fps)
{
    const bool show_message = !s_message.empty() && SDL_GetPerformanceCounter() < s_message_end;
    if (!show_message)
        s_message.clear();
    if (!s_open && !show_message && !show_fps)
        return;
    if (!ensure_context())
        return;

    ImGuiContext *previous = ImGui::GetCurrentContext();
    ImGui::SetCurrentContext(s_ctx);
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(float(width), float(height));
    io.DeltaTime = 1.0f / 60.0f;
    ImGui::NewFrame();

    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs |
                                   ImGuiWindowFlags_NoFocusOnAppearing;
    if (show_fps && !s_open) {
        ImGui::SetNextWindowPos(ImVec2(12.0f, 12.0f));
        ImGui::SetNextWindowBgAlpha(0.55f);
        ImGui::Begin("##fps", NULL, flags);
        ImGui::Text("%.1f FPS", fps);
        ImGui::End();
    }
    if (s_open) {
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), io.DisplaySize, IM_COL32(0, 0, 0, 140));
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always,
                                ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0.92f);
        ImGui::Begin("##ingamemenu", NULL, flags);
        ImGui::TextUnformatted("Paused");
        ImGui::Separator();
        const float item_w = ImGui::CalcTextSize("Load State (empty)").x + ImGui::GetStyle().FramePadding.x * 8;
        for (int i = 0; i < k_item_count; i++)
            ImGui::Selectable(k_items[i].label, i == s_selected, 0, ImVec2(item_w, 0));
        ImGui::Separator();
        ImGui::TextDisabled("A: select   B: back");
        ImGui::End();
    }
    if (show_message) {
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y - 40.0f), ImGuiCond_Always,
                                ImVec2(0.5f, 1.0f));
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("##ingamemessage", NULL, flags);
        ImGui::TextUnformatted(s_message.c_str());
        ImGui::End();
    }
    ImGui::Render();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ImGui_ImplM2GL_RenderDrawData(ImGui::GetDrawData());
    ImGui::SetCurrentContext(previous);
}

void m2nx_overlay_shutdown(void)
{
    s_open = false;
    s_pending = M2NX_NONE;
    s_message.clear();
    if (!s_ctx)
        return;
    ImGuiContext *previous = ImGui::GetCurrentContext();
    ImGuiContext *ctx = s_ctx;
    s_ctx = NULL;
    ImGui::SetCurrentContext(ctx);
    ImGui_ImplM2GL_ShutdownContext();
    ImGui::DestroyContext(ctx);
    ImGui::SetCurrentContext(previous == ctx ? NULL : previous);
}
