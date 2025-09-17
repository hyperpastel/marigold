#include "SafeQueue.hpp"
#include "embeds.hpp"
#include <atomic>
#include <cstdlib>
#include <dpp/channel.h>
#include <dpp/cluster.h>
#include <format>
#include <string_view>

#pragma once

namespace marigold::download
{

struct SystemResult
{
	int code;
	std::string file_path;

	SystemResult(int code, std::string file_path)
		: code(code), file_path(std::move(file_path))
	{
	}
};

struct Request
{
	std::string m_video_id;
	dpp::snowflake m_snowflake;

	inline Request(Request&& other)
		: m_video_id(std::move(other.m_video_id)),
		  m_snowflake(std::move(other.m_snowflake))
	{
	}

	inline Request(const Request& other) = default;

	inline Request(std::string video_id, dpp::snowflake event)
		: m_video_id(std::move(video_id)), m_snowflake(std::move(event))
	{
	}
};

inline auto download_by_id(std::string_view video_id) -> SystemResult
{
	const auto file_name = std::format("~/down/{}.mp3", video_id);

	// TODO Launch this as an actual process and capture the output of it
	const auto command = std::format(
		" yt-dlp -x --audio-format mp3 -o \"~/down/%(id)s.%(ext)s\" "
		"\"https://www.youtube.com/watch?v={}\"",
		video_id);

	const auto code = system(command.c_str());
	const auto file_path = file_name;

	return {code, file_path};
}

inline void download_thread_main(marigold::util::SafeQueue<Request>& requests,
								 dpp::cluster& cluster,
								 std::atomic_bool& should_run)
{
	while (should_run)
	{
		const auto request = requests.pop();
		const auto result = download_by_id(request.m_video_id);

		const auto response =
			result.code == 0
				? marigold::embeds::download_success(request.m_video_id,
													 "https://youtube.com")
				: marigold::embeds::download_fail(request.m_video_id);

		const auto message = dpp::message{request.m_snowflake, response};
		cluster.message_create(message);
	}
}

} // namespace marigold::download
