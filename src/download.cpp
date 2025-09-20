#include "download.hpp"
#include "embeds.hpp"
#include <array>
#include <cstdio>
#include <format>
#include <memory>
#include <sstream>

using namespace marigold::download;

Result::Result(int exit_code, std::string output)
	: m_exit_code(exit_code), m_output(std::move(output))
{
}

Request::Request(Request&& other)
	: m_video_id(std::move(other.m_video_id)),
	  m_channel_snowflake(std::move(other.m_channel_snowflake))
{
}

Request::Request(std::string video_id, dpp::snowflake channel_snowflake)
	: m_video_id(std::move(video_id)),
	  m_channel_snowflake(std::move(channel_snowflake))
{
}

auto Request::execute() const -> Result
{

	auto output = std::stringstream{};
	auto exit_code = int{};

	const auto deleter = [&exit_code](FILE* f) -> void
	{ exit_code = pclose(f); };

	const auto command = std::format(
		" yt-dlp -x --audio-format mp3 -o \"~/down/%(id)s.%(ext)s\" "
		"\"https://www.youtube.com/watch?v={}\"",
		m_video_id);

	auto pipe = std::unique_ptr<FILE, decltype(deleter)>{
		popen(command.c_str(), "r"), deleter};

	if (!pipe)
	{
		exit_code = -1;
		output << "Failed to execute download commanad with popen";
	}
	else
	{
		auto buffer = std::array<char, 128>{};
		auto do_read = true;
		auto read = size_t{};

		while (do_read)
		{
			read = std::fread(buffer.data(), sizeof(char), buffer.size(),
							  pipe.get());
			buffer[read] = '\0';
			if (read == 0)
			{
				if (std::feof(pipe.get()) || std::ferror(pipe.get()))
				{
					do_read = false;
				}
			}
			else
			{
				output << buffer.data();
			}
		}
	}

	std::cout.flush();

	return {exit_code, output.str()};
}

void marigold::download::download_thread_main(
	marigold::util::SafeQueue<Request>& requests, dpp::cluster& cluster,
	std::atomic_bool& should_run)
{
	while (should_run)
	{
		const auto request = requests.pop();
		const auto result = request.execute();

		const auto response =
			result.m_exit_code == 0
				? marigold::embeds::download_success(
					  request.m_video_id,
					  std::format("https://hyperpas.tel/marigold/{}.mp3",
								  request.m_video_id))
				: marigold::embeds::download_fail(request.m_video_id,
												  result.m_output);

		const auto message =
			dpp::message{request.m_channel_snowflake, response};
		cluster.message_create(message);
	}
}
