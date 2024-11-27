#pragma once
#include "Automaton.h"
#include "AFNTransition.h"
#include <list>

class NonDeterministicFiniteAutomaton : public Automaton
{
	std::vector<AFNTransition> m_transitions;

public:
	
	NonDeterministicFiniteAutomaton() = default;
	NonDeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFNTransition> transitions, std::string initialState, std::unordered_set<std::string> finalStates);
	NonDeterministicFiniteAutomaton(const NonDeterministicFiniteAutomaton& automaton);

	void AddTransition(AFNTransition transition) { m_transitions.push_back(transition); }
	void AddToTransitions(std::vector<AFNTransition> transitions, std::string fromState, std::string symbol);
	void buildAutomaton(std::string regulateExpression);
	const std::vector<AFNTransition>& GetTransitions() const { return m_transitions; }

	~NonDeterministicFiniteAutomaton() override = default;
};

