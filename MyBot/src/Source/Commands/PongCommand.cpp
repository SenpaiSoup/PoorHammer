#include "PongCommand.h"

PongCommand::PongCommand()
{
	m_Tag = "pong";
	m_Description = "Ping pong!";
}

dpp::slashcommand PongCommand::BuildCommand(dpp::cluster& _Cluster)
{
	return dpp::slashcommand(m_Tag, m_Description, _Cluster.me.id);
}

void PongCommand::Execute(dpp::slashcommand_t _event)
{
	_event.edit_response("Ping");
}