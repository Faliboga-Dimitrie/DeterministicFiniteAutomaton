#include "NonDeterministicFiniteAutomaton.h"

NonDeterministicFiniteAutomaton::NonDeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<Tranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates):
	Automaton(states, alphabet, tranzitions, initialState, finalStates)
{
}

NonDeterministicFiniteAutomaton::NonDeterministicFiniteAutomaton(const NonDeterministicFiniteAutomaton& automaton) :
	Automaton(automaton)
{
}

void NonDeterministicFiniteAutomaton::buildAutomaton(std::string regulateExpression)
{
	std::stack<NonDeterministicFiniteAutomaton> automatons;
	int statesCounter = 0;
	std::unordered_set<std::string> FirstStates;
	std::unordered_set<std::string> FirstAlphabet;
	std::vector<Tranzition> FirstTranzition;
	std::string FirstInState;
	std::unordered_set<std::string> FirstFinStates;

	FirstStates.insert("q0");
	FirstStates.insert("q1");

	FirstAlphabet.insert(regulateExpression.substr(0,1));
	FirstTranzition.push_back({ "q0", regulateExpression.substr(0,1), {"q1"} });
	FirstInState = "q0";
	FirstFinStates.insert("q1");
	//Se creaza primul automat
	NonDeterministicFiniteAutomaton firstAutomaton(FirstStates, FirstAlphabet, FirstTranzition, FirstInState, FirstFinStates);

	statesCounter += 2;

	if (regulateExpression.length() == 1)
		return;

	automatons.push(firstAutomaton);

	for (size_t i = 1; i < regulateExpression.length(); i++) {


		std::unordered_set<std::string> currentStates;

		std::unordered_set<std::string> currentAlphabet;
		std::vector<Tranzition> currentTranzition;
		std::string currentInState;
		std::unordered_set<std::string> currentFinStates;

		switch (getOperation(regulateExpression[i])) {
		case AutomatonOperation::CONCATENATION:
		{
			//Se scot ultimele 2 automate din stiva
			NonDeterministicFiniteAutomaton automaton2 = automatons.top();
			automatons.pop();
			NonDeterministicFiniteAutomaton automaton1 = automatons.top();
			automatons.pop();

			//Se combina set-urile de stari ale celor 2 automate
			std::unordered_set<std::string> statesSet1 = automaton1.GetStates();
			std::unordered_set<std::string> statesSet2 = automaton2.GetStates();
			currentStates.insert(statesSet1.begin(), statesSet1.end());
			currentStates.insert(statesSet2.begin(), statesSet2.end());

			//Se combina alfabeturile celor 2 automate
			std::unordered_set<std::string> alphabet1 = automaton1.GetAlphabet();
			std::unordered_set<std::string> alphabet2 = automaton2.GetAlphabet();
			currentAlphabet.insert(alphabet1.begin(), alphabet1.end());
			currentAlphabet.insert(alphabet2.begin(), alphabet2.end());

			//Se combina tranzitile celor 2 automate
			std::vector<Tranzition> tranzitionSet1 = automaton1.GetTranzitions();
			std::vector<Tranzition> tranzitionSet2 = automaton2.GetTranzitions();
			currentTranzition.insert(currentTranzition.end(), tranzitionSet1.begin(), tranzitionSet1.end());
			currentTranzition.insert(currentTranzition.end(), tranzitionSet2.begin(), tranzitionSet2.end());

			//Se salveaza starea finala a primului automat
			std::string state3 = *automaton1.GetFinalStates().begin();

			//Ultima tranzitie a primului automat isi schimba toState-ul catre starea initiala a celui de al doilea automat
			for (auto& tr : currentTranzition) {
				if (tr.GetToState() == state3) {
					tr.SetToState(automaton2.GetInitialState());
					break;
				}
			}

			//Se sterge starea finala a primului automat din set-ul de stari al noului automat deoarece nu mai e nevoie de ea
			currentStates.erase(state3);

			//Se seteaza starea initiala si finala a noului automat
			currentInState = automaton1.GetInitialState();
			currentFinStates = automaton2.GetFinalStates();

			//Se creaza noul automat si se adauga in stiva
			NonDeterministicFiniteAutomaton currentAutomaton(currentStates, currentAlphabet, currentTranzition, currentInState, currentFinStates);

			automatons.push(currentAutomaton);

			break;
		}
		case AutomatonOperation::UNION:
		{
			//Se scot ultimele 2 automate din stiva
			NonDeterministicFiniteAutomaton automaton2 = automatons.top();
			automatons.pop();
			NonDeterministicFiniteAutomaton automaton1 = automatons.top();
			automatons.pop();

			//Se creaza starea initiala si finala pentru noul automat
			currentInState = "q" + std::to_string(statesCounter);
			std::string currentFinState = "q" + std::to_string(statesCounter + 1);
			currentFinStates.insert(currentFinState);

			//Se combina set-urile de stari ale celor 2 automate
			std::unordered_set<std::string> statesSet1 = automaton1.GetStates();
			std::unordered_set<std::string> statesSet2 = automaton2.GetStates();
			currentStates.insert(statesSet1.begin(), statesSet1.end());
			currentStates.insert(statesSet2.begin(), statesSet2.end());

			//Se combina alfabeturile celor 2 automate
			std::unordered_set<std::string> alphabet1 = automaton1.GetAlphabet();
			std::unordered_set<std::string> alphabet2 = automaton2.GetAlphabet();
			currentAlphabet.insert(alphabet1.begin(), alphabet1.end());
			currentAlphabet.insert(alphabet2.begin(), alphabet2.end());

			//Se combina tranzitile celor 2 automate
			std::vector<Tranzition> tranzitionSet1 = automaton1.GetTranzitions();
			std::vector<Tranzition> tranzitionSet2 = automaton2.GetTranzitions();
			currentTranzition.insert(currentTranzition.end(), tranzitionSet1.begin(), tranzitionSet1.end());
			currentTranzition.insert(currentTranzition.end(), tranzitionSet2.begin(), tranzitionSet2.end());

			//Se creaza tranzitile pentru starea initiala
			currentTranzition.push_back({ currentInState, "", {automaton1.GetInitialState()} });
			currentTranzition.push_back({ currentInState, "", {automaton2.GetInitialState()} });

			//Se salveaza starile finale ale celor 2 automate
			std::string finalState1 = *automaton1.GetFinalStates().begin();
			std::string finalState2 = *automaton2.GetFinalStates().begin();
			//Se creaza tranzitile pentru starea finala
			currentTranzition.push_back({ finalState1, "", {currentFinState} });
			currentTranzition.push_back({ finalState2, "", {currentFinState} });

			//Se creaza noul automat si se adauga in stiva
			NonDeterministicFiniteAutomaton currentAutomaton(currentStates, currentAlphabet, currentTranzition, currentInState, currentFinStates);

			automatons.push(currentAutomaton);

			statesCounter += 2;

			break;
		}
		case AutomatonOperation::KLEENE_STAR:
		{
			//Se scoate ultimul automat din stiva
			NonDeterministicFiniteAutomaton automaton1 = automatons.top();
			automatons.pop();

			//Se creaza si adauga starea initiala si finala pentru noul automat
			currentInState = "q" + std::to_string(statesCounter);
			std::string currentFinState = "q" + std::to_string(statesCounter + 1);
			currentFinStates.insert(currentFinState);

			//Se adauga noile stari initiale si finale in setul de stari al noului automat
			automaton1.AddState(currentInState);
			automaton1.AddState(currentFinState);

			//Se salveaza starea finala a automatului initial
			std::string prevFinalState = *automaton1.GetFinalStates().begin();

			//Se creaza noile tranziti pentru automat
			automaton1.AddTranzition({ currentInState, "", {automaton1.GetInitialState()} });
			automaton1.AddTranzition({ currentInState, "", {currentFinState} });
			automaton1.AddTranzition({ prevFinalState, "", {automaton1.GetInitialState()} });
			automaton1.AddTranzition({ prevFinalState, "", {currentFinState} });

			//Se seteaza noile stari initiale si finale a noului automat
			automaton1.SetInitialState(currentInState);
			automaton1.SetFinalStates(currentFinStates);

			//Se adauga inapoi "noul" automat
			automatons.push(automaton1);

			statesCounter += 2;

			break;
		}
		default: //Cazul default este pentru caractere
		{
			std::string state1 = "q" + std::to_string(statesCounter);
			std::string state2 = "q" + std::to_string(statesCounter + 1);

			currentStates.insert(state1);
			currentStates.insert(state2);

			currentAlphabet.insert(regulateExpression.substr(i, i - 1));

			currentTranzition.push_back({ state1, regulateExpression.substr(i,i-1), state2 });

			currentInState = state1;

			currentFinStates.insert(state2);

			NonDeterministicFiniteAutomaton currentAutomaton(currentStates, currentAlphabet, currentTranzition, currentInState, currentFinStates);

			automatons.push(currentAutomaton);

			statesCounter += 2;

			break;
		}
		}
	}
	*this = automatons.top();
}

