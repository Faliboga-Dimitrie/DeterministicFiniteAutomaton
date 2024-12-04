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
        throw std::runtime_error("The file '" + fileName + "' could not be opened.");
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
    int columnWidth = 10; 
    for (const auto& state : GetStates())
        columnWidth = std::max(columnWidth, static_cast<int>(state.length()));
    for (const auto& symbol : GetAlphabet())
        columnWidth = std::max(columnWidth, static_cast<int>(symbol.length()));

    std::set<std::string, NaturalOrder> states(GetStates().begin(), GetStates().end());
    std::set<std::string> alphabet(GetAlphabet().begin(), GetAlphabet().end());
    std::set<std::string> finalStates(GetFinalStates().begin(), GetFinalStates().end());

    std::cout << "=============================\n";
    std::cout << "        AUTOMATON DETAILS\n";
    std::cout << "=============================\n";

    std::cout << "\nStates:\n";
    for (const auto& state : states)
    {
        std::cout << "  - " << state << "\n";
    }

    std::cout << "\nAlphabet:\n";
    for (const auto& symbol : alphabet)
    {
        std::cout << "  - " << symbol << "\n";
    }

    std::cout << "\nInitial State:\n";
    std::cout << "  - " << GetInitialState() << "\n";

    std::cout << "\nFinal States:\n";
    for (const auto& state : finalStates)
    {
        std::cout << "  - " << state << "\n";
    }

    std::cout << "\nTransition Table:\n";
    std::cout << std::setw(columnWidth) << "From State"
        << std::setw(columnWidth) << "Symbol"
        << std::setw(columnWidth) << "To State" << "\n";
    std::cout << std::string(columnWidth * 3, '-') << "\n";

    for (const auto& tranzition : GetTranzitions())
    {
        std::cout << std::setw(columnWidth) << tranzition.GetFromState()
            << std::setw(columnWidth) << tranzition.GetSymbol()
            << std::setw(columnWidth) << tranzition.GetToState()
            << "\n";
    }

    std::cout << "=============================\n";
}
