#include "PingCommand.h"

PingCommand::PingCommand()
{
	m_Tag = "ping";
	m_Description = "Ping pong!";
}

dpp::slashcommand PingCommand::BuildCommand(dpp::cluster& _Cluster)
{
	return dpp::slashcommand(m_Tag, m_Description, _Cluster.me.id);
}

void PingCommand::Execute(dpp::slashcommand_t _event)
{
	_event.edit_response("Pong");
}
