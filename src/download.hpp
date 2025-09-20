#include "SafeQueue.hpp"
#include <atomic>
#include <cstdlib>
#include <dpp/channel.h>
#include <dpp/cluster.h>

#pragma once

namespace marigold::download
{

struct Result
{
	int m_exit_code;
	std::string m_output;

	Result(int code, std::string output);
};

struct Request
{
	std::string m_video_id;
	dpp::snowflake m_channel_snowflake;

	Request(Request&& other);
	Request(const Request& other) = default;
	Request(std::string video_id, dpp::snowflake channel_snowflake);

	auto execute() const -> Result;
};

void download_thread_main(marigold::util::SafeQueue<Request>& requests,
						  dpp::cluster& cluster, std::atomic_bool& should_run);
} // namespace marigold::download
