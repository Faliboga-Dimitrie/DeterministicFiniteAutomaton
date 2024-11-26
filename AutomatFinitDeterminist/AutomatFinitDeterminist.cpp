#include <iostream>
#include "DeterministicFiniteAutomaton.h"

int main()
{
	Automaton a;
	a.regulateExpressionToPostfix();
	std::cout << a.GetPostfixPoloishExpression();
}
