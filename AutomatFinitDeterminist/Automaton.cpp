#include "Automaton.h"

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

Automaton::Automaton(const std::unordered_set<std::string>& states,
    const std::unordered_set<std::string>& alphabet,
    const std::vector<Tranzition>& tranzitions,
    const std::string& initialState,
    const std::unordered_set<std::string>& finalStates) :
    m_states{ states },
    m_alphabet{ alphabet },
    m_initialState{ initialState },
    m_tranzitions{ tranzitions },
    m_finalStates{ finalStates }
{
}

void Automaton::regulateExpressionToPostfix(const std::string& fileName)
{
    if (m_regulateExpression.empty())
    {
		ReadRegulateExpression(fileName);

    }
    addConcatenateSimbol();
    infixToPostfix();
}

void Automaton::PrintAutomaton()
{
    std::cout << "States: ";
    for (const auto& state : GetStates())
    {
        std::cout << state << " ";
    }
    std::cout << std::endl;
    std::cout << "Alphabet: ";
    for (const auto& symbol : GetAlphabet())
    {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;
    std::cout << "Initial state: " << GetInitialState() << std::endl;
    std::cout << "Final states: ";
    for (const auto& state : GetFinalStates())
    {
        std::cout << state << " ";
    }
    std::cout << std::endl;
    std::cout << "Tranzitions: " << std::endl;
    for (const auto& tranzition : GetTranzitions())
    {
        std::cout << tranzition.GetFromState() << " -- " << tranzition.GetSymbol() << " --> " << tranzition.GetToState() << std::endl;
    }
}