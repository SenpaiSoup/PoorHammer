#pragma once

#include <String>
#include <dpp/dpp.h>

class ICommand
{
public:
	std::string m_Tag;
	std::string m_Description;

	std::vector<uint64_t> m_AllowedRoles;

public:
	virtual dpp::slashcommand BuildCommand(dpp::cluster& _Cluster) = 0;
	virtual void Execute(dpp::slashcommand_t) = 0;
	void CheckPermission(dpp::slashcommand_t _event)
	{
		_event.thinking(false);
		if (m_AllowedRoles.empty())
		{
			this->Execute(_event);
			return;
		}

		for (auto& allowedRole : m_AllowedRoles)
		{
			dpp::snowflake roleID = allowedRole;
			dpp::role* role = dpp::find_role(roleID);
			auto user = dpp::find_guild_member(_event.command.guild_id, _event.command.get_issuing_user().id);

			for (auto& guildrole : user.get_roles())
			{
				if (guildrole == roleID)
				{
					this->Execute(_event);
					return;
				}
			}
		}

		std::string allowedRolesMessage = "This command requires one of the following roles:\n\n";
		for (auto& allowed : m_AllowedRoles)
		{
			dpp::snowflake roleID = allowed;
			dpp::role* role = dpp::find_role(roleID);
			if (role != nullptr)
			{
				allowedRolesMessage.append("- " + role->name + "\n");
			}
		}


		dpp::embed noAccessEmbed = dpp::embed()
			.set_color(dpp::colors::deep_red)
			.set_thumbnail("https://cdn.discordapp.com/icons/962855308932317204/35d21ce95860e26a2f86a018299fe957.webp?size=512")
			.set_title("Insufficient permissions")
			.set_description(allowedRolesMessage)
			.set_footer(dpp::embed_footer()
				.set_text("Contact Senpai_soup if you believe this to be wrong")
				.set_icon("https://cdn.discordapp.com/icons/962855308932317204/35d21ce95860e26a2f86a018299fe957.webp?size=512"));

		_event.edit_response(noAccessEmbed);
	}
};