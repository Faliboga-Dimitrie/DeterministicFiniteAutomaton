#include "DeterministicFiniteAutomaton.h"


//void DeterministicFiniteAutomaton::AFNtoAFD()
//{
//	Automaton::regulateExpressionToPostfix();
//	m_nfa.buildAutomaton(GetPostfixPoloishExpression());
//
//	std::unordered_set<std::string> states;
//	std::unordered_set<std::string> alphabet;
//	std::vector<AFDTranzition> tranzitions;
//	std::unordered_set<std::string> finalStates;
//	std::string initialState = GetInitialState();
//	std::unordered_set<std::string> initialStateSet;
//	initialStateSet.insert(initialState);
//	std::unordered_set<std::string> initialStateSetClosure = m_nfa.EClosure(initialStateSet);
//	std::vector<std::unordered_set<std::string>> statesSet;
//	statesSet.push_back(initialStateSetClosure);
//	states.insert(m_nfa.GetInitialState());
//	alphabet = m_nfa.GetAlphabet();
//	for (size_t i = 0; i < statesSet.size(); i++)
//	{
//		for (const auto& symbol : alphabet)
//		{
//			std::unordered_set<std::string> newStateSet = m_nfa.EClosure(m_nfa.Move(statesSet[i], symbol));
//			if (newStateSet.empty())
//			{
//				continue;
//			}
//			bool found = false;
//			for (size_t j = 0; j < statesSet.size(); j++)
//			{
//				if (newStateSet == statesSet[j])
//				{
//					found = true;
//					tranzitions.push_back(AFDTranzition("q" + std::to_string(i), symbol, "q" + std::to_string(j)));
//					break;
//				}
//			}
//			if (!found)
//			{
//				statesSet.push_back(newStateSet);
//				tranzitions.push_back(AFDTranzition("q" + std::to_string(i), symbol, "q" + std::to_string(statesSet.size() - 1)));
//			}
//		}
//	}
//	for (size_t i = 0; i < statesSet.size(); i++)
//	{
//		for (const auto& state : statesSet[i])
//		{
//			if (m_nfa.GetFinalStates().find(state) != m_nfa.GetFinalStates().end())
//			{
//				finalStates.insert("q" + std::to_string(i));
//				break;
//			}
//		}
//	}
//	SetStates(states);
//	SetAlphabet(alphabet);
//	SetInitialState("q0");
//	SetFinalStates(finalStates);
//	m_tranzitions = tranzitions;
//}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFDTranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates):
	Automaton(states, alphabet, initialState, finalStates),
	m_tranzitions{ tranzitions }
{
}

bool DeterministicFiniteAutomaton::VerifyAutomaton()
{
	//AFNtoAFD();
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

	for (const auto& tranzition : m_tranzitions)
	{
		if (tranzition.IsInitial() == true)
		{
			if (tranzition.GetSymbol() != firstSimbol)
			{
				std::cout << "The first symbol of the postfix expression is not the same as the symbol of the initial tranzition\n";
				return false;
			}
		}

		if (GetStates().find(tranzition.GetFromState()) == GetStates().end())
		{
			std::cout << "The state " << tranzition.GetFromState() << " is not in the set of states\n";
			return false;
		}

		if (GetAlphabet().find(tranzition.GetSymbol()) == GetAlphabet().end())
		{
			std::cout << "The symbol " << tranzition.GetSymbol() << " is not in the alphabet\n";
			return false;
		}

		if(tranzition.GetSymbol() == "")
		{
			std::cout << "The symbol of the tranzition is lambda\n";
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
	std::cout << "Tranzitions: "<< std::endl;
	for (const auto& tranzition : m_tranzitions)
	{
		std::cout << tranzition.GetFromState() << " -- " << tranzition.GetSymbol() << " --> " << tranzition.GetToState() << std::endl;
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
		for (const auto& tranzition : m_tranzitions)
		{
			if (tranzition.GetFromState() == currentState && tranzition.GetSymbol() == std::string(1, symbol))
			{
				currentState = tranzition.GetToState();
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



