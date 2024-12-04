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
    size_t i = 0;
    while (i < m_regulateExpression.size() - 1)
    {
        char curr = m_regulateExpression[i];
        char next = m_regulateExpression[i + 1];

        if ((isalnum(curr) || curr == ')' || curr == '*') &&
            (isalnum(next) || next == '('))
        {
            m_regulateExpression.insert(i + 1, 1, '.'); 
            i++; 
        }

        i++; 
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

void Automaton::validateExpression(const std::string& expression) {
    int openParens = 0; 
    char prevChar = '\0'; 

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (!isalnum(c) && c != '(' && c != ')' && c != '*' && c != '|' && c != '.') {
            throw InvalidRegexException("Caracter invalid în expresia regulatã: " + std::string(1, c));
        }

        if (c == '*' || c == '|' || c == '.') {
            if (i == 0 || prevChar == '\0' || prevChar == '|' || prevChar == '.' || prevChar == '(') {
                throw InvalidRegexException("Operator invalid sau plasat incorect în expresia regulatã.");
            }
        }

        if (c == '(') {
            openParens++;
        }
        else if (c == ')') {
            openParens--;
            if (openParens < 0) { 
                throw InvalidRegexException("Paranteze închise fãrã corespondent în expresia regulatã.");
            }
            if (prevChar == '|' || prevChar == '.' || prevChar == '(') {
                throw InvalidRegexException("Parantezã închisã dupã un operator invalid.");
            }
        }

        if (prevChar == '*' && c == '(') {
            throw InvalidRegexException("Operator '*' urmat de parantezã deschisã, ceea ce este invalid.");
        }

        if (!isspace(c)) {
            prevChar = c;
        }
    }

    if (openParens != 0) {
        throw InvalidRegexException("Parantezele nu sunt echilibrate în expresia regulatã.");
    }
    if (prevChar == '|' || prevChar == '.' || prevChar == '(') {
        throw InvalidRegexException("Expresia regulatã se terminã cu un operator sau parantezã deschisã.");
    }
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
		validateExpression(m_regulateExpression);
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
