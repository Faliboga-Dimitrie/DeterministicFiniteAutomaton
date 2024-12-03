#pragma once
#include "Automaton.h"
#include <list>

class NonDeterministicFiniteAutomaton : public Automaton
{
public:
	
	NonDeterministicFiniteAutomaton() = default;
	NonDeterministicFiniteAutomaton(const std::unordered_set<std::string>& states,
		const std::unordered_set<std::string>& alphabet,
		const std::vector<Tranzition>& tranzitions,
		const std::string& initialState,
		const std::unordered_set<std::string>& finalStates);
	NonDeterministicFiniteAutomaton(const NonDeterministicFiniteAutomaton& automaton);

	void buildAutomaton(const std::string& regulateExpression);

	/*std::unordered_set<std::string> EClosure(const std::unordered_set<std::string>& states);
	std::unordered_set<std::string> Move(const std::unordered_set<std::string>& states, const std::string& symbol);*/

	~NonDeterministicFiniteAutomaton() override = default;
};

