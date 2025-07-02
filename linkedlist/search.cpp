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
void searchByType(Transaction* head, const string& type) {
    Transaction* temp = head;
    bool found = false;

    while (temp != nullptr) {
        if (to_lowercase(temp->transaction_type) == to_lowercase(type)) {
            found = true;
            cout << "ID: " << temp->transaction_id
                 << ", Amount: $" << temp->amount
                 << ", Location: " << temp->location
                 << ", Type: " << temp->transaction_type << endl;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "No transactions of type '" << type << "' found.\n";
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
    cout << "Enter transaction type to search (e.g., withdrawal, deposit, transfer): ";
    cin >> type;

    cout << "\n--- Search Results for type '" << type << "' in channel '" << channel << "' ---\n";
    searchByType(selected, type);
}
