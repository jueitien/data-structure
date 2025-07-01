#include <iostream>
#include "store.hpp"

int main() {
    std::string filename = "financial_fraud_detection_dataset.csv";
    Transaction* head = load_transactions_from_csv(filename);

    // Split the list by payment channel
    split_by_payment_channel(head);

    // Count the number of transactions in the linked list
    int count = 0;
    Transaction* curr = head;
    while (curr) {
        ++count;
        curr = curr->next;
    }

    std::cout << "Number of transactions processed: " << count << std::endl;

    // Operation menu
    std::cout << "Choose operation:\n";
    std::cout << "1. Display transactions by payment channel\n";
    std::cout << "2. Sort transactions\n";
    std::cout << "3. Search transactions\n";
    std::cout << "4. Generate JSON\n";
    std::cout << "Enter choice (1-4): ";
    int op;
    std::cin >> op;

    if (op == 1) {
        // Ask user for payment channel and count
        std::string channel;
        int num;
        std::cout << "Enter payment channel (card, ACH, UPI, wire_transfer): ";
        std::cin >> channel;
        std::cout << "How many transactions to display? ";
        std::cin >> num;

        // Select the appropriate list
        Transaction* selected = nullptr;
        if (channel == "card") selected = card_head;
        else if (channel == "ACH") selected = ACH_head;
        else if (channel == "UPI") selected = UPI_head;
        else if (channel == "wire_transfer") selected = wire_transfer_head;
        else {
            std::cout << "Invalid channel." << std::endl;
            // Free the linked lists before exiting
            Transaction* lists[] = {card_head, ACH_head, UPI_head, wire_transfer_head};
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
                      << ", Channel: " << curr->payment_channel
                      << ", Fraud: " << (curr->is_fraud ? "Yes" : "No")
                      << std::endl;
            curr = curr->next;
            ++printed;
        }
    } else if (op == 2) {
        std::cout << "Sort operation selected. (Not implemented)\n";
        // ...call sort function here...
    } else if (op == 3) {
        std::cout << "Search operation selected. (Not implemented)\n";
        // ...call search function here...
    } else if (op == 4) {
        std::cout << "Generate JSON operation selected. (Not implemented)\n";
        // ...call generate json function here...
    } else {
        std::cout << "Invalid operation.\n";
    }

    // Free the linked lists
    Transaction* lists[] = {card_head, ACH_head, UPI_head, wire_transfer_head};
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
