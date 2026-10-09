// license:GPL-3.0-or-later
/*
 * Model 2 NX - Sega Model 2 for Nintendo Switch
 *
 * nxlog.cpp
 *
 * The Switch log file (see switch/nxlog.h).
 */

#include "nxlog.h"

#include "osdcore.h"
#include "strformat.h"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <mutex>
#include <sstream>
#include <string>

// Same folder as MODEL2NX_ROOT in switch/SwitchPlatform.h
#define MODEL2NX_LOG_PATH "sdmc:/switch/model2nx/model2nx.log"

namespace {

std::mutex s_mutex;

void Append(const char *text, size_t size)
{
	std::lock_guard<std::mutex> lock(s_mutex);
	if (FILE *f = std::fopen(MODEL2NX_LOG_PATH, "ab"))
	{
		std::fwrite(text, 1, size, f);
		std::fclose(f);
	}
}

const char *ChannelPrefix(osd_output_channel channel)
{
	switch (channel)
	{
	case OSD_OUTPUT_CHANNEL_ERROR:   return "[error] ";
	case OSD_OUTPUT_CHANNEL_WARNING: return "[warning] ";
	default:                         return "";
	}
}

class nx_log_output : public osd_output
{
public:
	virtual void output_callback(osd_output_channel channel, util::format_argument_pack<char> const &args) override
	{
		if (channel != OSD_OUTPUT_CHANNEL_DEBUG)
		{
			std::ostringstream text;
			text << ChannelPrefix(channel);
			util::stream_format(text, args);
			const std::string s = text.str();
			Append(s.data(), s.size());
		}
		chain_output(channel, args);
	}
};

nx_log_output s_output;
bool s_started = false;

} // anonymous namespace


void NxLogReset()
{
	std::lock_guard<std::mutex> lock(s_mutex);
	if (FILE *f = std::fopen(MODEL2NX_LOG_PATH, "wb"))
		std::fclose(f);
}

void NxLogPrintf(const char *format, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, format);
	int n = std::vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	if (n > 0)
		Append(buffer, std::min<size_t>(size_t(n), sizeof(buffer) - 1));
}

void NxLogStartMame()
{
	if (!s_started)
	{
		osd_output::push(&s_output);
		s_started = true;
	}
}

void NxLogStopMame()
{
	if (s_started)
	{
		osd_output::pop(&s_output);
		s_started = false;
	}
}
