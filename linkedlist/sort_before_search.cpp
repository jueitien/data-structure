#include "search.hpp"
#include "sort_before_search.hpp"
#include <iostream>
#include <algorithm>
using namespace std;




// Get middle node between start and end
Transaction* getMiddle(Transaction* start, Transaction* end) {
    if (!start) return nullptr;
    Transaction* slow = start;
    Transaction* fast = start;

    while (fast != end && fast->next != end) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
}

// Get value by column number
string getFieldValue(Transaction* node, int column_input) {
    switch (column_input) {
        case 1: return node->transaction_id;
        case 2: return node->sender_account;
        case 3: return node->receiver_account;
        case 4: return to_string(node->amount);
        case 5: return node->transaction_type;
        case 6: return node->location;
        case 7: return node->fraud_type;
        default: return "";
    }
}

// Main function
Transaction* searchByType2(Transaction* head, const string& type, int column_input) {
    string target = to_lowercase(type);
    bool found = false;

    Transaction* result_head = nullptr;
    Transaction* result_tail = nullptr;

    // Only binary search for transaction_id
    if (column_input == 1) {
        Transaction* start = head;
        Transaction* end = nullptr;

        while (start != end) {
            Transaction* mid = getMiddle(start, end);
            if (!mid) break;

            string midVal = to_lowercase(mid->transaction_id);

            if (midVal == target) {
                found = true;
                append_transaction(result_head, result_tail, mid); // same append helper as used in searchByType
                cout << "ID: " << mid->transaction_id
                     << ", Amount: $" << mid->amount
                     << ", Location: " << mid->location
                     << ", Type: " << mid->transaction_type << endl;
                break; // only one match for transaction_id expected in sorted binary search
            } else if (midVal < target) {
                start = mid->next;
            } else {
                end = mid;
            }
        }
    } else {
        // Fallback to linear search for other columns
        cout << "Note: Only 'transaction_id' is sorted. Performing linear search for column " << column_input << ".\n";
        Transaction* temp = head;
        while (temp != nullptr) {
            string field_value = to_lowercase(getFieldValue(temp, column_input));
            if (field_value == target) {
                found = true;
                append_transaction(result_head, result_tail, temp);
                cout << "ID: " << temp->transaction_id
                     << ", Amount: $" << temp->amount
                     << ", Location: " << temp->location
                     << ", Type: " << temp->transaction_type << endl;
            }
            temp = temp->next;
        }
    }

    if (!found) {
        cout << "No transaction found for value: " << type << endl;
    }

    return result_head;
}