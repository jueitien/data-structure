#include <iostream>
#include "store.hpp"
#include "textChecker.hpp"
#include "search.hpp"
#include "sort.hpp" // Include sort.hpp for sort_transactions

int main() {
    std::string filename = "financial_fraud_detection_dataset.csv";
    Transaction* head = load_transactions_from_csv(filename);

    Transaction* curr = head;

    // Split the list by payment channel
    split_by_payment_channel(head);

    bool running = true;
    while (running) {
        // Operation menu
        int op = 0;
        while (true) {
            std::cout << "Choose operation:\n";
            std::cout << "1. Display transactions by payment channel\n";
            std::cout << "2. Sort transactions\n";
            std::cout << "3. Search transactions\n";
            std::cout << "4. Generate JSON\n";
            std::cout << "Enter choice (1-4): ";
            std::cin >> op;
            if (std::cin.fail() || op < 1 || op > 4) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid operation. Please enter a number from 1 to 4.\n";
            } else {
                std::cin.ignore();
                break;
            }
        }

        bool valid_action = true;
        if (op == 1) {
            // Ask user for payment channel and count
            std::string channel;
            int num;
            // Validate channel input
            while (true) {
                std::cout << "Enter payment channel (card, ACH, UPI, wire_transfer): ";
                std::cin >> channel;
                if (channel == "card" || channel == "ACH" || channel == "UPI" || channel == "wire_transfer") {
                    break;
                } else {
                    std::cout << "Invalid channel. Please enter one of: card, ACH, UPI, wire_transfer.\n";
                }
            }
            // Validate number of transactions
            while (true) {
                std::cout << "How many transactions to display? ";
                std::cin >> num;
                if (std::cin.fail() || num <= 0) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Invalid number. Please enter a positive integer.\n";
                } else {
                    std::cin.ignore();
                    break;
                }
            }

            // Select the appropriate list
            Transaction* selected = nullptr;
            if (channel == "card") selected = card_head;
            else if (channel == "ACH") selected = ACH_head;
            else if (channel == "UPI") selected = UPI_head;
            else if (channel == "wire_transfer") selected = wire_transfer_head;

            // Print the requested number of transactions
            Transaction* curr = selected;
            int printed = 0;
            while (curr && printed < num) {
                std::cout << "ID: " << curr->transaction_id
                          << ", Amount: " << curr->amount
                          << ", Channel: " << curr->payment_channel
                          << ", Fraud: " << (curr->is_fraud ? "Yes" : "No")
                          << std::endl;
                curr = curr->next;
                ++printed;
            }
        } else if (op == 2) {
            // Use sort_transactions from sort.hpp
            std::cout << "Sort operation selected.\n";
            std::string channel;
            // Validate channel input
            while (true) {
                std::cout << "Enter payment channel to sort (card, ACH, UPI, wire_transfer): ";
                std::cin >> channel;
                if (channel == "card" || channel == "ACH" || channel == "UPI" || channel == "wire_transfer") {
                    break;
                } else {
                    std::cout << "Invalid channel. Please enter one of: card, ACH, UPI, wire_transfer.\n";
                }
            }
            Transaction** selected = nullptr;
            if (channel == "card") selected = &card_head;
            else if (channel == "ACH") selected = &ACH_head;
            else if (channel == "UPI") selected = &UPI_head;
            else if (channel == "wire_transfer") selected = &wire_transfer_head;
            sort_transactions(*selected);
        } else if (op == 3) {
            std::cout << "Search operation selected.\n";
            search_by_payment_channel_and_type();
        } else if (op == 4) {
            std::cout << "Generate JSON operation selected. (Not implemented)\n";
            // ...call generate json function here...
        } else {
            valid_action = false;
            std::cout << "Invalid operation.\n";
        }

        // Only call text checker if a valid action was performed
        if (valid_action) {
            bool to_menu = text_checker_menu();
            if (!to_menu) {
                // Free the linked lists before exit
                Transaction* lists[] = {card_head, ACH_head, UPI_head, wire_transfer_head};
                Transaction* curr;
                for (int i = 0; i < 4; ++i) {
                    curr = lists[i];
                    while (curr) {
                        Transaction* next = curr->next;
                        delete curr;
                        curr = next;
                    }
                }
                running = false;
            }
        }
        // else: loop back to main menu without calling text checker
    }

    return 0;
}
