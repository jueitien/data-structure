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
    std::cout << "Enter the transaction type to search for (e.g., payment, deposit, transfer, withdrawal): ";
    std::getline(std::cin, target); 

    if (target != "payment" && target != "deposit" && target != "transfer" && target != "withdrawal") {
        std::cout << "Please key in correct answer (payment, deposit, transfer, withdrawal), Invalid input\n";
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < resultSize; i++) {
        if (filtered_transactions[i].transaction_type == target) {
            std::cout << filtered_transactions[i].transaction_type << " | " << filtered_transactions[i].transaction_id << "\n";
            result1[Size++] = filtered_transactions[i];
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
    }
    delete[] result1;

}

