#include "ConstructAutomaton.h"

NonDeterministicFiniteAutomaton ConstructAutomaton::construct(std::string initialAutomaton)
{
	std::stack<NonDeterministicFiniteAutomaton> automatons;

	//Se creaza primul automat
	NonDeterministicFiniteAutomaton firstAutomaton({ "q0", "q1" }, { std::to_string(initialAutomaton[0]) }, { {"q0", std::to_string(initialAutomaton[0]), {"q1"}} }, "q0", { "q1" });

	statesCounter += 2;

	if (initialAutomaton.length() == 1)
		return firstAutomaton;

	automatons.push(firstAutomaton);

	for (size_t i = 1; i < initialAutomaton.length(); i++) {
		
		
		std::unordered_set<std::string> currentStates;

		std::unordered_set<std::string> currentAlphabet;
		std::vector<AFNTranzition> currentTranzition;
		std::string currentInState;
		std::unordered_set<std::string> currentFinStates;

		switch (initialAutomaton[i]) {
		case '.':
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
			std::vector<AFNTranzition> tranzitionSet1 = automaton1.GetTranzitions();
			std::vector<AFNTranzition> tranzitionSet2 = automaton2.GetTranzitions();
			currentTranzition.insert(currentTranzition.end(), tranzitionSet1.begin(), tranzitionSet1.end());
			currentTranzition.insert(currentTranzition.end(), tranzitionSet2.begin(), tranzitionSet2.end());

			//Se salveaza starea finala a primului automat
			std::string state3 = *automaton1.GetFinalStates().begin();

			//Ultima tranzitie a primului automat isi schimba toState-ul catre starea initiala a celui de al doilea automat
			for (auto& tr: currentTranzition){
				if (tr.GetToStates()[0] == state3) {
					tr.SetToStates({automaton2.GetInitialState()});
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
		case '|':
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
			std::vector<AFNTranzition> tranzitionSet1 = automaton1.GetTranzitions();
			std::vector<AFNTranzition> tranzitionSet2 = automaton2.GetTranzitions();
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
		case '*':
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

			currentAlphabet.insert(std::to_string(initialAutomaton[i]));

			currentTranzition.push_back({ state1, std::to_string(initialAutomaton[i]), {state2} });

			currentInState = state1;

			currentFinStates.insert(state2);

			NonDeterministicFiniteAutomaton currentAutomaton(currentStates, currentAlphabet, currentTranzition, currentInState, currentFinStates);

			automatons.push(currentAutomaton);

			statesCounter += 2;

			break;
		}
		}
	}

	return automatons.top();
}

//Cred ca putem sterge asta
/*std::unordered_set<std::string> states;
	states.insert("q0");
	states.insert("q1");

	std::unordered_set<std::string> alphabet;
	alphabet.insert(std::to_string(initialAutomaton[0]));

	std::vector<AFDTranzition> tranzitions;
	AFDTranzition firstTranzition("q0", std::to_string(initialAutomaton[0]), "q1");
	tranzitions.push_back(firstTranzition);

	std::string initialState = "q0";

	std::unordered_set<std::string> finalStates;
	finalStates.insert("q1");

	DeterministicFiniteAutomaton firstAutomaton(states, alphabet, tranzitions, initialState, finalStates);*/