//std::unordered_set<std::string> NonDeterministicFiniteAutomaton::EClosure(const std::unordered_set<std::string>& states)
//{
//	std::unordered_set<std::string> closure = states; 
//	std::stack<std::string> toProcess;   
//
//	for (const auto& state : states)
//	{
//		toProcess.push(state);
//	}
//
//	while (!toProcess.empty())
//	{
//		std::string currentState = toProcess.top();
//		toProcess.pop();
//
//		for (const auto& tranzition : m_tranzitions)
//		{
//			if (tranzition.GetFromState() == currentState && tranzition.GetSymbol() == "")
//			{
//				for (const auto& nextState : tranzition.GetToStates())
//				{
//					if (closure.find(nextState) == closure.end())
//					{
//						closure.insert(nextState);
//						toProcess.push(nextState);
//					}
//				}
//			}
//		}
//	}
//
//	return closure;
//}
//
//std::unordered_set<std::string> NonDeterministicFiniteAutomaton::Move(const std::unordered_set<std::string>& states, const std::string& symbol)
//{
//	std::unordered_set<std::string> newStates;
//	for (const auto& state : states)
//	{
//		for (const auto& tranzition : m_tranzitions)
//		{
//			if (tranzition.GetFromState() == state && tranzition.GetSymbol() == symbol)
//			{
//				for (const auto& nextState : tranzition.GetToStates())
//				{
//					newStates.insert(nextState);
//				}
//			}
//		}
//	}
//
//	return newStates;
//}