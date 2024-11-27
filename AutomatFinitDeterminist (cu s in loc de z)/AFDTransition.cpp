#include "AFDTransition.h"

AFDTransition::AFDTransition(std::string fromState, std::string symbol, std::string toState) :
	Transition(fromState, symbol),
	m_toState{ toState }
{
}
