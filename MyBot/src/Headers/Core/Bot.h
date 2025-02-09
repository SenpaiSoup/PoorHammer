#pragma once

#include <memory>

#include "DiscordAPI.h"
#include "PingCommand.h"

class Bot
{
protected:
	Bot();
	static Bot* m_Instance;

public:
	Bot(Bot& other) = delete;
	void operator=(const Bot&) = delete;

public:

	static Bot* Instance();
	bool Initialize();
	bool Run();
	bool Shutdown();

;
};