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

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<Tranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates):
	Automaton(states, alphabet, tranzitions, initialState, finalStates)
{
}

std::unordered_set<std::string> DeterministicFiniteAutomaton::getLambdaClose(std::string state, std::vector<Tranzition> tranzitions) {
	std::unordered_set<std::string> lambdaClose;
	std::stack<std::string> stackStates;
	stackStates.push(state);

	//Folosim un algoritm de tip DFS prin care adaugam toate starile conectate catre starea initiala care fac parte dintr-o tranzitie lambda
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

void DeterministicFiniteAutomaton::convertAFNtoAFD(NonDeterministicFiniteAutomaton nfa)
{
	std::unordered_map<std::string, std::unordered_set<std::string>> newStates;
	std::vector<Tranzition> tranzitions = nfa.GetTranzitions();

	SetInitialState("q0");
	SetAlphabet(nfa.GetAlphabet());

	std::stack<std::string> stackStates;
	std::string firstLambda = "";
	stackStates.push("q0");

	//Folosim un algoritm de tip DFS cu care parcurgem AFNul si punem toate starile 
	//care fac parte dintr-o tranzitie non-lambda in AFD pana gasim o tranzitie lambda
	while (!stackStates.empty()) {
		std::string currentState = stackStates.top();
		stackStates.pop();

		AddState(currentState);

		if (nfa.GetFinalStates().find(currentState) != nfa.GetFinalStates().end())
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

	//Daca nu am gasit tranzitii lambda, lasam automatul asa cum este
	if (firstLambda == "")
		return;

	//Folosim un algoritm de tip DFS prin care obtinem multimea starilor de tip lambda inchis a primului element care face parte dintr-o tranzitie lambda 
	//Initializam starea firstLambda din AFD ca fiind multimea lambda inchisa gasita mai sus
	std::unordered_set<std::string> firstLambdaClose = getLambdaClose(firstLambda, nfa.GetTranzitions());
	newStates[firstLambda] = firstLambdaClose;
	size_t firstLambdaNumber = firstLambda[1] - '0'; //folosit pentru numararea starilor

	std::queue<std::string> queueStates;
	queueStates.push(firstLambda);

	while (!queueStates.empty()) {
		std::string currentState = queueStates.front();
		queueStates.pop();
		AddState(currentState);

		for (std::string symbol : nfa.GetAlphabet()) {

			std::unordered_set<std::string> statesSet;

			//Cream functia de tranzitii pentru starea curenta
			for (std::string state : newStates[currentState]) {

				for (auto& tr : tranzitions) {

					if (tr.GetFromState() == state && tr.GetSymbol() == symbol)
						statesSet.insert(tr.GetToState());

				}
			}

			//Daca setul ramane gol dupa cautare, atunci aceasta tranzitie va duce catre multimea vida
			if (statesSet.empty())
				continue;

			//Daca aceasta functie de tranziti exista deja in map-ul nostru, atunci tranzitia va fi intre currentState si cealalta stare deja existenta
			bool foundState = false;
			for (auto& p : newStates) {
				if (p.second == statesSet) {
					AddTranzition({ currentState, symbol, p.first });
					foundState = true;
					break;
				}
			}
			if (foundState)
				continue;

			//Daca nu exista in map, atunci trebuia sa cream o noua stare
			//Cream functia de lambda inchis pentru starea curenta, care e formata din multimile lambda inchis a starilor din functia sa de tranzitii
			std::unordered_set<std::string> lambdaClose;
			for (std::string state : statesSet) {
				std::unordered_set<std::string> lambdaAux = getLambdaClose(state, nfa.GetTranzitions());
				lambdaClose.insert(lambdaAux.begin(), lambdaAux.end());
			}

			//Noua stare cu care currentState este in relatie de tranzitie se va adauga in queue
			std::string nextState = "q" + std::to_string(++firstLambdaNumber);
			queueStates.push(nextState);

			newStates[nextState] = lambdaClose;
			AddState(nextState);
			AddTranzition({ currentState, symbol, nextState });
		}
	}

	//Se adauga starile finale
	for (auto& p : newStates)
		for (std::string state : p.second)
			if (nfa.GetFinalStates().find(state) != nfa.GetFinalStates().end())
				AddFinalState(state); 
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
	for (const auto& tranzition : GetTranzitions())
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



