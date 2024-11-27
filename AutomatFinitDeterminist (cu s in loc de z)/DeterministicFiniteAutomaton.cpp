#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFDTransition> transitions, std::string initialState, std::unordered_set<std::string> finalStates):
	Automaton(states, alphabet, initialState, finalStates),
	m_transitions{ transitions }
{
}

bool DeterministicFiniteAutomaton::VerifyAutomaton()
{
	std::string firstSimbol = GetPostfixPoloishExpression().substr(0, 1);
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

	if (GetStates().find(GetInitialState()) == GetStates().end())
	{
		std::cout << "The initial state is not in the set of states\n";
		return false;
	}
	
	for (const auto& state : GetStates())
	{
		if (GetAlphabet().find(state) != GetAlphabet().end())
		{
			std::cout << "The state " << state << " is in the alphabet\n";
			return false;
		}
	}

	for (const auto& transition : m_transitions)
	{
		if (transition.IsInitial() == true)
		{
			if (transition.GetSymbol() != firstSimbol)
			{
				std::cout << "The first symbol of the postfix expression is not the same as the symbol of the initial transition\n";
				return false;
			}
		}

		if (GetStates().find(transition.GetFromState()) == GetStates().end())
		{
			std::cout << "The state " << transition.GetFromState() << " is not in the set of states\n";
			return false;
		}

		if (GetAlphabet().find(transition.GetSymbol()) == GetAlphabet().end())
		{
			std::cout << "The symbol " << transition.GetSymbol() << " is not in the alphabet\n";
			return false;
		}

		if(transition.GetSymbol() == "")
		{
			std::cout << "The symbol of the transition is lambda\n";
			return false;
		}
	}

	return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton()
{
	std::cout << "States: ";
	for (const auto& state : GetStates())
	{
		std::cout << state << " ";
	}
	std::cout << std::endl;
	std::cout << "Alphabet: ";
	for (const auto& symbol : GetAlphabet())
	{
		std::cout << symbol << " ";
	}
	std::cout << std::endl;
	std::cout << "Initial state: " << GetInitialState() << std::endl;
	std::cout << "Final states: ";
	for (const auto& state : GetFinalStates())
	{
		std::cout << state << " ";
	}
	std::cout << std::endl;
	std::cout << "Transitions: "<< std::endl;
	for (const auto& transition : m_transitions)
	{
		std::cout << transition.GetFromState() << " -- " << transition.GetSymbol() << " --> " << transition.GetToState() << std::endl;
	}
}

bool DeterministicFiniteAutomaton::CheckWord(const std::string& word)
{
	for (const auto& symbol : word)
	{
		if (GetAlphabet().find(std::string(1, symbol)) == GetAlphabet().end())
		{
			std::cout << "The symbol " << symbol << " is not in the alphabet\n";
			return false;
		}
	}

	std::string currentState = GetInitialState();
	for (const auto& symbol : word)
	{
		bool found = false;
		for (const auto& transition : m_transitions)
		{
			if (transition.GetFromState() == currentState && transition.GetSymbol() == std::string(1, symbol))
			{
				currentState = transition.GetToState();
				found = true;
				break;
			}
		}
		if (!found)
		{
			return false;
		}
	}

	if (GetFinalStates().find(currentState) == GetFinalStates().end())
	{
		return false;
	}

	return true;
}



