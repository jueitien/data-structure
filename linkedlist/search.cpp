#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include "search.hpp"
#include "store.hpp"
#include "sort_before_search.hpp"
#include "generate.hpp"
#include <limits>




using namespace std;
using namespace std::chrono;

// Helper: convert string to lowercase
string to_lowercase(const string &str)
{
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

void append_transaction(Transaction*& head, Transaction*& tail, Transaction* node) {
    Transaction* new_node = new Transaction(*node); // Copy data
    new_node->next = nullptr;
    if (!head) {
        head = tail = new_node;
    } else {
        tail->next = new_node;
        tail = new_node;
    }
}

// Search for a specific transaction type in a linked list
Transaction* searchByType(Transaction* head, const string& type, int column_input) {
    Transaction* result_head = nullptr;
    Transaction* result_tail = nullptr;

    Transaction* temp = head;
    while (temp != nullptr) {
        string field_value;
        switch (column_input) {
            case 1: field_value = temp->transaction_id; break;
            case 2: field_value = temp->sender_account; break;
            case 3: field_value = temp->receiver_account; break;
            case 4: field_value = to_string(temp->amount); break;
            case 5: field_value = temp->transaction_type; break;
            case 6: field_value = temp->location; break;
            case 7: field_value = temp->fraud_type; break;
            default: return nullptr;
        }

        if (to_lowercase(field_value) == to_lowercase(type)) {
            append_transaction(result_head, result_tail, temp);
        }
        temp = temp->next;
    }

    if (!result_head) {
        cout << "No transactions found for the given search.\n";
    }

    return result_head;
}

void Json_generate_confirmation(Transaction* result_head, const string& filename) {
    string response;
    cout << "Do you want to generate a JSON file? (yes/no): ";
    cin >> response;
    response = to_lowercase(response);
    if (response == "yes") {
        cout << "Generating JSON file...\n";
        generate_json_from_linkedlist(result_head, filename);
    } else {
        cout << "JSON generation skipped.\n";
    }
}

// This function integrates channel selection + search
void search_by_payment_channel_and_type() {
    string channel;
    Transaction* selected = nullptr;

    while (true) {
        cout << "Enter payment channel (card, ACH, UPI, wire_transfer): ";
        cin >> channel;
        channel = to_lowercase(channel);

        if (channel == "card") selected = card_head;
        else if (channel == "ach") selected = ACH_head;
        else if (channel == "upi") selected = UPI_head;
        else if (channel == "wire_transfer") selected = wire_transfer_head;
        else {
            cout << "Invalid payment channel.\n";
            continue;
        }
        break;
    }

    int column_input;
    string type, column_name;
    int method;

    // Get column input
    while (true) {
        cout << "Which column to search? (Transaction ID[1]... Fraud Type[7]): ";
        cin >> column_input;
        if (cin.fail() || column_input < 1 || column_input > 7) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 7.\n";
            continue;
        }
        break;
    }

    cout << "Enter the value to search: ";
    cin >> type;

    cout << "Methods to search (linear search[1], Sort before search[2]): ";
    cin >> method;

    Transaction* result = nullptr;
    if (method == 1) {
        auto start = high_resolution_clock::now();
        cout << "\n--- Linear Search Results ---\n";
        result = searchByType(selected, type, column_input);
        auto end = high_resolution_clock::now();
        cout << "Search took " << duration_cast<microseconds>(end - start).count() << " microseconds.\n";
    } else if (method == 2) {
        auto start = high_resolution_clock::now();
        cout << "\n--- Binary Search Results ---\n";
        result = searchByType2(selected, type, column_input);  
        auto end = high_resolution_clock::now();
        cout << "Search took " << duration_cast<microseconds>(end - start).count() << " microseconds.\n";
    }

    if (result != nullptr) {
        string filename = channel + "_search_results.json";
        Json_generate_confirmation(result, filename);
    }
}
