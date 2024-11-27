#include "NonDeterministicFiniteAutomaton.h"

NonDeterministicFiniteAutomaton::NonDeterministicFiniteAutomaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::vector<AFNTransition> transitions, std::string initialState, std::unordered_set<std::string> finalStates):
	Automaton(states, alphabet, initialState, finalStates),
	m_transitions{ transitions }
{
}

NonDeterministicFiniteAutomaton::NonDeterministicFiniteAutomaton(const NonDeterministicFiniteAutomaton& automaton) :
	Automaton(automaton), m_transitions{ automaton.GetTransitions() }
{
}

void NonDeterministicFiniteAutomaton::AddToTransitions(std::vector<AFNTransition> transitions, std::string fromState, std::string symbol)
{

}

void NonDeterministicFiniteAutomaton::buildAutomaton(std::string regulateExpression)
{
	std::stack<NonDeterministicFiniteAutomaton> automatons;
	int statesCounter = 0;
	//Se creaza primul automat
	NonDeterministicFiniteAutomaton firstAutomaton({ "q0", "q1" }, { std::to_string(regulateExpression[0]) }, { {"q0", std::to_string(regulateExpression[0]), {"q1"}} }, "q0", { "q1" });

	statesCounter += 2;

	if (regulateExpression.length() == 1)
		return;

	automatons.push(firstAutomaton);

	for (size_t i = 1; i < regulateExpression.length(); i++) {


		std::unordered_set<std::string> currentStates;

		std::unordered_set<std::string> currentAlphabet;
		std::vector<AFNTransition> currentTransition;
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
			std::vector<AFNTransition> transitionSet1 = automaton1.GetTransitions();
			std::vector<AFNTransition> transitionSet2 = automaton2.GetTransitions();
			currentTransition.insert(currentTransition.end(), transitionSet1.begin(), transitionSet1.end());
			currentTransition.insert(currentTransition.end(), transitionSet2.begin(), transitionSet2.end());

			//Se salveaza starea finala a primului automat
			std::string state3 = *automaton1.GetFinalStates().begin();

			//Ultima tranzitie a primului automat isi schimba toState-ul catre starea initiala a celui de al doilea automat
			for (auto& tr : currentTransition) {
				if (tr.GetToStates()[0] == state3) {
					tr.SetToStates({ automaton2.GetInitialState() });
					break;
				}
			}

			//Se sterge starea finala a primului automat din set-ul de stari al noului automat deoarece nu mai e nevoie de ea
			currentStates.erase(state3);

			//Se seteaza starea initiala si finala a noului automat
			currentInState = automaton1.GetInitialState();
			currentFinStates = automaton2.GetFinalStates();

			//Se creaza noul automat si se adauga in stiva
			NonDeterministicFiniteAutomaton currentAutomaton(currentStates, currentAlphabet, currentTransition, currentInState, currentFinStates);

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
			std::vector<AFNTransition> transitionSet1 = automaton1.GetTransitions();
			std::vector<AFNTransition> transitionSet2 = automaton2.GetTransitions();
			currentTransition.insert(currentTransition.end(), transitionSet1.begin(), transitionSet1.end());
			currentTransition.insert(currentTransition.end(), transitionSet2.begin(), transitionSet2.end());

			//Se creaza tranzitile pentru starea initiala
			currentTransition.push_back({ currentInState, "", {automaton1.GetInitialState()} });
			currentTransition.push_back({ currentInState, "", {automaton2.GetInitialState()} });

			//Se salveaza starile finale ale celor 2 automate
			std::string finalState1 = *automaton1.GetFinalStates().begin();
			std::string finalState2 = *automaton2.GetFinalStates().begin();
			//Se creaza tranzitile pentru starea finala
			currentTransition.push_back({ finalState1, "", {currentFinState} });
			currentTransition.push_back({ finalState2, "", {currentFinState} });

			//Se creaza noul automat si se adauga in stiva
			NonDeterministicFiniteAutomaton currentAutomaton(currentStates, currentAlphabet, currentTransition, currentInState, currentFinStates);

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
			automaton1.AddTransition({ currentInState, "", {automaton1.GetInitialState()} });
			automaton1.AddTransition({ currentInState, "", {currentFinState} });
			automaton1.AddTransition({ prevFinalState, "", {automaton1.GetInitialState()} });
			automaton1.AddTransition({ prevFinalState, "", {currentFinState} });

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

			currentAlphabet.insert(std::to_string(regulateExpression[i]));

			currentTransition.push_back({ state1, std::to_string(regulateExpression[i]), {state2} });

			currentInState = state1;

			currentFinStates.insert(state2);

			NonDeterministicFiniteAutomaton currentAutomaton(currentStates, currentAlphabet, currentTransition, currentInState, currentFinStates);

			automatons.push(currentAutomaton);

			statesCounter += 2;

			break;
		}
		}
	}
	*this = automatons.top();
}
