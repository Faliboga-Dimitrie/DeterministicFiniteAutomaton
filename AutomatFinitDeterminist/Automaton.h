#pragma once
#include <vector>
#include <unordered_set>
#include <functional>
#include <stack>
#include <iostream>
#include <string>
#include <fstream>

class Automaton
{
	std::unordered_set<std::string> m_states;
	std::unordered_set<std::string> m_alphabet;
	std::string m_initialState;
	std::unordered_set<std::string> m_finalStates;
	std::string m_regulateExpression;
	std::string m_postfixPoloishExpression;

	int priority(char op);
	void addConcatenateSimbol();
	void infixToPostfix();

public:

	Automaton() = default;
	Automaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::string initialState, std::unordered_set<std::string> finalStates);

	void ReadRegulateExpression(const std::string& fileName);
	std::string& GetPostfixPoloishExpression() { return m_postfixPoloishExpression; }
	std::string& GetInitialState() { return m_initialState; }
	std::unordered_set<std::string>& GetFinalStates() { return m_finalStates; }
	std::unordered_set<std::string>& GetStates() { return m_states; }
	std::unordered_set<std::string>& GetAlphabet() { return m_alphabet; }

	void regulateExpressionToPostfix();

	virtual ~Automaton() = default;
};

