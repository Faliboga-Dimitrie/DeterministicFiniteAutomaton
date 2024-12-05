#include <iostream>
#include "DeterministicFiniteAutomaton.h"

enum class switchOptions
{
	PrintRegulateExpression,
	PrintAutomaton,
	CheckAnotherWord,
	ClearScreen,
	Exit,
	DispayOptionMenu,
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
		return switchOptions::ClearScreen;
	}
	if (str == "5")
	{
		return switchOptions::DispayOptionMenu;
	}
	if (str == "6")
	{
		return switchOptions::Exit;
	}
	return switchOptions::None;
}

void displayOptionMenu()
{
	std::cout << "Choose an option:\n";
	std::cout << "1. Print the regular expression\n";
	std::cout << "2. Print the automaton\n";
	std::cout << "3. Check another word\n";
	std::cout << "4. Clear screen\n";
	std::cout << "5. Display option menu\n";
	std::cout << "6. Exit\n";
}

int main() 
{
    DeterministicFiniteAutomaton dfa;

    std::cout << "Welcome to the Deterministic Finite Automaton Tool!\n";

    std::ifstream file("regulateExpression.txt");

    try
    {
        dfa.convertAFNtoAFD("regulateExpression.txt");
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

	std::string automatonOutputFileName = "AutomatonOutput.txt";
	std::ofstream automatonOutputFile(automatonOutputFileName);

    if (dfa.VerifyAutomaton()) 
	{
        std::cout << "Automaton loaded successfully!\n\n";
		displayOptionMenu();
		int ok = 0;
        do {
			ok = 0;
			std::string option;
            std::string word;
			std::cin >> option; 
			switchOptions switchOption = stringToSwitchOption(option);

            switch (switchOption)
            {
			case switchOptions::PrintRegulateExpression:
				std::cout << "The regulate expresion on witch the automaton is based: " << automatonData << "\n";
				std::cout << "Please select another option.\n";
				break;
			case switchOptions::PrintAutomaton:
				dfa.PrintAutomaton(automatonOutputFile,false);
				automatonOutputFile << "The automate is:\n";
				dfa.PrintAutomaton(automatonOutputFile,true);
				std::cout << "Please select another option.\n";
				break;
			case switchOptions::CheckAnotherWord:
                std::cout << "\nEnter a word to check if it is accepted by the automaton: ";
                std::cin >> word;

                if (dfa.CheckWord(word)) {
                    std::cout << "Result: The word '" << word << "' is **ACCEPTED** by the automaton.\n";
                }
                else {
                    std::cout << "Result: The word '" << word << "' is **NOT ACCEPTED** by the automaton.\n";
                }
				std::cout << "Please select another option.\n";
				break;
			case switchOptions::ClearScreen:
				system("cls");
				displayOptionMenu();
				break;
			case switchOptions::DispayOptionMenu:
				displayOptionMenu();
				break;
			case switchOptions::Exit:
				ok = 1;
				break;
            default:
				std::cout << "Invalid option. Please try again.\n";
                break;
            }

		} while (ok == 0);
    }
    else {
		std::cerr << "Error: The automaton is invalid. Please check the file and try again.\n";
    }

    std::cout << "Thank you for using the Deterministic Finite Automaton Tool. Goodbye!\n";

	automatonOutputFile.close();

    return 0;
}
