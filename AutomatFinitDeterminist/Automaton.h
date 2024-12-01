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
protected:

	enum class AutomatonOperation
	{
		CONCATENATION,
		UNION,
		KLEENE_STAR,
		EROOR
	};
	AutomatonOperation getOperation(char c);


private:

	std::unordered_set<std::string> m_states;
	std::unordered_set<std::string> m_alphabet;
	std::string m_initialState;
	std::unordered_set<std::string> m_finalStates;
	std::string m_regulateExpression;
	std::string m_postfixPoloishExpression;

	int priority(AutomatonOperation op);
	void addConcatenateSimbol();
	void infixToPostfix();
	
public:

	Automaton() = default;
	Automaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::string initialState, std::unordered_set<std::string> finalStates);
	Automaton(const Automaton& automaton) = default;

	void ReadRegulateExpression(const std::string& fileName);
	const std::string& GetPostfixPoloishExpression() const { return m_postfixPoloishExpression; }
	const std::string& GetInitialState() const { return m_initialState; }
	const std::unordered_set<std::string>& GetFinalStates() const { return m_finalStates; }
	const std::unordered_set<std::string>& GetStates() const { return m_states; }
	const std::unordered_set<std::string>& GetAlphabet() const { return m_alphabet; }
	void AddState(std::string state) { m_states.insert(state); }
	void AddFinalState(std::string state) { m_finalStates.insert(state); }
	void SetInitialState(std::string intialState) { m_initialState = intialState; }
	void SetFinalStates(std::unordered_set<std::string> finalStates) { m_finalStates = finalStates; }
	void SetAlphabet(std::unordered_set<std::string> alphabet) { m_alphabet = alphabet; }
	void SetStates(std::unordered_set<std::string> states) { m_states = states; }
	void AddSymbol(std::string symbol) { m_alphabet.insert(symbol); }

	void regulateExpressionToPostfix();

	virtual ~Automaton() = default;
};

