#include "search.hpp"
#include "sort_before_search.hpp"
#include <iostream>
#include <algorithm>
using namespace std;


// Get middle node for binary search
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

// Get previous node (simulate for singly linked list)
Transaction* getPrevious(Transaction* head, Transaction* node) {
    if (!head || head == node) return nullptr;
    Transaction* prev = head;
    while (prev->next && prev->next != node) {
        prev = prev->next;
    }
    return (prev->next == node) ? prev : nullptr;
}

// Get value from node based on selected column
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

// ---------- Merge Sort Section ----------

// Split the linked list into two halves
void split(Transaction* source, Transaction** front, Transaction** back) {
    Transaction* slow = source;
    Transaction* fast = source->next;

    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = nullptr;
}

// Merge two sorted linked lists by selected field
Transaction* merge(Transaction* a, Transaction* b, int column_input) {
    if (!a) return b;
    if (!b) return a;

    string valA = to_lowercase(getFieldValue(a, column_input));
    string valB = to_lowercase(getFieldValue(b, column_input));

    bool cmp;
    if (column_input == 4)
        cmp = stod(valA) < stod(valB); // numeric comparison
    else
        cmp = valA < valB;

    if (cmp) {
        a->next = merge(a->next, b, column_input);
        return a;
    } else {
        b->next = merge(a, b->next, column_input);
        return b;
    }
}

// Recursive merge sort
Transaction* mergeSort(Transaction* head, int column_input) {
    if (!head || !head->next) return head;

    Transaction* a;
    Transaction* b;
    split(head, &a, &b);

    a = mergeSort(a, column_input);
    b = mergeSort(b, column_input);

    return merge(a, b, column_input);
}

// Wrapper to sort by column
Transaction* sortByColumn(Transaction* head, int column_input) {
    return mergeSort(head, column_input);
}

// ---------- Binary Search with Range Expansion ----------

Transaction* searchByType2(Transaction* head, const string& type, int column_input) {
    string target = to_lowercase(type);
    bool found = false;

    // Sort the linked list first
    head = sortByColumn(head, column_input);

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

            // Expand left
            Transaction* left = mid;
            while (left) {
                string val = to_lowercase(getFieldValue(left, column_input));
                if (val != target) break;
                append_transaction(result_head, result_tail, left);
                if (left == head) break;
                left = getPrevious(head, left);
            }

            // Expand right
            Transaction* right = mid->next;
            while (right) {
                string val = to_lowercase(getFieldValue(right, column_input));
                if (val != target) break;
                append_transaction(result_head, result_tail, right);
                right = right->next;
            }

            break;
        }
        else if (midVal < target) {
            start = mid->next;
        } else {
            end = mid;
        }
    }

    if (!found) {
        cout << "No transaction found for value: " << type << endl;
    } else {
        cout << "--- Binary Search Results ---\n";
        Transaction* temp = result_head;
        while (temp) {
            cout << "ID: " << temp->transaction_id
                 << ", Amount: $" << temp->amount
                 << ", Location: " << temp->location
                 << ", Type: " << temp->transaction_type << endl;
            temp = temp->next;
        }
    }

    return result_head;
}
