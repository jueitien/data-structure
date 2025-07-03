#include <iostream>
#include "Data_structure.hpp"
#include "Store.hpp"
#include <chrono>// timestemp





void SearchByLinear(){
    int resultSize = transactionCount;
    Transaction* result1 = new Transaction[resultSize];
    int Size = 0;
    cout<< transactionCount << "\n";
    std::string target;

    do {
        std::cout << "Enter the transaction type to search for (e.g., payment, deposit, transfer, withdrawal): ";
        std::getline(std::cin, target); 

        if (target != "payment" && target != "deposit" && target != "transfer" && target != "withdrawal") {
            std::cout << "Invalid input\n";
        }
    } while (target != "payment" && target != "deposit" && target != "transfer" && target != "withdrawal");
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < resultSize; i++)
    {
        if (filtered_transactions[i].transaction_type==target)
        {
            // std::cout << filtered_transactions[i].transaction_type << " | " << filtered_transactions[i].transaction_id << " | " << filtered_transactions[i].device_hash<<"\n";
            result1[Size++] = filtered_transactions[i];
        }
        
    }
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
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
