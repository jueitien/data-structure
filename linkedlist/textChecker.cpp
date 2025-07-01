#include <iostream>
#include <string>

// Returns true if user wants to return to main menu, false if exit
bool text_checker_menu() {
    std::string input;
    while (true) {
        std::cout << "\n";
        std::cout << "Type 'menu' to return to main menu, 'exit' to quit: ";
        std::getline(std::cin, input);
        if (input == "menu") {
            return true; // Return to main menu
        } else if (input == "exit") {
            return false; // Exit program
        } else {
            std::cout << "Invalid input. Please try again.\n";
        }
    }
}
