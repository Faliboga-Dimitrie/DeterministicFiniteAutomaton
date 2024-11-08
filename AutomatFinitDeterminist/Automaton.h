#pragma once
#include <vector>
#include <unordered_set>
#include <functional>
#include <stack>
#include <iostream>
#include <string>

class Automaton
{
	std::vector<std::string> m_states;
	std::unordered_set<std::string> m_alphabet;
	std::string m_initialState;
	std::unordered_set<std::string> m_finalStates;
	std::string m_regulateExpression;
	std::string m_postfixPoloishExpression;

	int priority(char op);
	std::string& addConcatenateSimbol(const std::string& expresion);
	void infixToPostfix(std::string& infix);
	void regulateExpressionToPostfix();

public:

	Automaton() = default;
	Automaton(std::vector<std::string> states, std::unordered_set<std::string> alphabet, std::string initialState, std::unordered_set<std::string> finalStates);

	void SetRegulateExpression(const std::string& expression) { m_regulateExpression = expression; }
	std::string& GetRegulateExpression() { return m_regulateExpression; }
	std::string& GetPostfixPoloishExpression() { return m_postfixPoloishExpression; }
	std::string& GetInitialState() { return m_initialState; }
	std::unordered_set<std::string>& GetFinalStates() { return m_finalStates; }
	std::vector<std::string>& GetStates() { return m_states; }
	std::unordered_set<std::string>& GetAlphabet() { return m_alphabet; }

	virtual ~Automaton() = default;
};

