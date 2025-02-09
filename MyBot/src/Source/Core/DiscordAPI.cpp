#include "DiscordAPI.h"
#include <dpp/dpp.h>

DiscordAPI* DiscordAPI::m_Instance = nullptr;

///***************************************************************
///					DiscordAPIImpl.h
///***************************************************************

class DiscordAPI::DiscordAPIImpl
{
	friend DiscordAPI;

private:
	DiscordAPIImpl(std::string _Token);
	~DiscordAPIImpl();

	bool RegisterCommand(std::shared_ptr<ICommand> _Command);
	bool RegisterMessageListner(std::shared_ptr<IMessageListner> _Listner);
	bool RegisterButton(std::shared_ptr<IButtonInteraction> _Button);
	bool GetTokenSet();
	void AddGuildmemberRole(dpp::guild_member _member, dpp::snowflake _roleID);
	void RemoveGuilmemberRole(dpp::guild_member _member, dpp::snowflake _roleID);
	void SetStatus(BotStatus _Status, BotActivity _Activity, std::string _Message);

	//Timers
	bool Addtimer(std::shared_ptr<ITimer> _Timer);
	void StopTimer(int _ID);

	std::string GetGlobalName();
	std::string GetLocalName();
	std::string GetAvatarURL();

	void Clearcommands();
	bool Initialize();
	void Start();
	void StartLogger();


	//internal functions
	void LoadCommands();
	void LoadEvents();

	//standard variables
private:

	std::string m_Token;
	bool m_TokenSet;
	std::vector<std::shared_ptr<ICommand>> m_Commands;
	std::vector<dpp::slashcommand> m_DPPCommands;
	std::vector<std::shared_ptr<IMessageListner>> m_Listners;
	std::vector<std::shared_ptr<IButtonInteraction>> m_Buttons;
	std::vector<std::shared_ptr<ITimer>> m_Timers;

	//D++ variables
private:
	dpp::cluster m_BotCluster;

};

///***************************************************************
///					DiscordAPIImpl.cpp
///***************************************************************

DiscordAPI::DiscordAPIImpl::DiscordAPIImpl(std::string _Token) :
	m_Token(_Token), m_TokenSet(false), m_BotCluster(m_Token, dpp::i_default_intents | dpp::i_message_content)
{
	;
}

DiscordAPI::DiscordAPIImpl::~DiscordAPIImpl()
{
	m_BotCluster.global_bulk_command_delete();
}

bool DiscordAPI::DiscordAPIImpl::RegisterCommand(std::shared_ptr<ICommand> _Command)
{
	m_Commands.push_back(_Command);
	return false;
}

bool DiscordAPI::DiscordAPIImpl::RegisterMessageListner(std::shared_ptr<IMessageListner> _Listner)
{
	m_Listners.push_back(_Listner);
	return false;
}

bool DiscordAPI::DiscordAPIImpl::RegisterButton(std::shared_ptr<IButtonInteraction> _Button)
{
	m_Buttons.push_back(_Button);
	return false;
}

bool DiscordAPI::DiscordAPIImpl::GetTokenSet()
{
	return m_TokenSet;
}

void DiscordAPI::DiscordAPIImpl::AddGuildmemberRole(dpp::guild_member _member, dpp::snowflake _roleID)
{
	m_BotCluster.guild_edit_member(_member.add_role(_roleID));
}

void DiscordAPI::DiscordAPIImpl::RemoveGuilmemberRole(dpp::guild_member _member, dpp::snowflake _roleID)
{
	m_BotCluster.guild_edit_member(_member.remove_role(_roleID));
}

void DiscordAPI::DiscordAPIImpl::SetStatus(BotStatus _Status, BotActivity _Activity, std::string _Message)
{
	dpp::presence_status status;
	dpp::activity_type activity;
	switch (_Status)
	{
	case Online:
		status = dpp::presence_status::ps_online;
		break;
	case Away:
		status = dpp::presence_status::ps_idle;
		break;
	case DND:
		status = dpp::presence_status::ps_dnd;
		break;
	case Invisible:
		status = dpp::presence_status::ps_invisible;
		break;
	default:
		break;
	}

	switch (_Activity)
	{
	case Competing:
		activity = dpp::activity_type::at_competing;
		break;
	case Custom:
		activity = dpp::activity_type::at_custom;
		break;
	case Game:
		activity = dpp::activity_type::at_game;
		break;
	case Listening:
		activity = dpp::activity_type::at_listening;
		break;
	case Streaming:
		activity = dpp::activity_type::at_streaming;
		break;
	case Watching:
		activity = dpp::activity_type::at_watching;
		break;
	default:
		break;
	}

	m_BotCluster.set_presence(dpp::presence(status, activity, _Message));
}

bool DiscordAPI::DiscordAPIImpl::Addtimer(std::shared_ptr<ITimer> _Timer)
{
	for (auto& timer : m_Timers)
	{
		if (timer->m_ID == _Timer->m_ID)
		{
			m_Timers.push_back(_Timer);
			return false;
		}
	}

	m_BotCluster.start_timer([_Timer](const dpp::timer& timer) { _Timer->m_Function(); }, _Timer->m_Timing);
	return true;
}

void DiscordAPI::DiscordAPIImpl::StopTimer(int _ID)
{
	for (auto& timer : m_Timers)
	{
		if (timer->m_ID == _ID)
		{
			m_BotCluster.stop_timer(timer->m_ID);
			return;
		}
	}
}

std::string DiscordAPI::DiscordAPIImpl::GetGlobalName()
{
	return m_BotCluster.me.global_name;
}

