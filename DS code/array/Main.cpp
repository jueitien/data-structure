#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#include "Store.hpp"

void searchbababa(){
    std::cout << "\nEnter the searching method: \n1.Linear Search \n2.Binary Search\n\nEnter your choice: ";
    std::string input;
    std::getline(std::cin, input);
    int search_target = std::stoi(input);

    if (search_target == 1) {
        SearchByLinear();
    } else if (search_target == 2) {
        SearchByBinary();
    } else {
        std::cout << "Invalid Input";
    }
}

void showMenu() {
    int choice;
    int subchoice;
    char input[100];


    do {
        printf("\n--- Financial Transaction System ---\n");
        printf("1. Store CSV Data by Payment Channel\n");
        printf("2. Store CSV Data by All\n");
        printf("3. Search by Keyword\n");
        printf("4. Output First N Rows\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                storeSeperate();
                break;

            case 2:
                storeAll();
                break;

            case 3:
                searchbababa();
                break;

            default:
                printf("Invalid choice. Try again.\n");
    }

} while (choice != 5);


}

int main() {
    showMenu();
    return 0;
}
