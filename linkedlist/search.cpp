#include <iostream>
#include <string>
#include <algorithm>
#include "search.hpp"
#include "store.hpp"

using namespace std;

// Helper: convert string to lowercase
string to_lowercase(const string& str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// Search for a specific transaction type in a linked list
void searchByType(Transaction* head, const string& type, int column_input) {
    Transaction* temp = head;
    bool found = false;

    while (temp != nullptr) {
        string field_value;

        switch (column_input) {
            case 1:
                field_value = temp->transaction_id;
                break;
            case 2:
                field_value = temp->sender_account;
                break;
            case 3:
                field_value = temp->receiver_account;
                break;
            case 4:
                field_value = std::to_string(temp->amount);  // convert double to string
                break;
            case 5:
                field_value = temp->transaction_type;
                break;
            case 6:
                field_value = temp->location;
                break;
            case 7:
                field_value = temp->fraud_type;
                break;
            default:
                std::cout << "Invalid column input.\n";
                return;
        }

        // Convert both to lowercase for case-insensitive match
        if (to_lowercase(field_value) == to_lowercase(type)) {
            found = true;
            cout << "ID: " << temp->transaction_id
                 << ", Amount: $" << temp->amount
                 << ", Location: " << temp->location
                 << ", Type: " << temp->transaction_type << endl;
        }

        temp = temp->next;
    }

    if (!found) {
        cout << "No transactions found for the given search.\n";
    }
}

// This function integrates channel selection + search
void search_by_payment_channel_and_type() {
    string channel;
    cout << "Enter payment channel (card, ACH, UPI, wire_transfer): ";
    cin >> channel;

    channel = to_lowercase(channel);
    Transaction* selected = nullptr;

    if (channel == "card") {
        selected = card_head;
    } else if (channel == "ach") {
        selected = ACH_head;
    } else if (channel == "upi") {
        selected = UPI_head;
    } else if (channel == "wire_transfer") {
        selected = wire_transfer_head;
    } else {
        cout << "Invalid payment channel.\n";
        return;
    }

    string type;
    int column_input;

    cout << "Which column you want to search? (Transaction ID[1], Sender Account[2], Receiver Account[3], Amount[4], Transaction Type[5], Location[6], Fraud Type[7]): ";
    cin >> column_input;
    
    

    cout << "Enter the value to search (Transaction Type: withdrawal, deposit, transfer): ";
    cin >> type;

    

    cout << "\n--- Search Results for type '" << type << "' in channel '" << channel << "' ---\n";
    searchByType(selected, type, column_input);
    return;



}
