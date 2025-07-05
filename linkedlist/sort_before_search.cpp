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

    Transaction* start = head;
    Transaction* end = nullptr;

    while (start != end) {
        Transaction* mid = getMiddle(start, end);
        if (!mid) break;

        string midVal = to_lowercase(getFieldValue(mid, column_input));

        if (midVal == target) {
            found = true;
            append_transaction(result_head, result_tail, mid);
            cout << "ID: " << mid->transaction_id
                 << ", Amount: $" << mid->amount
                 << ", Location: " << mid->location
                 << ", Type: " << mid->transaction_type << endl;
            break;  // If values are unique, we can break here
        } else if (midVal < target) {
            start = mid->next;
        } else {
            end = mid;
        }
    }

    if (!found) {
        cout << "No transaction found for value: " << type << endl;
    }

    return result_head;
}
