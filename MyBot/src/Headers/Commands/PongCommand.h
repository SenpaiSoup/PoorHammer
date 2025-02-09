#pragma once

#include "ICommand.h"

class PongCommand : public ICommand
{
public:
	PongCommand();
	dpp::slashcommand BuildCommand(dpp::cluster& _Cluster) override;
	void Execute(dpp::slashcommand_t _event) override;
};