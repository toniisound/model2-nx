#
# Model 2 NX - Sega Model 2 for Nintendo Switch
#
# MAME (only what its Model 2 driver needs, see mame.mk) with MAME's SDL front
# end and the Switch platform layer from SuperModel NX.
#
# Nintendo Switch (.nro), from WSL or Linux with devkitPro:
#   sudo dkp-pacman -S switch-dev switch-sdl2 switch-mesa switch-zlib
#   make -j$(nproc)
#   make NXLINK=1 -j$(nproc)        # stdout/stderr to `nxlink -s` on the PC
#
# PC build (Linux/WSL, for testing and profiling on the PC; needs SDL2):
#   make PLATFORM=linux -j$(nproc)
#   build/linux/model2nx -validate
#
# Options:
#   DEBUG=1    -O1 -g, no -Werror
#   NXLINK=1   (Switch) console output to the PC
#   V=1        show the compiler command lines
#
# Output:
#   build/switch/model2nx.nro  -> copy to sdmc:/switch/model2nx/
#   build/linux/model2nx
#

PLATFORM ?= switch
.DEFAULT_GOAL := all
NXLINK   ?= 0
DEBUG    ?= 0

MAME  := mame
BUILD := build/$(PLATFORM)

APP_TITLE   := Model 2 NX
APP_AUTHOR  := ToniiSound
APP_VERSION := 0.1.0

include mame.mk

ifeq ($(V),1)
Q :=
else
Q := @
endif

#---------------------------------------------------------------------------------
# Platform
#---------------------------------------------------------------------------------
# Defines every OSD (front end) file is built with. SDLMAME_NO_X11 and
# USE_OPENGL=0: MAME draws through SDL's 2D renderer ("accel" video), which
# SDL implements with OpenGL ES on the Switch.
OSD_DEFINES := -DOSD_SDL -DSDLMAME_SDL2=1 -DSDLMAME_NO_X11 -DUSE_OPENGL=0 \
	-DNO_USE_MIDI -DUSE_QTDEBUG=0 -DNO_USE_PORTAUDIO -DNO_USE_PULSEAUDIO \
	-DNO_USE_PIPEWIRE

ifeq ($(PLATFORM),switch)

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif
include $(DEVKITPRO)/libnx/switch_rules

TARGET  := $(BUILD)/model2nx
ICON    := $(firstword $(wildcard assets/icon.jpg) $(LIBNX)/default_icon.jpg)
ROMFS   := $(BUILD)/romfs

ARCH    := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE
PKGCONF := $(firstword $(wildcard $(PORTLIBS)/bin/aarch64-none-elf-pkg-config) pkg-config)

# NATIVE_DRC=drcbe_c: MAME's recompilers (SHARC on Model 2B, MB86235 on 2C)
# run their intermediate code through the C back end for now. The ARM64 back
# end needs the libnx JIT (see docs/roadmap.md).
# ASMJIT_NO_JIT: asmjit's own executable-memory manager needs mmap; MAME's
# recompilers use asmjit only to assemble into MAME's own cache.
PLATFORM_DEFINES := -D__SWITCH__ -DSDLMAME_SWITCH \
	-DNATIVE_DRC=drcbe_c -DASMJIT_NO_JIT
ifeq ($(strip $(NXLINK)),1)
PLATFORM_DEFINES += -DSWITCH_NXLINK
endif

PLATFORM_INCLUDES := -Iswitch -Iosd/nx/include -I$(PORTLIBS)/include -I$(PORTLIBS)/include/SDL2 -I$(LIBNX)/include
PLATFORM_SOURCES  := switch/SwitchPlatform.cpp osd/nx/nxstubs.cpp \
	osd/nx/osdlib_switch.cpp osd/nx/posix_switch.cpp osd/nx/sqlite_switch.c \
	osd/nx/linenoise_stub.c osd/nx/http_switch.cpp osd/nx/nxlog.cpp

# MAME's web server (-http) is built on asio, which needs POSIX networking
# libnx lacks: osd/nx/http_switch.cpp is the same manager without a server.
MAME_EMU_SOURCES := $(filter-out src/emu/http.cpp,$(MAME_EMU_SOURCES))

# Replaced by the osd/nx versions above (no mmap, dlopen, ptys or socket files)
MAME_OSD_SOURCES := $(filter-out \
	src/osd/modules/lib/osdlib_unix.cpp \
	src/osd/modules/file/posixptty.cpp \
	src/osd/modules/file/posixsocket.cpp, \
	$(MAME_OSD_SOURCES))

LDFLAGS  = -specs=$(DEVKITPRO)/libnx/switch.specs $(ARCH) -Wl,-Map,$(TARGET).map
SDL_LIBS := $(shell $(PKGCONF) --libs sdl2 2>/dev/null)
ifeq ($(strip $(SDL_LIBS)),)
SDL_LIBS := -lSDL2 -lEGL -lGLESv2 -lglapi -ldrm_nouveau
endif
LIBS     := $(SDL_LIBS) -lnx -lm
LIBPATHS := -L$(PORTLIBS)/lib -L$(LIBNX)/lib

