#pragma once

#include <string>
#include <dpp/dpp.h>

class IButtonInteraction
{
public:
	std::string m_ButtonID;

public:
	virtual void Execute(dpp::button_click_t _event) = 0;
};