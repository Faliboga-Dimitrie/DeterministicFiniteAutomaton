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
            throw InvalidRegexException("Invalid character in the regular expression:" + std::string(1, c));
        }

        if (c == '*' || c == '|' || c == '.') {
            if (i == 0 || prevChar == '\0' || prevChar == '|' || prevChar == '.' || prevChar == '(' || prevChar == '*') {
                throw InvalidRegexException("Invalid operator or incorrectly placed operator in the regular expression.");
            }
        }

        if (c == '(') {
            openParens++;
        }
        else if (c == ')') {
            openParens--;
            if (openParens < 0) { 
                throw InvalidRegexException("Closed parentheses without a matching pair in the regular expression.");
            }
            if (prevChar == '|' || prevChar == '.' || prevChar == '(') {
                throw InvalidRegexException("Closed parenthesis after an invalid operator.");
            }
        }

        if (prevChar == '*' && c == '(') {
            throw InvalidRegexException("The '*' operator followed by an open parenthesis, which is invalid.");
        }

        if (!isspace(c)) {
            prevChar = c;
        }
    }

    if (openParens != 0) {
        throw InvalidRegexException("The parentheses are not balanced in the regular expression.");
    }
    if (prevChar == '|' || prevChar == '.' || prevChar == '(') {
        throw InvalidRegexException("The regular expression ends with an operator or an open parenthesis.");
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

void Automaton::PrintAutomaton(std::ofstream& automatonOutputFile, bool inFile)
{
    int columnWidth = 10; 
    for (const auto& state : GetStates())
        columnWidth = std::max(columnWidth, static_cast<int>(state.length()));
    for (const auto& symbol : GetAlphabet())
        columnWidth = std::max(columnWidth, static_cast<int>(symbol.length()));

    std::set<std::string, NaturalOrder> states(GetStates().begin(), GetStates().end());
    std::set<std::string> alphabet(GetAlphabet().begin(), GetAlphabet().end());
    std::set<std::string> finalStates(GetFinalStates().begin(), GetFinalStates().end());

    if (!inFile)
    {
        std::cout << "=============================\n";
        std::cout << "        AUTOMATON DETAILS\n";
        std::cout << "=============================\n";
        std::cout << "\nStates:\n";
    }
    else
    {
		automatonOutputFile << "=============================\n";
		automatonOutputFile << "        AUTOMATON DETAILS\n";
		automatonOutputFile << "=============================\n";
        automatonOutputFile << "\nStates:\n";
    }	

    for (const auto& state : states)
    {
		if (!inFile)
			std::cout << "  - " << state << "\n";
		else
			automatonOutputFile << "  - " << state << "\n";
    }

	if (!inFile)
        std::cout << "\nAlphabet:\n";
	else
		automatonOutputFile << "\nAlphabet:\n";

    for (const auto& symbol : alphabet)
    {
		if (!inFile)
            std::cout << "  - " << symbol << "\n";
		else
			automatonOutputFile << "  - " << symbol << "\n";
    }

    if (!inFile)
    {
        std::cout << "\nInitial State:\n";
        std::cout << "  - " << GetInitialState() << "\n";
        std::cout << "\nFinal States:\n";
    }
    else
	{
		automatonOutputFile << "\nInitial State:\n";
		automatonOutputFile << "  - " << GetInitialState() << "\n";
        automatonOutputFile << "\nFinal States:\n";
	}

    for (const auto& state : finalStates)
    {
		if (!inFile)
			std::cout << "  - " << state << "\n";
		else
			automatonOutputFile << "  - " << state << "\n";
    }

    if (!inFile)
    {
		std::cout << "\nTranzitions:\n";
		std::cout << std::setw(columnWidth) << "From"
			<< std::setw(columnWidth) << "Symbol"
			<< std::setw(columnWidth) << "To"
			<< "\n";
	}
	else
	{
		automatonOutputFile << "\nTranzitions:\n";
		automatonOutputFile << std::setw(columnWidth) << "From"
			<< std::setw(columnWidth) << "Symbol"
			<< std::setw(columnWidth) << "To"
			<< "\n";
	}

    for (const auto& tranzition : GetTranzitions())
    {
        if (!inFile)
        {
            std::cout << std::setw(columnWidth) << tranzition.GetFromState()
                << std::setw(columnWidth) << tranzition.GetSymbol()
                << std::setw(columnWidth) << tranzition.GetToState()
                << "\n";
        }
        else
        {
            automatonOutputFile << std::setw(columnWidth) << tranzition.GetFromState()
                << std::setw(columnWidth) << tranzition.GetSymbol()
                << std::setw(columnWidth) << tranzition.GetToState()
                << "\n";
        }
    }

	if (!inFile)
		std::cout << "=============================\n";
	else
		automatonOutputFile << "=============================\n";
}

void Automaton::clearAutomaton()
{
	m_states.clear();
	m_alphabet.clear();
	m_initialState.clear();
	m_finalStates.clear();
	m_regulateExpression.clear();
	m_postfixPoloishExpression.clear();
	m_tranzitions.clear();
}
