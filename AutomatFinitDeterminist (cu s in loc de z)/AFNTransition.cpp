#include "AFNTransition.h"

AFNTransition::AFNTransition(std::string fromState, std::string symbol, std::vector<std::string> toStates):
	Transition(fromState, symbol),
	m_toStates{ toStates }
{
}