std::string DiscordAPI::DiscordAPIImpl::GetLocalName()
{
	return m_BotCluster.me.username;
}

std::string DiscordAPI::DiscordAPIImpl::GetAvatarURL()
{
	return m_BotCluster.me.get_avatar_url();
}

void DiscordAPI::DiscordAPIImpl::Clearcommands()
{
	m_BotCluster.global_bulk_command_delete();
}

bool DiscordAPI::DiscordAPIImpl::Initialize()
{
	this->LoadCommands();
	this->LoadEvents();
	return true;
}

void DiscordAPI::DiscordAPIImpl::Start()
{

	m_BotCluster.start(dpp::st_wait);
}

void DiscordAPI::DiscordAPIImpl::StartLogger()
{
	m_BotCluster.on_log(dpp::utility::cout_logger());
}

void DiscordAPI::DiscordAPIImpl::LoadCommands()
{
	for (std::shared_ptr<ICommand> command : m_Commands)
	{
		m_DPPCommands.push_back(command->BuildCommand(m_BotCluster));
	}

	m_BotCluster.on_ready([&](const dpp::ready_t& event)
		{
			m_DPPCommands;
			m_BotCluster.global_bulk_command_create(m_DPPCommands);
		});
}

void DiscordAPI::DiscordAPIImpl::LoadEvents()
{
	m_BotCluster.on_slashcommand([&](const dpp::slashcommand_t& _event)
		{
			for (std::shared_ptr<ICommand> command : m_Commands)
			{
				if (_event.command.get_command_name() == command->m_Tag)
				{
					command->CheckPermission(_event);
				}
			}
		});

	m_BotCluster.on_message_create([&](const dpp::message_create_t& _event) {

		for (std::shared_ptr<IMessageListner> listner : m_Listners)
		{
			if (_event.msg.channel_id == listner->m_ChannelID && _event.msg.author.id == listner->m_AuthorID)
			{
				listner->Execute(_event);
			}
		}
		});


	m_BotCluster.on_button_click([&](const dpp::button_click_t& _event) {
		for (std::shared_ptr<IButtonInteraction> button : m_Buttons)
		{
			if (_event.custom_id == button->m_ButtonID)
			{
				button->Execute(_event);
			}
		}
		});
}

///***************************************************************
///					DiscordAPI.cpp
///***************************************************************

bool DiscordAPI::RegisterCommand(std::shared_ptr<ICommand> _Command)
{
	return m_DiscordAPIImpl->RegisterCommand(std::move(_Command));
}

bool DiscordAPI::RegisterMessageListner(std::shared_ptr<IMessageListner> _Listner)
{
	return m_DiscordAPIImpl->RegisterMessageListner(std::move(_Listner));
}

bool DiscordAPI::RegisterButton(std::shared_ptr<IButtonInteraction> _Button)
{
	return m_DiscordAPIImpl->RegisterButton(std::move(_Button));
}

DiscordAPI::DiscordAPI(std::string _token)
{
	m_DiscordAPIImpl = new DiscordAPIImpl(_token);
}

DiscordAPI::~DiscordAPI()
{
	m_DiscordAPIImpl->~DiscordAPIImpl();
	delete m_DiscordAPIImpl;
}

DiscordAPI* DiscordAPI::Instance()
{
	if (m_Instance == nullptr)
	{
		std::fstream tokenFile;
		tokenFile.open("Token.txt", std::ios::in);
		std::string token;

		if (tokenFile.is_open())
		{
			if (tokenFile.is_open())
			{
				std::getline(tokenFile, token);
			}
			tokenFile.close();
		}
		else {
			std::cout << "Token file could not be opened!" << std::endl;
		}

		m_Instance = new DiscordAPI(token);

	}
	return m_Instance;
}

bool DiscordAPI::GetTokenSet()
{
	return m_DiscordAPIImpl->GetTokenSet();
}

void DiscordAPI::AddGuildmemberRole(dpp::guild_member _member, dpp::snowflake _roleID)
{
	m_DiscordAPIImpl->AddGuildmemberRole(_member, _roleID);
}

void DiscordAPI::RemoveGuilmemberRole(dpp::guild_member _member, dpp::snowflake _roleID)
{
	m_DiscordAPIImpl->RemoveGuilmemberRole(_member, _roleID);
}

void DiscordAPI::SetStatus(BotStatus _Status, BotActivity _Activity, std::string _Message)
{
	m_DiscordAPIImpl->SetStatus(_Status, _Activity, _Message);
}

void DiscordAPI::ClearCommands()
{
	m_DiscordAPIImpl->Clearcommands();
}

bool DiscordAPI::Addtimer(std::shared_ptr<ITimer> _Timer)
{
	return m_DiscordAPIImpl->Addtimer(std::move(_Timer));
}

void DiscordAPI::StopTimer(int _ID)
{
	m_DiscordAPIImpl->StopTimer(_ID);
}

std::string DiscordAPI::GetGlobalName()
{
	return m_DiscordAPIImpl->GetGlobalName();
}

std::string DiscordAPI::GetLocalName()
{
	return m_DiscordAPIImpl->GetLocalName();
}

std::string DiscordAPI::GetAvatarURL()
{
	return m_DiscordAPIImpl->GetAvatarURL();
}

bool DiscordAPI::Initialize()
{
	return m_DiscordAPIImpl->Initialize();
}

void DiscordAPI::Start()
{
	return m_DiscordAPIImpl->Start();
}

void DiscordAPI::StartLogger()
{
	m_DiscordAPIImpl->StartLogger();
}

