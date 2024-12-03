#include "Tranzition.h"

Tranzition::Tranzition(const std::string& fromState, const std::string& symbol, const std::string& toState) :
	m_fromState{ fromState },
	m_symbol{ symbol },
	m_toState{ toState }
{
}