#pragma once
#include "AFDTransition.h"
#include "NonDeterministicFiniteAutomaton.h"

class DeterministicFiniteAutomaton : public Automaton
{
	
	std::vector<AFDTransition> m_transitions;
	NonDeterministicFiniteAutomaton m_nfa;
	
public:
	DeterministicFiniteAutomaton() = default;
	DeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFDTransition> transitions, std::string initialState, std::unordered_set<std::string> finalStates);
	bool VerifyAutomaton();
	void PrintAutomaton();
	bool CheckWord(const std::string& word);
	std::vector<AFDTransition> GetTransitions() { return m_transitions; }
	void AddTransition(AFDTransition transition) { m_transitions.push_back(transition); }

	~DeterministicFiniteAutomaton() override = default;
};


