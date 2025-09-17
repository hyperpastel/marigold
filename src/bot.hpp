#pragma once

#include "SafeQueue.hpp"
#include "download.hpp"
#include <dpp/cluster.h>

namespace marigold::bot
{

struct MarigoldBot
{
	dpp::cluster m_cluster;
	marigold::util::SafeQueue<download::Request>& m_requests;

	MarigoldBot(std::string token,
				marigold::util::SafeQueue<download::Request>& requests);

	void register_commands();
	void run();
};

} // namespace marigold::bot
