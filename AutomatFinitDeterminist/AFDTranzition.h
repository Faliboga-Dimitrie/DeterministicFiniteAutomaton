#pragma once
#include "Tranzition.h"

class AFDTranzition : public Tranzition
{
	std::string m_toState;
public:
	
	AFDTranzition(std::string fromState, std::string symbol, std::string toState);
};
