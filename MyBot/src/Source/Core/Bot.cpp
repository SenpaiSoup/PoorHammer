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
	return false;
}

bool Bot::Run()
{
	return false;
}

bool Bot::Shutdown()
{
	return false;
}
