#pragma once
#include "Automaton.h"
#include "NonDeterministicFiniteAutomaton.h"
#include "AFDTranzition.h"
#include <list>

class ConstructAutomaton
{
	size_t statesCounter = 0;
	NonDeterministicFiniteAutomaton m_automaton;
	//std::string construct(std::string initialAutomaton); //string?
	NonDeterministicFiniteAutomaton construct(std::string initialAutomaton);
};

