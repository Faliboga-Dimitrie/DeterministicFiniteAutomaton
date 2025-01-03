#pragma once
#include "NonDeterministicFiniteAutomaton.h"
#include <unordered_map>
#include <queue>

class DeterministicFiniteAutomaton : public Automaton
{
	NonDeterministicFiniteAutomaton m_nfa;
	
public:
	DeterministicFiniteAutomaton() = default;
	DeterministicFiniteAutomaton(const std::unordered_set<std::string>& states,
		const std::unordered_set<std::string>& alphabet,
		const std::vector<Tranzition>& tranzitions,
		const std::string& initialState,
		const std::unordered_set<std::string>& finalStates);

	std::unordered_set<std::string> getLambdaClose(const std::string& state, const std::vector<Tranzition>& tranzitions);
	void convertAFNtoAFD(const std::string& regulateExpresionFileName);

	bool VerifyAutomaton();
	bool CheckWord(const std::string& word);
	void clearDeterministicFiniteAutomaton();

	~DeterministicFiniteAutomaton() override = default;
};


