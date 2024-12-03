#pragma once
#include "NonDeterministicFiniteAutomaton.h"
#include <unordered_map>
#include <queue>

class DeterministicFiniteAutomaton : public Automaton
{
	
	NonDeterministicFiniteAutomaton m_nfa;

	//void AFNtoAFD();
	
public:
	DeterministicFiniteAutomaton() = default;
	DeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<Tranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates);
	std::unordered_set<std::string> getLambdaClose(std::string state, std::vector<Tranzition> tranzitions);
	bool hasNoEmptySets(const std::unordered_map<std::string, std::unordered_set<std::string>>& map);
	void convertAFNtoAFD(NonDeterministicFiniteAutomaton nfa);
	bool VerifyAutomaton();
	void PrintAutomaton();
	bool CheckWord(const std::string& word);

	~DeterministicFiniteAutomaton() override = default;
};


