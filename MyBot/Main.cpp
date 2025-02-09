#include "MyBot.h"
#include "Bot.h"

int main()
{
	Bot::Instance()->Initialize();
	Bot::Instance()->Run();
	Bot::Instance()->Shutdown();

	return 0;
}
