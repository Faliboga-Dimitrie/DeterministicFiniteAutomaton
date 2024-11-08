#include "AFDTranzition.h"

AFDTranzition::AFDTranzition(std::string fromState, std::string symbol, std::string toState) :
	Tranzition(fromState, symbol),
	m_toState{ toState }
{
}
