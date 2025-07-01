#include <iostream>
#include "store.hpp"

int main() {
    std::string filename = "financial_fraud_detection_dataset.csv";
    Transaction* head = load_transactions_from_csv(filename);

    // Split the list by transaction type
    split_by_transaction_type(head);

    // Count the number of transactions in the linked list
    int count = 0;
    Transaction* curr = head;
    while (curr) {
        ++count;
        curr = curr->next;
    }

    std::cout << "Number of transactions processed: " << count << std::endl;

    // Ask user for transaction type and count
    std::string type;
    int num;
    std::cout << "Enter transaction type (withdrawal, deposit, transfer, payment): ";
    std::cin >> type;
    std::cout << "How many transactions to display? ";
    std::cin >> num;

    // Select the appropriate list
    Transaction* selected = nullptr;
    if (type == "withdrawal") selected = withdrawal_head;
    else if (type == "deposit") selected = deposit_head;
    else if (type == "transfer") selected = transfer_head;
    else if (type == "payment") selected = payment_head;
    else {
        std::cout << "Invalid type." << std::endl;
        // Free the linked lists before exiting
        // Free all four lists
        Transaction* lists[] = {withdrawal_head, deposit_head, transfer_head, payment_head};
        for (int i = 0; i < 4; ++i) {
            curr = lists[i];
            while (curr) {
                Transaction* next = curr->next;
                delete curr;
                curr = next;
            }
        }
        return 1;
    }

    // Print the requested number of transactions
    curr = selected;
    int printed = 0;
    while (curr && printed < num) {
        // Print basic info (customize as needed)
        std::cout << "ID: " << curr->transaction_id
                  << ", Amount: " << curr->amount
                  << ", Type: " << curr->transaction_type
                  << ", Fraud: " << (curr->is_fraud ? "Yes" : "No")
                  << std::endl;
        curr = curr->next;
        ++printed;
    }

    // Free the linked lists
    // Free all four lists
    Transaction* lists[] = {withdrawal_head, deposit_head, transfer_head, payment_head};
    for (int i = 0; i < 4; ++i) {
        curr = lists[i];
        while (curr) {
            Transaction* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    return 0;
}
