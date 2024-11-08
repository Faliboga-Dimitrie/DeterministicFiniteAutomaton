#pragma once
#include "AFDTranzition.h"
#include "Automaton.h"

class DeterministicFiniteAutomaton : public Automaton
{
	
	std::vector<AFDTranzition> m_tranzitions;
	
public:
	DeterministicFiniteAutomaton() = default;
	DeterministicFiniteAutomaton(std::vector<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFDTranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates);
	bool VerifyAutomaton();
	void PrintAutomaton() ;
	bool CheckWord(const std::string& word);

	~DeterministicFiniteAutomaton() override = default;
	
};


