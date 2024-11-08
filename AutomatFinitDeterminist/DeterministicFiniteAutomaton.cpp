#include "DeterministicFiniteAutomaton.h"

int DeterministicFiniteAutomaton::priority(char op)
{
    switch (op) 
    {
    case '|': return 1;
    case '.': return 2;
    case '*': return 3;
    default: return 0;
    }
}

std::string& DeterministicFiniteAutomaton::addConcatenateSimbol(const std::string& expresion)
{
	std::string newExpresion = expresion;
    for (size_t i = 0; i < expresion.size() - 1; i++) 
    {
        if ((isalnum(expresion[i]) || expresion[i] == ')' || expresion[i] == '*') &&
            (isalnum(expresion[i + 1]) || expresion[i + 1] == '(')) 
        {
            newExpresion.insert(i + 1, 1, '.');
        }
    }
	return newExpresion;
}

void DeterministicFiniteAutomaton::infixToPostfix(std::string& infix)
{
    std::stack<char> charStack;

    for (char c : infix) 
    {
        if (isalnum(c)) 
        {
            m_postfixPoloishExpression += c;
        }
        else if (c == '(') 
        {
            charStack.push(c);
        }
        else if (c == ')') 
        {
            while (!charStack.empty() && charStack.top() != '(') 
            {
                m_postfixPoloishExpression += charStack.top();
                charStack.pop();
            }
            charStack.pop();
        }
        else 
        {
            while (!charStack.empty() && priority(charStack.top()) >= priority(c))
            {
                m_postfixPoloishExpression += charStack.top();
                charStack.pop();
            }
            charStack.push(c);
        }
    }

    while (!charStack.empty()) 
    {
        m_postfixPoloishExpression += charStack.top();
        charStack.pop();
    }
}

void DeterministicFiniteAutomaton::regulateExpressionToPostfix()
{
	if (m_regulateExpression.empty())
	{
		std::string expression;
		std::cout << "The expression is empty\n";
		std::cout << "Input the expression: ";
		std::cin >> expression;
		SetRegulateExpression(expression);
		regulateExpressionToPostfix();
	}
	else
	{
		infixToPostfix(addConcatenateSimbol(m_regulateExpression));
	}
}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::vector<std::string> states, std::unordered_set<std::string> alphabet, std::vector<Tranzition> tranzitions, std::string initialState, std::unordered_set<std::string> finalStates):
    m_states{ states },
    m_alphabet{ alphabet },
    m_tranzitions{ tranzitions },
    m_initialState{ initialState },
    m_finalStates{ finalStates }
{
}



