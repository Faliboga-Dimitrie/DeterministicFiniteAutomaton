#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(const std::unordered_set<std::string>& states,
	const std::unordered_set<std::string>& alphabet,
	const std::vector<Tranzition>& tranzitions,
	const std::string& initialState,
	const std::unordered_set<std::string>& finalStates):
	Automaton(states, alphabet, tranzitions, initialState, finalStates)
{
}

std::unordered_set<std::string> DeterministicFiniteAutomaton::getLambdaClose(const std::string& state, const std::vector<Tranzition>& tranzitions) {
	std::unordered_set<std::string> lambdaClose;
	std::stack<std::string> stackStates;
	stackStates.push(state);

	while (!stackStates.empty()) {
		std::string currentState = stackStates.top();
		lambdaClose.insert(currentState);
		stackStates.pop();

		for (auto& tr : tranzitions) {
			if (tr.GetFromState() == currentState && tr.GetSymbol() == "")
				stackStates.push(tr.GetToState());
		}
	}
	return lambdaClose;
}

void DeterministicFiniteAutomaton::convertAFNtoAFD(const std::string& regulateExpresionFileName)
{
	Automaton::regulateExpressionToPostfix(regulateExpresionFileName);
	m_nfa.buildAutomaton(GetPostfixPoloishExpression());

	std::unordered_map<std::string, std::unordered_set<std::string>> newStates;
	std::vector<Tranzition> tranzitions = m_nfa.GetTranzitions();

	SetInitialState(m_nfa.GetInitialState());
	SetAlphabet(m_nfa.GetAlphabet());

	std::stack<std::string> stackStates;
	std::string firstLambda = "";
	stackStates.push(m_nfa.GetInitialState());

	while (!stackStates.empty()) {
		std::string currentState = stackStates.top();
		stackStates.pop();

		AddState(currentState);

		if (m_nfa.GetFinalStates().find(currentState) != m_nfa.GetFinalStates().end())
			AddFinalState(currentState);

		for (auto& tr : tranzitions) {
			if (tr.GetFromState() == currentState && tr.GetSymbol() != "") {
				AddTranzition(tr);
				stackStates.push(tr.GetToState());
			}
			else if (tr.GetFromState() == currentState && tr.GetSymbol() == "") {
				firstLambda = tr.GetFromState();
				break;
			}
		}

		if (firstLambda != "")
			break;
	}

	if (firstLambda == "")
		return;

	std::unordered_set<std::string> firstLambdaClose = getLambdaClose(firstLambda, m_nfa.GetTranzitions());
	newStates[firstLambda] = firstLambdaClose;
	size_t firstLambdaNumber = std::stoi(firstLambda.substr(1)); 

	std::queue<std::string> queueStates;
	queueStates.push(firstLambda);

	while (!queueStates.empty()) {
		std::string currentState = queueStates.front();
		queueStates.pop();
		AddState(currentState);

		for (std::string symbol : m_nfa.GetAlphabet()) {
			bool foundState = false;
			std::unordered_set<std::string> statesSet;

			for (std::string state : newStates[currentState]) {

				for (auto& tr : tranzitions) {

					if (tr.GetFromState() == state && tr.GetSymbol() == symbol)
						statesSet.insert(tr.GetToState());
				}
			}

			if (statesSet.empty())
				continue;

			std::unordered_set<std::string> lambdaClose;
			for (std::string state : statesSet) {
				std::unordered_set<std::string> lambdaAux = getLambdaClose(state, m_nfa.GetTranzitions());
				lambdaClose.insert(lambdaAux.begin(), lambdaAux.end());
			}

			for (auto& p : newStates) {
				if (p.second == lambdaClose) {
					AddTranzition({ currentState, symbol, p.first });
					foundState = true;
					break;
				}
			}
			if (foundState)
				continue;

			std::string nextState = "q" + std::to_string(++firstLambdaNumber);
			queueStates.push(nextState);

			newStates[nextState] = lambdaClose;
			AddState(nextState);
			AddTranzition({ currentState, symbol, nextState });
		}
	}

	for (auto& p : newStates)
		for (std::string state : p.second)
			if (m_nfa.GetFinalStates().find(state) != m_nfa.GetFinalStates().end())
				AddFinalState(p.first);
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

	for (const auto& tranzition : GetTranzitions())
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
		for (const auto& tranzition : GetTranzitions())
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

void DeterministicFiniteAutomaton::clearDeterministicFiniteAutomaton()
{
	Automaton::clearAutomaton();
	m_nfa.clearNonDeterministicFiniteAutomaton();
}




