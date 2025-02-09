#pragma once

#include <functional>

class ITimer
{
public:
	int m_ID;
	uint64_t m_Timing;
	std::function<void()> m_Function;

public:
	virtual void CreateFunction() = 0;
};
