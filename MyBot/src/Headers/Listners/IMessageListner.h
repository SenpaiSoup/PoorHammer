#pragma once

#include <String>
#include <dpp/dpp.h>

class IMessageListner
{
public:
	bool m_AuthorRequired;
	bool m_ChannelRequired;
	__int64 m_AuthorID;
	__int64 m_ChannelID;
	std::string m_Keyword;

public:
	virtual void Execute(dpp::message_create_t event) = 0;
};
