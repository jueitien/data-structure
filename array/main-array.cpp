#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "Store.hpp"
#include "Data_structure.hpp" 

void saveSortedTransactionsToFile(const string& filename = "array/Sortoutput.txt") {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Failed to open output file." << endl;
        return;
    }

    outFile << "Transaction ID,Location,Amount" << endl;
    for (int i = 0; i < transactionCount; ++i) { 
        outFile << filtered_transactions[i].transaction_id << ","
                << filtered_transactions[i].location << ","
                << filtered_transactions[i].amount << endl;
    }

    outFile.close();
    cout << "Sorted transactions saved to '" << filename << "' successfully." << endl;
}


void showMenu() {
    int choice;
    char input[100];


    do {
        printf("\n--- Financial Transaction System ---\n");
        printf("1. Store CSV Data by Payment Channel\n");
        printf("2. Store CSV Data by All\n");
        printf("3. Search by BinarySearch\n");
        printf("4. Search by LinearSearch\n");
        printf("5. Sort by MergeSort\n");
        printf("6. Sort by HeapSort\n");
        printf("7. Exit\n");
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
                if (transactionCount == 0) {
                    printf("Please store data first (Option 1 or 2).\n");
                } else {
                    SearchByBinary();
                }
                break;
            case 4:
                if (transactionCount == 0) {
                    printf("Please store data first (Option 1 or 2).\n");
                } else {
                    SearchByLinear();
                }
                break;
            case 5:
                if (transactionCount == 0) {
                    printf("Please store data first (Option 1 or 2).\n");
                } else {
                    sortByMergeSort();
                    saveSortedTransactionsToFile("array/Sortoutput.txt");
                } 
                break;
            case 6:
                if (transactionCount == 0) {
                    printf("Please store data first (Option 1 or 2).\n");
                } else {
                    heapSort();
                    saveSortedTransactionsToFile("array/Sortoutput.txt");
                }            
                break;
            case 7:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 7);

}

int main() {
    showMenu();
    return 0;
}
