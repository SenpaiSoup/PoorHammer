#include "Bot.h"

Bot* Bot::m_Instance = nullptr;

Bot::Bot()
{
}

Bot* Bot::Instance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new Bot();
	}
	return m_Instance;
}

bool Bot::Initialize()
{
	DiscordAPI::Instance()->ClearCommands();
	DiscordAPI::Instance()->Initialize();
	DiscordAPI::Instance()->StartLogger();

	return false;
}

bool Bot::Run()
{
	DiscordAPI::Instance()->Start();
	return false;
}

bool Bot::Shutdown()
{
	return false;
}