# Lua's POSIX extras (popen, mkstemp) are not in newlib.
MAME_LUA_DEFINES := $(filter-out -DLUA_USE_POSIX,$(MAME_LUA_DEFINES))
MAME_FLAC_DEFINES := $(filter-out -DFLAC__SYS_LINUX,$(MAME_FLAC_DEFINES))
# expat: no getentropy() in libnx (newlib declares it, nothing implements it);
# it still has /dev/urandom and its own fallback.
MAME_EXPAT_DEFINES := $(filter-out -DHAVE_GETENTROPY,$(MAME_EXPAT_DEFINES))
# SQLite is only reached from Lua (plugins, off here): no Unix VFS, which
# needs file locking and other calls newlib lacks (see osd/nx/sqlite_switch.c).
MAME_SQLITE3_DEFINES += -DSQLITE_OS_OTHER=1 -DSQLITE_THREADSAFE=0

else ifeq ($(PLATFORM),linux)

TARGET  := $(BUILD)/model2nx
CC      ?= gcc
CXX     ?= g++
ARCH    :=
SDL2_CONFIG ?= sdl2-config

PLATFORM_DEFINES  := -DSDLMAME_UNIX -DSDLMAME_LINUX -DPTR64=1
PLATFORM_INCLUDES := $(shell $(SDL2_CONFIG) --cflags)
PLATFORM_SOURCES  := osd/nx/nxstubs.cpp osd/nx/linenoise_stub.c

# No SDL_ttf/fontconfig needed on the PC either: MAME's built-in UI font.
MAME_OSD_SOURCES := $(filter-out src/osd/modules/font/font_sdl.cpp,$(MAME_OSD_SOURCES))
PLATFORM_DEFINES += -DMODEL2NX_NO_FONT_SDL

LDFLAGS  :=
LIBS     := $(shell $(SDL2_CONFIG) --libs) -lpthread -ldl -lutil -lm
LIBPATHS :=

else
$(error PLATFORM must be switch or linux)
endif

#---------------------------------------------------------------------------------
# Flags
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEBUG)),1)
OPT := -O1 -g
else
OPT := -O3 -fomit-frame-pointer
endif

# MAME's own warning set, without -Werror (newer/other compilers warn more).
WARN := -Wall -Wno-unknown-pragmas -Wno-sign-compare -Wno-conversion -Wno-unused \
	-Wno-stringop-overflow -Wno-stringop-overread -Wno-nonnull -Wno-array-bounds \
	-Wno-maybe-uninitialized -Wno-deprecated-declarations

BASE_FLAGS := $(ARCH) $(OPT) -pipe -fno-strict-aliasing -ffunction-sections -fdata-sections $(WARN)
CFLAGS_ALL   := $(BASE_FLAGS) -std=gnu99
# -Wno-sfinae-incomplete: GCC 16 warns about MAME's device_t in every file.
CXXFLAGS_ALL := $(BASE_FLAGS) -std=gnu++20 -Wno-sfinae-incomplete

#---------------------------------------------------------------------------------
# Objects
#---------------------------------------------------------------------------------
objs_of = $(patsubst %,$(BUILD)/obj/$(1)/%.o,$(basename $(2)))

ALL_OBJS :=

# One static library per MAME library, like MAME's own build: the linker then
# only pulls in the devices something references.
define MAME_LIB
$(1)_OBJS := $$(call objs_of,$(1),$$(MAME_$(call UC,$(1))_SOURCES))
$$($(1)_OBJS): LIB_FLAGS := $$(MAME_COMMON_DEFINES) $$(MAME_$(call UC,$(1))_DEFINES) \
	$$(OSD_DEFINES) $$(PLATFORM_DEFINES) $$(MAME_$(call UC,$(1))_INCLUDES) $$(PLATFORM_INCLUDES)
$(BUILD)/lib/lib$(1).a: $$($(1)_OBJS)
	@mkdir -p $$(dir $$@)
	@echo archiving $$(notdir $$@)
	$(Q)rm -f $$@
	$(Q)$$(AR) rcs $$@ $$^
ALL_OBJS += $$($(1)_OBJS)
$(BUILD)/obj/$(1)/%.o: $(MAME)/%.cpp
	$$(compile_cxx)
$(BUILD)/obj/$(1)/%.o: $(MAME)/%.cc
	$$(compile_cxx)
$(BUILD)/obj/$(1)/%.o: $(MAME)/%.c
	$$(if $$(filter $(1),$$(FORCE_CPP_LIBS)),$$(compile_c_as_cxx),$$(compile_c))
