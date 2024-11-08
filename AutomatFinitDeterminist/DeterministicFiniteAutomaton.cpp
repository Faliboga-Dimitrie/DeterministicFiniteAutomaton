#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::vector<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFDTranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates):
	Automaton(states, alphabet, initialState, finalStates),
	m_tranzitions{ tranzitions }
{
}

bool DeterministicFiniteAutomaton::VerifyAutomaton()
{
	std::unordered_set<std::string> states;
	std::unordered_set<std::string> alphabet;
	std::unordered_set<std::string> finalStates;
	std::string initialState;
	std::vector<AFDTranzition> tranzitions;

	if (GetStates().empty())
	{
		std::cout << "The set of states is empty\n";
		return false;
	}

	if (GetAlphabet().empty())
	{
		std::cout << "The alphabet is empty\n";
		return false;
	}

	if (GetFinalStates().empty())
	{
		std::cout << "The set of final states is empty\n";
		return false;
	}

	if (GetInitialState().empty())
	{
		std::cout << "The initial state is empty\n";
		return false;
	}

	for (const auto& state : GetStates())
	{
		states.insert(state);
	}

	for (const auto& letter : GetAlphabet())
	{
		alphabet.insert(letter);
	}

	for (const auto& finalState : GetFinalStates())
	{
		finalStates.insert(finalState);
	}

	initialState = GetInitialState();

	for (const auto& tranzition : m_tranzitions)
	{
		tranzitions.push_back(tranzition);
	}

	for (const auto& state : states)
	{
		if (alphabet.find(state) != alphabet.end())
		{
			std::cout << "The states and the alphabet are not disjoint\n";
			return false;
		}
	}

	if (states.size() != GetStates().size())
	{
		std::cout << "The states are not unique\n";
		return false;
	}

	if (alphabet.size() != GetAlphabet().size())
	{
		std::cout << "The alphabet is not unique\n";
		return false;
	}

	if (finalStates.size() != GetFinalStates().size())
	{
		std::cout << "The final states are not unique\n";
		return false;
	}

	if (states.find(initialState) == states.end())
	{
		std::cout << "The initial state is not in the set of states\n";
		return false;
	}

	for (const auto& finalState : finalStates)
	{
		if (states.find(finalState) == states.end())
		{
			std::cout << "The final state is not in the set of states\n";
			return false;
		}
	}

	for (const auto& tranzition : tranzitions)
	{
		if (states.find(tranzition.GetFromState()) == states.end())
		{
			std::cout << "The source state of a tranzition is not in the set of states\n";
			return false;
		}

		if (states.find(tranzition.GetToState()) == states.end())
		{
			std::cout << "The destination state of a tranzition is not in the set of states\n";
			return false;
		}

		if (alphabet.find(tranzition.GetSymbol()) == alphabet.end())
		{
			std::cout << "The letter of a tranzition is not in the alphabet\n";
			return false;
		}

		if (tranzition.GetSymbol().size() != 1)
		{
			std::cout << "The letter of a tranzition is not a single character\n";
			return false;
		}

		if (tranzition.GetFromState() == "" || tranzition.GetToState() == "")
		{
			std::cout << "The tranzition is not complete\n";
			return false;
		}

		if(tranzition.GetSymbol() == "" || tranzition.GetSymbol() == "lambda")
		{
			std::cout << "Lamba transitions are not accepted in a AFD\n";
			return false;
		}
	}

	return true;
}



