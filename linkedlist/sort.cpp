#include "sort.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>

// Helper function to compare locations based on order
static bool compare_location(const std::string& a, const std::string& b, SortOrder order) {
    if (order == SortOrder::Ascending)
        return a < b;
    else
        return a > b;
}

// Merge two sorted lists by location
static Transaction* merge_sorted_lists(Transaction* l1, Transaction* l2, SortOrder order) {
    Transaction dummy;
    Transaction* tail = &dummy;
    while (l1 && l2) {
        if (compare_location(l1->location, l2->location, order)) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}

// Helper function to trim and lowercase a string
static std::string to_lower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c){ return std::tolower(c); });
    return out;
}

// Generic compare function for all fields
static bool compare_field(const Transaction* a, const Transaction* b, const std::string& field, SortOrder order) {
    // Compare based on field name and type
    if (field == "transaction_id") {
        return (order == SortOrder::Ascending) ? (a->transaction_id < b->transaction_id) : (a->transaction_id > b->transaction_id);
    } else if (field == "timestamp") {
        return (order == SortOrder::Ascending) ? (a->timestamp < b->timestamp) : (a->timestamp > b->timestamp);
    } else if (field == "sender_account") {
        return (order == SortOrder::Ascending) ? (a->sender_account < b->sender_account) : (a->sender_account > b->sender_account);
    } else if (field == "receiver_account") {
        return (order == SortOrder::Ascending) ? (a->receiver_account < b->receiver_account) : (a->receiver_account > b->receiver_account);
    } else if (field == "amount") {
        return (order == SortOrder::Ascending) ? (a->amount < b->amount) : (a->amount > b->amount);
    } else if (field == "transaction_type") {
        return (order == SortOrder::Ascending) ? (a->transaction_type < b->transaction_type) : (a->transaction_type > b->transaction_type);
    } else if (field == "merchant_category") {
        return (order == SortOrder::Ascending) ? (a->merchant_category < b->merchant_category) : (a->merchant_category > b->merchant_category);
    } else if (field == "location") {
        return (order == SortOrder::Ascending) ? (a->location < b->location) : (a->location > b->location);
    } else if (field == "device_used") {
        return (order == SortOrder::Ascending) ? (a->device_used < b->device_used) : (a->device_used > b->device_used);
    } else if (field == "is_fraud") {
        return (order == SortOrder::Ascending) ? (!a->is_fraud && b->is_fraud) : (a->is_fraud && !b->is_fraud);
    } else if (field == "fraud_type") {
        return (order == SortOrder::Ascending) ? (a->fraud_type < b->fraud_type) : (a->fraud_type > b->fraud_type);
    } else if (field == "time_since_last_transaction") {
        return (order == SortOrder::Ascending) ? (a->time_since_last_transaction < b->time_since_last_transaction) : (a->time_since_last_transaction > b->time_since_last_transaction);
    } else if (field == "spending_deviation_score") {
        return (order == SortOrder::Ascending) ? (a->spending_deviation_score < b->spending_deviation_score) : (a->spending_deviation_score > b->spending_deviation_score);
    } else if (field == "velocity_score") {
        return (order == SortOrder::Ascending) ? (a->velocity_score < b->velocity_score) : (a->velocity_score > b->velocity_score);
    } else if (field == "geo_anomaly_score") {
        return (order == SortOrder::Ascending) ? (a->geo_anomaly_score < b->geo_anomaly_score) : (a->geo_anomaly_score > b->geo_anomaly_score);
    } else if (field == "payment_channel") {
        return (order == SortOrder::Ascending) ? (a->payment_channel < b->payment_channel) : (a->payment_channel > b->payment_channel);
    } else if (field == "ip_address") {
        return (order == SortOrder::Ascending) ? (a->ip_address < b->ip_address) : (a->ip_address > b->ip_address);
    } else if (field == "device_hash") {
        return (order == SortOrder::Ascending) ? (a->device_hash < b->device_hash) : (a->device_hash > b->device_hash);
    }
    // Default to transaction_id if field not found
    return (order == SortOrder::Ascending) ? (a->transaction_id < b->transaction_id) : (a->transaction_id > b->transaction_id);
}

