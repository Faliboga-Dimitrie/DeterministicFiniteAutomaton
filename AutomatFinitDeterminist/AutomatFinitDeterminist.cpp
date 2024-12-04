#include <iostream>
#include "DeterministicFiniteAutomaton.h"

int main() {
    std::string continuu;
    DeterministicFiniteAutomaton dfa;

    std::cout << "Welcome to the Deterministic Finite Automaton Tool!\n";
    std::cout << "Please enter the name of the file containing the automaton: ";
    std::string fileName;
    std::cin >> fileName;

    std::ifstream file(fileName);

    try
    {
        dfa.convertAFNtoAFD(fileName);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::string automatonData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (dfa.VerifyAutomaton()) {
        std::cout << "Automaton loaded successfully!\n\n";
        std::cout << "Automaton details:\n";
        dfa.PrintAutomaton();
        std::cout <<"Expresia regulata pe care se bazeaza automatul: " << automatonData << "\n";
        do {
            std::string word;
            std::cout << "\nEnter a word to check if it is accepted by the automaton: ";
            std::cin >> word;

            if (dfa.CheckWord(word)) {
                std::cout << "Result: The word '" << word << "' is **ACCEPTED** by the automaton.\n";
            }
            else {
                std::cout << "Result: The word '" << word << "' is **NOT ACCEPTED** by the automaton.\n";
            }

            std::cout << "Would you like to check another word? (yes/no): ";
            std::cin >> continuu;
            while (continuu != "yes" && continuu != "no") {
                std::cout << "Invalid input. Please enter 'yes' or 'no': ";
                std::cin >> continuu;
            }
        } while (continuu != "no");
    }
    else {
        std::cerr << "Error: The automaton is invalid. Please check the file and try again.\n";
    }

    std::cout << "Thank you for using the Deterministic Finite Automaton Tool. Goodbye!\n";
    return 0;
}
