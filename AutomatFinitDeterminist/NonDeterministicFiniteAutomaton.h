#pragma once
#include "Automaton.h"
#include "AFNTranzition.h"

class NonDeterministicFiniteAutomaton : public Automaton
{
	std::vector<AFNTranzition> m_tranzitions;

public:

	NonDeterministicFiniteAutomaton() = default;
	NonDeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFNTranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates);

	~NonDeterministicFiniteAutomaton() override = default;
};

