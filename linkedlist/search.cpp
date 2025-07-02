#include <iostream>
#include <string>
#include <algorithm>
#include "search.hpp"
#include "store.hpp"
#include "sort_before_search.hpp"

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
            case 1:field_value = temp->transaction_id;break;
            case 2:field_value = temp->sender_account;break;
            case 3:field_value = temp->receiver_account;break;
            case 4:field_value = std::to_string(temp->amount);break;
            case 5:field_value = temp->transaction_type;break;
            case 6:field_value = temp->location;break;
            case 7:field_value = temp->fraud_type;break;
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
    string column_name;
    int method;

    cout << "Which column you want to search? (Transaction ID[1], Sender Account[2], Receiver Account[3], Amount[4], Transaction Type[5], Location[6], Fraud Type[7]): ";
    cin >> column_input;

    switch (column_input) {
        case 1: column_name = "transaction_id"; break;
        case 2: column_name = "sender_account"; break;
        case 3: column_name = "receiver_account"; break;
        case 4: column_name = "amount"; break;
        case 5: column_name = "transaction_type"; break;
        case 6: column_name = "location"; break;
        case 7: column_name = "fraud_type"; break;
        default: cout << "Invalid input. Please insert (1/2/3/4/5/6/7)\n"; return;
    }
    
    

    cout << "Enter the value to search: ";
    cin >> type;

    cout << "Methods to search (linear search[1], Sort before search[2]): ";
    cin >> method;

    if(method == 1){
        cout << "\n--- Search Results for '" << column_name << "' in channel '" << channel << "' with the value of '" << type << "'---\n";
        searchByType(selected, type, column_input);
        return;
    }else if(method == 2){
        cout << "\n--- Search Results for '" << column_name << "' in channel '" << channel << "' with the value of '" << type << "'---\n";
        searchByType2(selected, type, column_input);
        return;
    }

    

    



}