endef

# Libraries MAME builds as C++ even though their sources are .c files
# (GENie's ForceCPP): Lua, its libraries and SoftFloat 3.
FORCE_CPP_LIBS := softfloat3 lua lualibs

define compile_cxx
@mkdir -p $(dir $@)
@echo $(patsubst $(MAME)/%,%,$<)
$(Q)$(CXX) -MMD -MP $(CXXFLAGS_ALL) $(LIB_FLAGS) -c $< -o $@
endef

define compile_c_as_cxx
@mkdir -p $(dir $@)
@echo $(patsubst $(MAME)/%,%,$<)
$(Q)$(CXX) -MMD -MP -x c++ $(CXXFLAGS_ALL) $(LIB_FLAGS) -c $< -o $@
endef

define compile_c
@mkdir -p $(dir $@)
@echo $(patsubst $(MAME)/%,%,$<)
$(Q)$(CC) -MMD -MP $(CFLAGS_ALL) $(LIB_FLAGS) -c $< -o $@
endef

UC = $(shell echo '$(1)' | tr '[:lower:]' '[:upper:]')

$(foreach lib,$(MAME_LIBS),$(eval $(call MAME_LIB,$(lib))))

# Front end (MAME's SDL OSD) and our platform layer
# (MAME builds them with sdlprefix.h forced in, for its platform macros.)
OSD_OBJS := $(call objs_of,osd,$(MAME_OSD_SOURCES))
$(OSD_OBJS): LIB_FLAGS := $(MAME_COMMON_DEFINES) $(OSD_DEFINES) $(PLATFORM_DEFINES) \
	-I$(MAME)/src/osd/modules/lib $(MAME_OSD_INCLUDES) -Iswitch $(PLATFORM_INCLUDES) \
	-include $(MAME)/src/osd/sdl/sdlprefix.h

PLATFORM_OBJS := $(call objs_of,platform,$(PLATFORM_SOURCES))
$(PLATFORM_OBJS): LIB_FLAGS := $(MAME_COMMON_DEFINES) $(OSD_DEFINES) $(PLATFORM_DEFINES) \
	-I$(MAME)/src/osd/modules/lib $(MAME_OSD_INCLUDES) -I$(MAME)/3rdparty/linenoise \
	$(MAME_EMU_DEFINES) $(MAME_EMU_INCLUDES) \
	-Iswitch $(PLATFORM_INCLUDES) -include $(MAME)/src/osd/sdl/sdlprefix.h

ALL_OBJS += $(OSD_OBJS) $(PLATFORM_OBJS)

# Link order: driver first, then what it depends on (as MAME links them).
LINK_LIBS := $(foreach lib,$(MAME_LIBS),$(BUILD)/lib/lib$(lib).a)

$(BUILD)/obj/osd/%.o: $(MAME)/%.cpp
	$(compile_cxx)
$(BUILD)/obj/platform/%.o: %.cpp
	$(compile_cxx)
$(BUILD)/obj/platform/%.o: %.c
	$(compile_c)

#---------------------------------------------------------------------------------
# Rules
#---------------------------------------------------------------------------------
.PHONY: all clean romfs frontend

# Only the front end and platform layer (quick check of Switch-specific code)
frontend: $(OSD_OBJS) $(PLATFORM_OBJS)

ifeq ($(PLATFORM),switch)
all: $(TARGET).nro

$(TARGET).nro: $(TARGET).elf $(TARGET).nacp romfs
	@elf2nro $(TARGET).elf $@ --icon=$(ICON) --nacp=$(TARGET).nacp --romfsdir=$(ROMFS)
	@echo built ... $(notdir $@)

$(TARGET).nacp: Makefile
	@mkdir -p $(dir $@)
	@nacptool --create "$(APP_TITLE)" "$(APP_AUTHOR)" "$(APP_VERSION)" $@

# Files installed to the SD card on first launch (SwitchPlatform.cpp)
romfs:
	@mkdir -p $(ROMFS)
	@cp -u config/switch/mame.ini config/switch/ui.ini $(ROMFS)/
else
all: $(TARGET)
endif

# MAME's libraries depend on each other in a cycle (emu <-> optional <-> frontend),
# so the archives go in a group.
$(TARGET)$(if $(filter switch,$(PLATFORM)),.elf): $(OSD_OBJS) $(PLATFORM_OBJS) $(LINK_LIBS)
	@mkdir -p $(dir $@)
	@echo linking $(notdir $@)
	$(Q)$(CXX) $(LDFLAGS) $(OSD_OBJS) $(PLATFORM_OBJS) -Wl,--start-group $(LINK_LIBS) -Wl,--end-group \
		$(LIBPATHS) $(LIBS) -o $@

clean:
	@echo clean ...
	@rm -rf $(BUILD)

-include $(ALL_OBJS:.o=.d)
