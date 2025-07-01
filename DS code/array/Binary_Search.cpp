#include <iostream>
#include "Data_structure.h"
#include "Store.hpp"
#include <chrono>
#include <algorithm>


void SearchByBinary() {

    cout << "The data is sorting";
    Transaction* sorted_transactions = filtered_transactions;

    std::sort(&sorted_transactions[0], &sorted_transactions[0] + transactionCount, [](const auto& a, const auto& b) {
        return a.transaction_type < b.transaction_type;
    });

    cout << "The data completely sorting";
    std::string target;
    std::cout << "Enter the transaction type to search for (e.g., payment, deposit, transfer, withdrawal): ";
    std::getline(std::cin, target); 

    if (target != "payment" && target != "deposit" && target != "transfer" && target != "withdrawal") {
        std::cout << "Please key in correct answer (payment, deposit, transfer, withdrawal), Invalid input\n";
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < transactionCount; i++) {
        if (filtered_transactions[i].transaction_type == target) {
            std::cout << filtered_transactions[i].transaction_type << " | " << filtered_transactions[i].transaction_id << "\n";
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Search execution time: " << duration.count() << " microseconds\n";
}

