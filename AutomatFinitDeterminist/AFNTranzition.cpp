#include "AFNTranzition.h"

AFNTranzition::AFNTranzition(std::string fromState, std::string symbol, std::vector<std::string> toStates):
	Tranzition(fromState, symbol),
	m_toStates{ toStates }
{
}
