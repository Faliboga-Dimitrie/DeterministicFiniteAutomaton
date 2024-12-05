#include "NonDeterministicFiniteAutomaton.h"

NonDeterministicFiniteAutomaton::NonDeterministicFiniteAutomaton(const std::unordered_set<std::string>& states,
	const std::unordered_set<std::string>& alphabet,
	const std::vector<Tranzition>& tranzitions,
	const std::string& initialState,
	const std::unordered_set<std::string>& finalStates):
	Automaton(states, alphabet, tranzitions, initialState, finalStates)
{
}

NonDeterministicFiniteAutomaton::NonDeterministicFiniteAutomaton(const NonDeterministicFiniteAutomaton& automaton) :
	Automaton(automaton)
{
}

void NonDeterministicFiniteAutomaton::buildAutomaton(const std::string& regulateExpression)
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

	NonDeterministicFiniteAutomaton firstAutomaton(
		FirstStates,
		FirstAlphabet,
		FirstTranzition,
		FirstInState,
		FirstFinStates);

	statesCounter += 2;

	if (regulateExpression.length() == 1)
		return;

	automatons.push(firstAutomaton);

	for (size_t i = 1; i < regulateExpression.length(); i++)
	{
		std::unordered_set<std::string> currentStates;
		std::unordered_set<std::string> currentAlphabet;
		std::vector<Tranzition> currentTranzition;
		std::string currentInState;
		std::unordered_set<std::string> currentFinStates;

		switch (getOperation(regulateExpression[i])) {
		case AutomatonOperation::CONCATENATION:
		{
			NonDeterministicFiniteAutomaton automaton2 = automatons.top();
			automatons.pop();
			NonDeterministicFiniteAutomaton automaton1 = automatons.top();
			automatons.pop();

			currentStates.insert(automaton1.GetStates().begin(), automaton1.GetStates().end());
			currentStates.insert(automaton2.GetStates().begin(), automaton2.GetStates().end());

			currentAlphabet.insert(automaton1.GetAlphabet().begin(), automaton1.GetAlphabet().end());
			currentAlphabet.insert(automaton2.GetAlphabet().begin(), automaton2.GetAlphabet().end());

			currentTranzition.insert(currentTranzition.end(), automaton1.GetTranzitions().begin(), automaton1.GetTranzitions().end());
			currentTranzition.insert(currentTranzition.end(), automaton2.GetTranzitions().begin(), automaton2.GetTranzitions().end());

			std::string state3 = *automaton1.GetFinalStates().begin();

			for (auto& tr : currentTranzition)
			{
				if (tr.GetToState() == state3) 
				{
					tr.SetToState(automaton2.GetInitialState());
				}
			}

			currentStates.erase(state3);

			currentInState = automaton1.GetInitialState();
			currentFinStates = automaton2.GetFinalStates();

			NonDeterministicFiniteAutomaton currentAutomaton(
				currentStates,
				currentAlphabet,
				currentTranzition,
				currentInState,
				currentFinStates);

			automatons.push(currentAutomaton);

			break;
		}
		case AutomatonOperation::UNION:
		{
			NonDeterministicFiniteAutomaton automaton2 = automatons.top();
			automatons.pop();
			NonDeterministicFiniteAutomaton automaton1 = automatons.top();
			automatons.pop();

			currentInState = "q" + std::to_string(statesCounter);
			std::string currentFinState = "q" + std::to_string(statesCounter + 1);
			currentFinStates.insert(currentFinState);
			currentStates.insert(currentInState);
			currentStates.insert(currentFinState);

			currentStates.insert(automaton1.GetStates().begin(), automaton1.GetStates().end());
			currentStates.insert(automaton2.GetStates().begin(), automaton2.GetStates().end());

			currentAlphabet.insert(automaton1.GetAlphabet().begin(), automaton1.GetAlphabet().end());
			currentAlphabet.insert(automaton2.GetAlphabet().begin(), automaton2.GetAlphabet().end());

			currentTranzition.insert(currentTranzition.end(), automaton1.GetTranzitions().begin(), automaton1.GetTranzitions().end());
			currentTranzition.insert(currentTranzition.end(), automaton2.GetTranzitions().begin(), automaton2.GetTranzitions().end());

			currentTranzition.push_back({ currentInState, "", {automaton1.GetInitialState()} });
			currentTranzition.push_back({ currentInState, "", {automaton2.GetInitialState()} });

			std::string finalState1 = *automaton1.GetFinalStates().begin();
			std::string finalState2 = *automaton2.GetFinalStates().begin();

			currentTranzition.push_back({ finalState1, "", {currentFinState} });
			currentTranzition.push_back({ finalState2, "", {currentFinState} });

			NonDeterministicFiniteAutomaton currentAutomaton(
				currentStates,
				currentAlphabet,
				currentTranzition,
				currentInState,
				currentFinStates);

			automatons.push(currentAutomaton);

			statesCounter += 2;

			break;
		}
		case AutomatonOperation::KLEENE_STAR:
		{
			NonDeterministicFiniteAutomaton automaton1 = automatons.top();
			automatons.pop();

			currentInState = "q" + std::to_string(statesCounter);
			std::string currentFinState = "q" + std::to_string(statesCounter + 1);
			currentFinStates.insert(currentFinState);

			automaton1.AddState(currentInState);
			automaton1.AddState(currentFinState);

			std::string prevFinalState = *automaton1.GetFinalStates().begin();

			automaton1.AddTranzition({ currentInState, "", {automaton1.GetInitialState()} });
			automaton1.AddTranzition({ currentInState, "", {currentFinState} });
			automaton1.AddTranzition({ prevFinalState, "", {automaton1.GetInitialState()} });
			automaton1.AddTranzition({ prevFinalState, "", {currentFinState} });

			automaton1.SetInitialState(currentInState);
			automaton1.SetFinalStates(currentFinStates);

			automatons.push(automaton1);

			statesCounter += 2;

			break;
		}
		default:
		{
			std::string symbol = regulateExpression.substr(i, 1);

			std::string state1 = "q" + std::to_string(statesCounter);
			std::string state2 = "q" + std::to_string(statesCounter + 1);

			std::unordered_set<std::string> localStates = { state1, state2 };
			std::unordered_set<std::string> localAlphabet = { symbol };
			std::vector<Tranzition> localTranzitions = { { state1, symbol, { state2 } } };
			std::unordered_set<std::string> localFinalStates = { state2 };

			NonDeterministicFiniteAutomaton currentAutomaton(
				localStates,
				localAlphabet,
				localTranzitions,
				state1,
				localFinalStates);

			automatons.push(currentAutomaton);

			statesCounter += 2;

			break;
		}
		}
	}
	*this = automatons.top();
}

void NonDeterministicFiniteAutomaton::clearNonDeterministicFiniteAutomaton()
{
	Automaton::clearAutomaton();
}
