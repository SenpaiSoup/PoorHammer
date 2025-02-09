#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>

#include "ICommand.h"
#include "ITimer.h"
#include "IMessageListner.h"
#include "IButtonInteraction.h"

#include "BotActivityEnum.h"
#include "BotStatusEnum.h"

class DiscordAPI
{
protected:
	DiscordAPI(std::string _token);
	static DiscordAPI* m_Instance;;

private:
	class DiscordAPIImpl;
	DiscordAPIImpl* m_DiscordAPIImpl;

public:
	//Generic functions
	~DiscordAPI();
	static DiscordAPI* Instance();

	//Commands
	bool RegisterCommand(std::shared_ptr<ICommand> _Command);
	bool RegisterMessageListner(std::shared_ptr<IMessageListner> _Listner);
	void ClearCommands();

	//Timers
	bool Addtimer(std::shared_ptr<ITimer> _Timer);
	void StopTimer(int _ID);

	//Misc
	bool RegisterButton(std::shared_ptr<IButtonInteraction> _Button);
	bool GetTokenSet();
	void AddGuildmemberRole(dpp::guild_member _member, dpp::snowflake _roleID);
	void RemoveGuilmemberRole(dpp::guild_member _member, dpp::snowflake _roleID);
	void SetStatus(BotStatus _Status, BotActivity _Activity, std::string _Message);

	//Bot data
	std::string GetGlobalName();
	std::string GetLocalName();
	std::string GetAvatarURL();

	//Core
	bool Initialize();
	void Start();
	void StartLogger();
};