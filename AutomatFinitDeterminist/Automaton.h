#pragma once
#include <vector>
#include <unordered_set>
#include <functional>
#include <stack>
#include <iostream>
#include <string>
#include <fstream>
#include "Tranzition.h"

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
	std::vector<Tranzition> m_tranzitions;

	int priority(AutomatonOperation op);
	void addConcatenateSimbol();
	void infixToPostfix();
	void ReadRegulateExpression(const std::string& fileName);

public:

	Automaton() = default;
	Automaton(const std::unordered_set<std::string>& states,
		const std::unordered_set<std::string>& alphabet,
		const std::vector<Tranzition>& tranzitions,
		const std::string& initialState,
		const std::unordered_set<std::string>& finalStates);
	Automaton(const Automaton& automaton) = default;

	const std::string& GetPostfixPoloishExpression() const { return m_postfixPoloishExpression; }
	const std::string& GetInitialState() const { return m_initialState; }
	const std::unordered_set<std::string>& GetFinalStates() const { return m_finalStates; }
	const std::unordered_set<std::string>& GetStates() const { return m_states; }
	const std::unordered_set<std::string>& GetAlphabet() const { return m_alphabet; }
	const std::vector<Tranzition>& GetTranzitions() const { return m_tranzitions; }

	void SetInitialState(const std::string& intialState) { m_initialState = intialState; }
	void SetFinalStates(const std::unordered_set<std::string>& finalStates) { m_finalStates = finalStates; }
	void SetAlphabet(const std::unordered_set<std::string>& alphabet) { m_alphabet = alphabet; }
	void SetStates(const std::unordered_set<std::string>& states) { m_states = states; }

	void AddState(const std::string& state) { m_states.insert(state); }
	void AddFinalState(const std::string& state) { m_finalStates.insert(state); }
	void AddSymbol(const std::string& symbol) { m_alphabet.insert(symbol); }
	void AddTranzition(const Tranzition& tranzition) { m_tranzitions.push_back(tranzition); }

	void regulateExpressionToPostfix(const std::string& fileName);
	void PrintAutomaton();

	virtual ~Automaton() = default;
};

