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
    std::cout << "\033[36m====== = DFA TOOL MENU ====== =\033[0m\n"; 
    std::cout << "\033[37m1. Print the regular expression\033[0m\n";  
    std::cout << "\033[37m2. Print the automaton\033[0m\n";
    std::cout << "\033[37m3. Check if a word is accepted\033[0m\n";
    std::cout << "\033[37m4. Change the regular expression\033[0m\n";
    std::cout << "\033[37m5. Display the menu options\033[0m\n";
    std::cout << "\033[37m6. Clear the screen\033[0m\n";
    std::cout << "\033[31m7. Exit\033[0m\n";
    std::cout << "\033[36m=============================\033[0m\n"; 
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
                std::cout << "\033[36mThe regular expression is: \033[0m" << automatonData << "\n\n";
				std::cout << "To see the menu options, press 5.\n";
                break;
            case switchOptions::PrintAutomaton:
                dfa.PrintAutomaton(automatonOutputFile, false);
                automatonOutputFile << "The automaton is:\n";
                dfa.PrintAutomaton(automatonOutputFile, true);
                std::cout << "\033[32mThe automaton has been saved in " << automatonOutputFileName << ".\033[0m\n";
                displayOptionMenu();
                break;
            case switchOptions::CheckAnotherWord:
            {
                std::cout << "\033[36mThe regular expression is: \033[0m" << automatonData << "\n";
                std::cout << "Enter a word: ";
                std::string word;
                std::cin >> word;
                bool isAccepted = dfa.CheckWord(word);
                std::cout << "The word '" << word << "' is ";
                std::cout << (isAccepted ? "\033[32m**ACCEPTED**" : "\033[31m**NOT ACCEPTED**") << "\033[0m by the automaton.\n\n";
                std::cout << "To see the menu options, press 5.\n";
                break;
            }
            case switchOptions::ChangeRegulateExpression:
            {
                std::cout << "\033[33mEnter the new regular expression: \033[0m";
                std::string newExpression;
                std::cin.ignore();
                std::getline(std::cin, newExpression);
                automatonData = newExpression;
                std::ofstream file("regulateExpression.txt", std::ios::trunc);
                if (!file) {
                    std::cerr << "\033[31mError: Could not open 'regulateExpression.txt' for writing.\033[0m\n";
                    break;
                }
                file << newExpression;
                file.close();

                dfa.clearDeterministicFiniteAutomaton();

                try {
                    dfa.convertAFNtoAFD("regulateExpression.txt");
                    std::cout << "\033[32mAutomaton updated successfully with the new regular expression.\033[0m\n\n";
                }
                catch (const std::runtime_error& e) {
                    std::cerr << "\033[31mError: " << e.what() << "\033[0m\n";
                }
				catch (const InvalidRegexException& ex) {
					std::cerr << "\033[31mError: " << ex.what() << "\033[0m\n";
				}
				displayOptionMenu();
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
                    std::cout << "Returning to the menu...\n\n";
					displayOptionMenu();
                }
                break;
            }
            default:
                std::cout << "\033[31mInvalid option. Please try again.\033[0m\n";
                std::cout << "To see the menu options, press 5.\n";
                break;
            }
        }
    }
    else {
		std::cerr << "Error: The automaton is invalid. Please check the file and try again.\n";
    }

    std::cout << "\033[36mThank you for using the Deterministic Finite Automaton Tool. Goodbye!\033[0m\n";
	automatonOutputFile.close();

    return 0;
}
