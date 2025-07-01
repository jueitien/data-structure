#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Store.hpp"

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
                printf("Enter keyword to search: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0'; // remove newline
            case 4:
                printf("Enter number of rows to print: ");
                int n;
                scanf("%d", &n);
                break;
            case 5:
                printf("Exiting program...\n");
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
