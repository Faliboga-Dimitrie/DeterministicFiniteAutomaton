#include "Transition.h"

Transition::Transition(std::string fromState, std::string symbol) :
	m_fromState{ fromState },
	m_symbol{ symbol }
{
}