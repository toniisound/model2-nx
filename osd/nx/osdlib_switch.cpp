// license:GPL-3.0-or-later
/*
 * Model 2 NX - Sega Model 2 for Nintendo Switch
 *
 * osdlib_switch.cpp
 *
 * Nintendo Switch version of MAME's src/osd/modules/lib/osdlib_unix.cpp
 * (built instead of it). libnx/newlib has no mmap, mprotect, dlopen or
 * signals, so:
 * - virtual memory is plain page-aligned heap memory, always readable and
 *   writable. Asking for executable pages fails, which puts MAME's
 *   recompiler cache in its W^X mode; with NATIVE_DRC=drcbe_c nothing is
 *   ever executed from it.
 * - there are no dynamic modules.
 */

#include "osdcore.h"
#include "osdlib.h"

#include <SDL2/SDL.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string_view>

#include <unistd.h>


const char *osd_getenv(const char *name)
{
	return getenv(name);
}

int osd_setenv(const char *name, const char *value, int overwrite)
{
	return setenv(name, value, overwrite);
}

void osd_process_kill()
{
	std::abort();
}

void osd_break_into_debugger(const char *message)
{
	printf("Ignoring MAME exception: %s\n", message);
}

std::pair<std::error_condition, unsigned> osd_get_cache_line_size() noexcept
{
	return std::make_pair(std::error_condition(), 64U);   // Cortex-A57
}

std::string osd_get_clipboard_text() noexcept
{
	return std::string();
}

std::error_condition osd_set_clipboard_text(std::string_view text) noexcept
{
	return std::errc::not_supported;
}

int osd_getpid() noexcept
{
	return 1;
}


namespace osd {

namespace {

class dynamic_module_switch_impl : public dynamic_module
{
protected:
	virtual generic_fptr_t get_symbol_address(char const *symbol) override
	{
		return nullptr;
	}
};

constexpr std::size_t PAGE_SIZE_SWITCH = 0x1000;

} // anonymous namespace


bool invalidate_instruction_cache(void const *start, std::size_t size) noexcept
{
	char const *const begin(reinterpret_cast<char const *>(start));
	char const *const end(begin + size);
	__builtin___clear_cache(const_cast<char *>(begin), const_cast<char *>(end));
	return true;
}


void *virtual_memory_allocation::do_alloc(std::initializer_list<std::size_t> blocks, unsigned intent, std::size_t &size, std::size_t &page_size) noexcept
{
	std::size_t const p(PAGE_SIZE_SWITCH);
	std::size_t s(0);
	for (std::size_t b : blocks)
		s += (b + p - 1) / p;
	s *= p;
	if (!s)
		return nullptr;
	void *const result(std::aligned_alloc(p, s));
	if (!result)
		return nullptr;
	std::memset(result, 0, s);
	size = s;
	page_size = p;
	return result;
}

void virtual_memory_allocation::do_free(void *start, std::size_t size) noexcept
{
	std::free(start);
}

bool virtual_memory_allocation::do_set_access(void *start, std::size_t size, unsigned access) noexcept
{
	// Heap pages are always read/write and never executable.
	return !(access & EXECUTE);
}


dynamic_module::ptr dynamic_module::open(std::vector<std::string> &&names)
{
	return std::make_unique<dynamic_module_switch_impl>();
}

} // namespace osd
