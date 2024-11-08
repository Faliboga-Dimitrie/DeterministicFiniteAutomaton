#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::vector<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFDTranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates):
	Automaton(states, alphabet, initialState, finalStates),
	m_tranzitions{ tranzitions }
{
}

bool DeterministicFiniteAutomaton::VerifyAutomaton()
{
}



