#pragma once

#include <dpp/appcommand.h>

namespace marigold::commands
{

auto inline make_download_command(const dpp::snowflake& id) -> dpp::slashcommand
{
	auto command = dpp::slashcommand(
		"download", "Download a youtube video to the remote server", id);
	const auto url_option = dpp::command_option(
		dpp::co_string, "url", "URL of the video to download", true);

	const auto do_mp4_option = dpp::command_option(
		dpp::co_boolean, "do_mp4",
		"Whether the video should be saved as an mp4 or just an mp3", false);

	command.add_option(url_option);
	command.add_option(do_mp4_option);

	return command;
}
} // namespace marigold::commands
