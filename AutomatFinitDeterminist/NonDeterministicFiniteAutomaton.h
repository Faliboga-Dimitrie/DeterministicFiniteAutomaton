#pragma once
#include "Automaton.h"
#include <list>

class NonDeterministicFiniteAutomaton : public Automaton
{
public:
	
	NonDeterministicFiniteAutomaton() = default;
	NonDeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<Tranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates);
	NonDeterministicFiniteAutomaton(const NonDeterministicFiniteAutomaton& automaton);

	void buildAutomaton(std::string regulateExpression);

	/*std::unordered_set<std::string> EClosure(const std::unordered_set<std::string>& states);
	std::unordered_set<std::string> Move(const std::unordered_set<std::string>& states, const std::string& symbol);*/

	~NonDeterministicFiniteAutomaton() override = default;
};

