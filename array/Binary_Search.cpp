#include <iostream>
#include "Data_structure.hpp"
#include "Store.hpp"
#include <chrono>
#include <algorithm>
#include <fstream>
#include "Store.hpp"



void SearchByBinary() {
    int resultSize = transactionCount;
    Transaction* result1 = new Transaction[resultSize];
    int Size = 0;
    cout<< transactionCount << "\n";
    cout << "The data is sorting....\n";
    Transaction* sorted_transactions = filtered_transactions;

    std::sort(&sorted_transactions[0], &sorted_transactions[0] + transactionCount, [](const auto& a, const auto& b) {
        return a.transaction_type < b.transaction_type;
    });

    cout << "The data completely sorting\n";
    std::string target;

    do {
        std::cout << "Enter the transaction type to search for (e.g., payment, deposit, transfer, withdrawal): ";
        std::getline(std::cin, target); 

        if (target != "payment" && target != "deposit" && target != "transfer" && target != "withdrawal") {
            std::cout << "Invalid input\n";
        }
    } while (target != "payment" && target != "deposit" && target != "transfer" && target != "withdrawal");

auto start = std::chrono::high_resolution_clock::now();
    int left = 0;
    int right = transactionCount - 1;
    int foundIndex = -1;

    // Binary Search to find target type 
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (filtered_transactions[mid].transaction_type == target) {
            foundIndex = mid;
            break;
        }
        else if (filtered_transactions[mid].transaction_type < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    if (foundIndex != -1) {
        // Step 2: Expand left and right to find all
        int i = foundIndex;

        // Expand left
        while (i >= 0 && filtered_transactions[i].transaction_type == target) {
            --i;
        }
        int start = i + 1; // one step back

        // Expand right
        i = foundIndex;
        while (i < transactionCount && filtered_transactions[i].transaction_type == target) {
            ++i;
        }
        int end = i - 1;

        // Step 3: Print all matching entries
        for (int j = start; j <= end; ++j) {
            result1[Size++] = filtered_transactions[j];
        }
    } 

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Search execution time: " << duration.count() << " microseconds\n";
    std::string ans;
    cout << "Do you want to generate a result in Json format?(Y/N)\n Enter your choice: ";
    getline(cin, ans);
    if (ans == "Y")
    {
        generateJsonReport(result1,Size);
    }  else {
        return;
    }
    delete[] result1;

}
