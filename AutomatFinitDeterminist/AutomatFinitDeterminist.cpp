#include "DeterministicFiniteAutomaton.h"

enum class switchOptions
{
	PrintRegulateExpression,
	PrintAutomaton,
	CheckAnotherWord,
	ChangeRegulateExpression,
    DispayOptionMenu,
    ClearScreen,
	Exit,
    None
};

switchOptions stringToSwitchOption(const std::string& str)
{
	if (str == "1")
	{
		return switchOptions::PrintRegulateExpression;
	}
	if (str == "2")
	{
		return switchOptions::PrintAutomaton;
	}
	if (str == "3")
	{
		return switchOptions::CheckAnotherWord;
	}
    if (str == "4")
    {
        return switchOptions::ChangeRegulateExpression;
    }
    if (str == "5")
    {
        return switchOptions::DispayOptionMenu;
    }

	if (str == "6")
	{
		return switchOptions::ClearScreen;
	}
	if (str == "7")
	{
		return switchOptions::Exit;
	}
	return switchOptions::None;
}

void displayOptionMenu() {
    std::cout << "\n======= DFA TOOL MENU =======\n";
    std::cout << "1. Print the regular expression\n";
    std::cout << "2. Print the automaton\n";
    std::cout << "3. Check if a word is accepted\n";
	std::cout << "4. Change the regular expression\n";
    std::cout << "5. Display the menu options\n";
    std::cout << "6. Clear the screen\n";
    std::cout << "7. Exit\n";
    std::cout << "=============================\n";
    std::cout << "Your choice: ";
}

int main() 
{
    DeterministicFiniteAutomaton dfa;
    std::string automatonOutputFileName = "AutomatonOutput.txt";
	std::string regulateExpressionFileName = "regulateExpression.txt";
    std::ifstream file(regulateExpressionFileName);
    std::ofstream automatonOutputFile(automatonOutputFileName, std::ios::trunc);

    if (!automatonOutputFile) {
        std::cerr << "Error: Could not open '" << automatonOutputFileName << "' for writing.\n";
        return 1;
    }

    std::cout << "Welcome to the Deterministic Finite Automaton Tool!\n";

    try
    {
        dfa.convertAFNtoAFD(regulateExpressionFileName);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const InvalidRegexException& ex) 
    {
        std::cerr << "Eroare: " << ex.what() << '\n';
    }

    std::string automatonData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (dfa.VerifyAutomaton()) 
	{
        std::cout << "\033[1;32mAutomaton loaded successfully!\033[0m\n\n"; 
		displayOptionMenu();
        bool isRunning = true;
        while (isRunning) {
            std::string option;
            std::cin >> option;
            switchOptions switchOption = stringToSwitchOption(option);

            switch (switchOption)
            {
            case switchOptions::PrintRegulateExpression:
                std::cout << "The regular expression is: " << automatonData << "\n";
                break;
            case switchOptions::PrintAutomaton:
                dfa.PrintAutomaton(automatonOutputFile, false);
                automatonOutputFile << "The automaton is:\n";
                dfa.PrintAutomaton(automatonOutputFile, true);
                std::cout << "The automaton has been saved in " << automatonOutputFileName << ".\n";
				displayOptionMenu();
                break;
            case switchOptions::CheckAnotherWord:
            {
                std::cout << "The regular expression is: " << automatonData << "\n";
                std::cout << "Enter a word: ";
                std::string word;
                std::cin >> word;
                std::cout << "The word '" << word << "' is "
                    << (dfa.CheckWord(word) ? "**ACCEPTED**" : "**NOT ACCEPTED**")
                    << " by the automaton.\n";
                break;
            }
            case switchOptions::ChangeRegulateExpression:
            {
                std::cout << "Enter the new regular expression: ";
                std::string newExpression;
                std::cin.ignore();
                std::getline(std::cin, newExpression);
                automatonData = newExpression;
                std::ofstream file("regulateExpression.txt", std::ios::trunc);
                if (!file) {
                    std::cerr << "Error: Could not open 'regulateExpression.txt' for writing.\n";
                    break;
                }
                file << newExpression;
                file.close();

                dfa.clearDeterministicFiniteAutomaton();

                try {
                    dfa.convertAFNtoAFD("regulateExpression.txt");
                    std::cout << "Automaton updated successfully with the new regular expression.\n";
                }
                catch (const std::runtime_error& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            case switchOptions::DispayOptionMenu:
                displayOptionMenu();
                break;
            case switchOptions::ClearScreen:
                system("cls");
                displayOptionMenu();
                break;
            case switchOptions::Exit:
            {
                std::cout << "Are you sure you want to exit? (yes/no): ";
                std::string confirmation;
                std::cin >> confirmation;
                if (confirmation == "yes" || confirmation == "y") {
                    isRunning = false;
                    system("cls");
                }
                else {
                    std::cout << "Returning to the menu...\n";
                }
                break;
            }
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
            }
        }
    }
    else {
		std::cerr << "Error: The automaton is invalid. Please check the file and try again.\n";
    }

    std::cout << "Thank you for using the Deterministic Finite Automaton Tool. Goodbye!\n";
	automatonOutputFile.close();

    return 0;
}
