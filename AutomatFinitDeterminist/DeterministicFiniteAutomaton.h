#pragma once
#include "AFDTranzition.h"
#include "NonDeterministicFiniteAutomaton.h"
#include <unordered_map>
#include <queue>
class DeterministicFiniteAutomaton : public Automaton
{
	
	std::vector<AFDTranzition> m_tranzitions;
	NonDeterministicFiniteAutomaton m_nfa;

	//void AFNtoAFD();
	
public:
	DeterministicFiniteAutomaton() = default;
	DeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFDTranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates);
	std::unordered_set<std::string> getLambdaClose(std::string state, std::vector<AFNTranzition> tranzitions);
	bool hasNoEmptySets(const std::unordered_map<std::string, std::unordered_set<std::string>>& map);
	void convertAFNtoAFD(NonDeterministicFiniteAutomaton nfa);
	bool VerifyAutomaton();
	void PrintAutomaton();
	bool CheckWord(const std::string& word);
	std::vector<AFDTranzition> GetTranzitions() { return m_tranzitions; }
	void AddTranzition(AFDTranzition tranzition) { m_tranzitions.push_back(tranzition); }

	~DeterministicFiniteAutomaton() override = default;
};


