// license:GPL-3.0-or-later
/*
 * Model 2 NX - Sega Model 2 for Nintendo Switch
 *
 * http_switch.cpp
 *
 * Switch version of MAME's src/emu/http.cpp (built instead of it): MAME's
 * optional web server (-http, off by default) without the server. The real
 * one is built on asio, which needs POSIX networking libnx does not fully
 * have, and a web server has no use on the console. The manager is never
 * active, so MAME never registers handlers that would be called.
 */

#include "emu.h"
#include "http.h"

// http.h only declares these (the real http.cpp gets them from webpp);
// http_manager's destructor needs complete types for its unique_ptrs.
namespace webpp
{
	class http_server { };
	class ws_server { };
	struct Connection { };
}

http_manager::http_manager(bool active, short port, const char *root)
	: m_active(false)
	, m_root(root ? root : "")
{
	if (active)
		osd_printf_warning("The HTTP server (-http) is not available in Model 2 NX\n");
}

http_manager::~http_manager()
{
}

void http_manager::clear()
{
	std::lock_guard<std::mutex> lock(m_handlers_mutex);
	m_handlers.clear();
}

void http_manager::add_template(const std::string &path, substitution substitute, char init, char term)
{
}

void http_manager::remove_template(const std::string &path)
{
}

void http_manager::serve_template(http_request_ptr request, http_response_ptr response, const std::string &path, substitution substitute, char init, char term)
{
}

void http_manager::serve_document(http_request_ptr request, http_response_ptr response, const std::string &path)
{
}

void http_manager::add_http_handler(const std::string &path, http_handler handler)
{
}

void http_manager::remove_http_handler(const std::string &path)
{
}

http_manager::websocket_endpoint_ptr http_manager::add_endpoint(const std::string &path, websocket_open_handler on_open, websocket_message_handler on_message, websocket_close_handler on_close, websocket_error_handler on_error)
{
	auto endpoint = std::make_shared<websocket_endpoint>();
	endpoint->on_open = on_open;
	endpoint->on_message = on_message;
	endpoint->on_close = on_close;
	endpoint->on_error = on_error;
	return endpoint;
}

void http_manager::remove_endpoint(const std::string &path)
{
}