// Merge two sorted lists by field
static Transaction* merge_sorted_lists_field(Transaction* l1, Transaction* l2, const std::string& field, SortOrder order) {
    Transaction dummy;
    Transaction* tail = &dummy;
    while (l1 && l2) {
        if (compare_field(l1, l2, field, order)) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}

// Split list into two halves
static void split_list(Transaction* head, Transaction** first, Transaction** second) {
    Transaction* slow = head;
    Transaction* fast = head ? head->next : nullptr;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    *first = head;
    *second = slow ? slow->next : nullptr;
    if (slow) slow->next = nullptr;
}

// Merge sort by location
Transaction* merge_sort_by_location(Transaction* head, SortOrder order) {
    if (!head || !head->next) return head;
    Transaction* first = nullptr;
    Transaction* second = nullptr;
    split_list(head, &first, &second);
    first = merge_sort_by_location(first, order);
    second = merge_sort_by_location(second, order);
    return merge_sorted_lists(first, second, order);
}

// Insertion sort by location
Transaction* insertion_sort_by_location(Transaction* head, SortOrder order) {
    Transaction dummy;
    dummy.next = nullptr;
    Transaction* curr = head;
    while (curr) {
        Transaction* next = curr->next;
        Transaction* prev = &dummy;
        while (prev->next && compare_location(prev->next->location, curr->location, order))
            prev = prev->next;
        curr->next = prev->next;
        prev->next = curr;
        curr = next;
    }
    return dummy.next;
}

// Merge sort by field
Transaction* merge_sort_by_field(Transaction* head, const std::string& field, SortOrder order) {
    if (!head || !head->next) return head;
    Transaction* first = nullptr;
    Transaction* second = nullptr;
    split_list(head, &first, &second);
    first = merge_sort_by_field(first, field, order);
    second = merge_sort_by_field(second, field, order);
    return merge_sorted_lists_field(first, second, field, order);
}

// Insertion sort by field
Transaction* insertion_sort_by_field(Transaction* head, const std::string& field, SortOrder order) {
    Transaction dummy;
    dummy.next = nullptr;
    Transaction* curr = head;
    while (curr) {
        Transaction* next = curr->next;
        Transaction* prev = &dummy;
        while (prev->next && compare_field(prev->next, curr, field, order))
            prev = prev->next;
        curr->next = prev->next;
        prev->next = curr;
        curr = next;
    }
    return dummy.next;
}

// User prompt to choose sorting algorithm, field, and order
void sort_transactions(Transaction*& head) {
    int algo_choice = 0, order_choice = 0;
    std::string field;
    std::cout << "Choose sorting algorithm:\n1. Merge Sort\n2. Insertion Sort\nEnter choice: ";
    std::cin >> algo_choice;
    std::cin.ignore();
    std::cout << "Enter column name to sort by (e.g., amount, timestamp, location): ";
    std::getline(std::cin, field);
    field = to_lower(field);
    std::cout << "Sort in:\n1. Ascending order\n2. Descending order\nEnter choice: ";
    std::cin >> order_choice;
    SortOrder order = (order_choice == 2) ? SortOrder::Descending : SortOrder::Ascending;

    // Count items before sorting
    int count = 0;
    Transaction* tmp = head;
    while (tmp) {
        ++count;
        tmp = tmp->next;
    }

    // Time the sorting
    auto start = std::chrono::high_resolution_clock::now();
    if (algo_choice == 2)
        head = insertion_sort_by_field(head, field, order);
    else
        head = merge_sort_by_field(head, field, order);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    // Log to file
    std::ofstream log("sort_log.txt", std::ios::app);
    log << "Algorithm: " << (algo_choice == 2 ? "Insertion Sort" : "Merge Sort")
        << ", Field: " << field
        << ", Order: " << (order == SortOrder::Descending ? "Descending" : "Ascending")
        << ", Items: " << count
        << ", Time(ms): " << elapsed.count()
        << std::endl;
    log.close();

    std::cout << "Sorted " << count << " items in " << elapsed.count() << " ms. (Logged to sort_log.txt)\n";
}
