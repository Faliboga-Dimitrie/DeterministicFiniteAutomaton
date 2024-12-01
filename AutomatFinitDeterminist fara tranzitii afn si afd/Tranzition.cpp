#include "Tranzition.h"

Tranzition::Tranzition(std::string fromState, std::string symbol, std::string toState) :
	m_fromState{ fromState },
	m_symbol{ symbol },
	m_toState{ toState }
{
}