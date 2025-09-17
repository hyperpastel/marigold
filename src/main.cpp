#include "SafeQueue.hpp"
#include "bot.hpp"
#include "download.hpp"
#include <atomic>
#include <dpp/dpp.h>
#include <iostream>

auto main(int argc, char* argv[]) -> int
{
	if (argc < 2)
	{
		std::cerr << "Missing token as parameter" << std::endl;
		return 1;
	}

	auto token = std::string{argv[1]};

	auto queue = marigold::util::SafeQueue<marigold::download::Request>{};
	auto bot = marigold::bot::MarigoldBot(token, queue);

	auto should_run = std::atomic_bool{true};
	auto t1 =
		std::thread{marigold::download::download_thread_main, std::ref(queue),
					std::ref(bot.m_cluster), std::ref(should_run)};

	bot.register_commands();
	bot.run();

	t1.join();

	return 0;
}
