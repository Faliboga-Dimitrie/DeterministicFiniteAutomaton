#pragma once
#include "Automaton.h"
#include "AFNTranzition.h"

class NonDeterministicFiniteAutomaton : public Automaton
{
	std::vector<AFNTranzition> m_tranzitions;

public:
	std::vector<AFNTranzition> GetTranzitions() { return m_tranzitions; }
	NonDeterministicFiniteAutomaton() = default;
	NonDeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFNTranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates);
	void AddTranzition(AFNTranzition tranzition) { m_tranzitions.push_back(tranzition); }

	~NonDeterministicFiniteAutomaton() override = default;
};

