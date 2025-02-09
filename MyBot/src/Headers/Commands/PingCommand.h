#pragma once

#include <Chrono>
#include "ICommand.h"

class PingCommand : public ICommand
{
public:
	PingCommand();
	dpp::slashcommand BuildCommand(dpp::cluster& _Cluster) override;
	void Execute(dpp::slashcommand_t _event) override;
};
