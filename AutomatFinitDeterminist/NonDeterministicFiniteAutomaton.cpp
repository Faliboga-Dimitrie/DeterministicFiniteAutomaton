#include "NonDeterministicFiniteAutomaton.h"

NonDeterministicFiniteAutomaton::NonDeterministicFiniteAutomaton(std::vector<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFNTranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates):
	Automaton(states, alphabet, initialState, finalStates),
	m_tranzitions{ tranzitions }
{
}
