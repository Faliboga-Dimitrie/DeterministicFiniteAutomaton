#include <iostream>
#include "Automaton.h"
#include "ConstructAutomaton.h"

int main()
{
	Automaton a;
	a.regulateExpressionToPostfix();
	std::cout << a.GetPostfixPoloishExpression();
}
