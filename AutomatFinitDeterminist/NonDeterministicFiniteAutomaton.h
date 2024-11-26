#pragma once
#include "Automaton.h"
#include "AFNTranzition.h"
#include <list>

class NonDeterministicFiniteAutomaton : public Automaton
{
	std::vector<AFNTranzition> m_tranzitions;

public:
	
	NonDeterministicFiniteAutomaton() = default;
	NonDeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFNTranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates);
	NonDeterministicFiniteAutomaton(const NonDeterministicFiniteAutomaton& automaton);

	void AddTranzition(AFNTranzition tranzition) { m_tranzitions.push_back(tranzition); }
	void buildAutomaton(std::string regulateExpression);
	const std::vector<AFNTranzition>& GetTranzitions() const { return m_tranzitions; }

	~NonDeterministicFiniteAutomaton() override = default;
};

