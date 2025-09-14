#include "SafeQueue.hpp"
#include "bot.hpp"
#include "download.hpp"
#include <atomic>
#include <dpp/dpp.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

auto trim_whitespace(const std::string& s) -> std::string
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isspace(*it))
		it++;

	std::string::const_reverse_iterator rit = s.rbegin();
	while (rit.base() != it && isspace(*rit))
		rit++;

	return {it, rit.base()};
}

auto read_token() -> std::string
{
	auto input = std::ifstream{std::filesystem::current_path() / "token"};
	auto sstream = std::stringstream{};

	while (input >> sstream.rdbuf())
		;

	return trim_whitespace(sstream.str());
}

auto main(int argc, char* argv[]) -> int
{

	auto queue = marigold::util::SafeQueue<marigold::download::Request>{};
	const auto token = read_token();
	auto bot = marigold::bot::MarigoldBot(token, queue);

	if (argc > 1 && std::string_view{argv[1]} == "clear")
	{
		bot.clear_commands();
		std::cout << "Cleared commands..." << std::endl;
		return 0;
	}

	auto should_run = std::atomic_bool{true};
	auto t1 =
		std::thread{marigold::download::download_thread_main, std::ref(queue),
					std::ref(bot.m_cluster), std::ref(should_run)};

	bot.register_commands();
	bot.run();

	t1.join();

	return 0;
}
