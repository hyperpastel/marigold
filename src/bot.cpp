#include "bot.hpp"
#include "SafeQueue.hpp"
#include "commands.hpp"
#include "download.hpp"
#include "embeds.hpp"
#include <dpp/cluster.h>
#include <dpp/message.h>
#include <regex>
#include <string>

std::optional<std::string> get_video_id_from_url(std::string url)
{
	const auto urlPattern = std::regex{
		R"((?:https?://)?(?:www\.)?(?:youtube\.com/watch\?v=|youtu\.be/)([a-zA-Z0-9_-]{11}))"};

	auto match = std::smatch{};
	if (std::regex_search(url, match, urlPattern) && match.size() > 1)
		return match[1].str();
	else
		return std::nullopt;
}

using namespace marigold::bot;

MarigoldBot::MarigoldBot(std::string token,
						 util::SafeQueue<download::Request>& requests)
	: m_cluster(dpp::cluster{std::move(token)}), m_requests(requests)
{

	m_cluster.on_log(dpp::utility::cout_logger());
}

void MarigoldBot::register_commands()
{

	m_cluster.on_slashcommand(
		[&](const dpp::slashcommand_t& event)
		{
			if (event.command.get_command_name() == "download")
			{

				const auto url =
					std::get<std::string>(event.get_parameter("url"));

				const auto video_id_optional = get_video_id_from_url(url);

				// TODO Use embeds
				if (!video_id_optional.has_value())
				{
					const auto embed = marigold::embeds::failed_to_parse(url);
					event.reply(embed);
					return;
				}

				const auto video_id = *video_id_optional;
				const auto request =
					download::Request{video_id, event.command.channel_id};

				const auto embed = marigold::embeds::queued_video(video_id);
				event.reply(embed);

				m_requests.push(request);
			}
		});

	m_cluster.on_ready(
		[&](const dpp::ready_t& event)
		{
			m_cluster.global_command_create(
				marigold::commands::make_download_command(m_cluster.me.id));
		});
}

void MarigoldBot::run() { m_cluster.start(dpp::st_wait); }
