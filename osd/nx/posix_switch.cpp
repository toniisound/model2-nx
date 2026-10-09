// license:GPL-3.0-or-later
/*
 * Model 2 NX - Sega Model 2 for Nintendo Switch
 *
 * posix_switch.cpp
 *
 * Nintendo Switch version of MAME's posixptty.cpp and posixsocket.cpp (built
 * instead of them): no pseudo-terminals or socket "files" (serial/network
 * passthrough to the host), which Model 2 games never use.
 */

#include "posixfile.h"

#include <cstdint>
#include <string>
#include <system_error>


bool posix_check_socket_path(std::string const &path) noexcept
{
	return false;
}

std::error_condition posix_open_socket(std::string const &path, std::uint32_t openflags, osd_file::ptr &file, std::uint64_t &filesize) noexcept
{
	return std::errc::not_supported;
}

bool posix_check_domain_path(std::string const &path) noexcept
{
	return false;
}

std::error_condition posix_open_domain(std::string const &path, std::uint32_t openflags, osd_file::ptr &file, std::uint64_t &filesize) noexcept
{
	return std::errc::not_supported;
}

bool posix_check_ptty_path(std::string const &path) noexcept
{
	return false;
}

std::error_condition posix_open_ptty(std::uint32_t openflags, osd_file::ptr &file, std::uint64_t &filesize, std::string &name) noexcept
{
	return std::errc::not_supported;
}
