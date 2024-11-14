#include <iostream>
#include "Automaton.h"

int main()
{
	Automaton a;
	a.regulateExpressionToPostfix();
	std::cout << a.GetPostfixPoloishExpression();
}
