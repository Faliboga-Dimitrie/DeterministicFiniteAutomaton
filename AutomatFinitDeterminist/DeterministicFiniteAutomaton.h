#pragma once
#include <vector>
#include <unordered_set>
#include <string>
#include <functional>
#include <stack>
#include <iostream>
#include "Tranzition.h"

class DeterministicFiniteAutomaton
{
	std::vector<std::string> m_states;
	std::unordered_set<std::string> m_alphabet;
	std::vector<Tranzition> m_tranzitions;
	std::string m_initialState;
	std::unordered_set<std::string> m_finalStates;
	std::string m_regulateExpression;
	std::string m_postfixPoloishExpression;

	int priority(char op);
	std::string& addConcatenateSimbol(const std::string& expresion);
	void infixToPostfix(std::string& infix);
	void regulateExpressionToPostfix();
public:
	DeterministicFiniteAutomaton() = default;
	DeterministicFiniteAutomaton(std::vector<std::string> states, std::unordered_set<std::string> alphabet, std::vector<Tranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates);
	bool VerifyAutomaton();
	void PrintAutomaton();
	bool CheckWord(const std::string& word);
	void SetRegulateExpression(const std::string& word) { m_regulateExpression = word;}
};


