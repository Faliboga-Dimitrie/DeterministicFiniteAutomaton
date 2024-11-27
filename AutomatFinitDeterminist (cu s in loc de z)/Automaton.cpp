#include "Automaton.h"

int Automaton::priority(AutomatonOperation op)
{
	switch (op)
	{
	case AutomatonOperation::UNION: return 1;
	case AutomatonOperation::CONCATENATION: return 2;
	case AutomatonOperation::KLEENE_STAR: return 3;
	default: return 0;
	}
}

void Automaton::addConcatenateSimbol()
{
    for (size_t i = 0; i < m_regulateExpression.size() - 1; i++)
    {
        if ((isalnum(m_regulateExpression[i]) || m_regulateExpression[i] == ')' || m_regulateExpression[i] == '*') &&
            (isalnum(m_regulateExpression[i + 1]) || m_regulateExpression[i + 1] == '('))
        {
            m_regulateExpression.insert(i + 1, 1, '.');
        }
    }
}

void Automaton::infixToPostfix()
{
    std::stack<char> charStack;

    for (char c : m_regulateExpression)
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
            while (!charStack.empty() && priority(getOperation(charStack.top())) >= priority(getOperation(c)))
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

Automaton::AutomatonOperation Automaton::getOperation(char c)
{
	if (c == '.')
	{
		return AutomatonOperation::CONCATENATION;
	}
	else if (c == '|')
	{
		return AutomatonOperation::UNION;
	}
	else if (c == '*')
	{
		return AutomatonOperation::KLEENE_STAR;
	}
	return AutomatonOperation::EROOR;

}

void Automaton::regulateExpressionToPostfix()
{
    if (m_regulateExpression.empty())
    {
		ReadRegulateExpression("regulateExpression.txt");

    }
    addConcatenateSimbol();
    infixToPostfix();
}

Automaton::Automaton(std::unordered_set<std::string> states, std::unordered_set<std::string> alphabet, std::string initialState, std::unordered_set<std::string> finalStates):
    m_states{ states },
    m_alphabet{ alphabet },
    m_initialState{ initialState },
    m_finalStates{ finalStates }
{
}

void Automaton::ReadRegulateExpression(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cout << "The file " << fileName << " could not be opened\n";
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		m_regulateExpression += line;
	}
	file.close();
}
