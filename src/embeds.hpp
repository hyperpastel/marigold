#pragma once

#include <dpp/message.h>
#include <format>

namespace marigold::embeds
{

constexpr auto color = 0xf1e9c9;

inline dpp::embed failed_to_parse(std::string_view url)
{
	const auto description =
		std::format("Failed to obtain ``video_id`` from "
					"parameter ``url``, which was: ```{}```",
					url);

	return dpp::embed{}.set_color(color).set_title("Error").set_description(
		description);
}

inline dpp::embed queued_video(std::string_view video_id)
{
	return dpp::embed{}
		.set_color(color)
		.set_title("Video queued")
		.add_field("video_id", video_id);
}

inline dpp::embed download_success(std::string_view video_id,
								   std::string_view download_url)
{
	return dpp::embed{}
		.set_color(color)
		.set_title("Video downloaded successfully")
		.set_description("Click to download")
		.add_field("video_id", video_id)
		.set_url(download_url);
}

// TODO Add error log here once we have process handles
inline dpp::embed download_fail(std::string_view video_id)
{
	return dpp::embed{}
		.set_color(color)
		.set_title("Failed to download video")
		.set_description("An error occurred")
		.add_field("video_id", video_id);
}

} // namespace marigold::embeds
