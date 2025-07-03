#include "sort.hpp"
#include "search.hpp"
#include "generate.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>


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

    // Validate sorting algorithm choice
    while (true) {
        std::cout << "Choose sorting algorithm:\n1. Merge Sort\n2. Insertion Sort\nEnter choice: ";
        std::cin >> algo_choice;
        if (std::cin.fail() || (algo_choice != 1 && algo_choice != 2)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid choice. Please enter 1 or 2.\n";
        } else {
            std::cin.ignore();
            break;
        }
    }

    // Validate field name (manual string checks)
    while (true) {
        std::cout << "Enter column name to sort by (e.g., amount, timestamp, location): ";
        std::getline(std::cin, field);
        field = to_lower(field);
        if (
            field == "transaction_id" ||
            field == "timestamp" ||
            field == "sender_account" ||
            field == "receiver_account" ||
            field == "amount" ||
            field == "transaction_type" ||
            field == "merchant_category" ||
            field == "location" ||
            field == "device_used" ||
            field == "is_fraud" ||
            field == "fraud_type" ||
            field == "time_since_last_transaction" ||
            field == "spending_deviation_score" ||
            field == "velocity_score" ||
            field == "geo_anomaly_score" ||
            field == "payment_channel" ||
            field == "ip_address" ||
            field == "device_hash"
        ) {
            break;
        } else {
            std::cout << "Invalid field name. Please enter a valid field.\n";
        }
    }

    // Validate sort order
    while (true) {
        std::cout << "Sort in:\n1. Ascending order\n2. Descending order\nEnter choice: ";
        std::cin >> order_choice;
        if (std::cin.fail() || (order_choice != 1 && order_choice != 2)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid choice. Please enter 1 or 2.\n";
        } else {
            break;
        }
    }
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

    // Write sorted values to file
    tmp = head;
    
    std::ofstream out(algo_choice == 2 ? "sort_insertion.txt" : "sort_merge.txt");
    while (tmp) {
        out << tmp->transaction_id << ','
            << tmp->timestamp << ','
            << tmp->sender_account << ','
            << tmp->receiver_account << ','
            << tmp->amount << ','
            << tmp->transaction_type << ','
            << tmp->merchant_category << ','
            << tmp->location << ','
            << tmp->device_used << ','
            << tmp->is_fraud << ','
            << tmp->fraud_type << ','
            << tmp->time_since_last_transaction << ','
            << tmp->spending_deviation_score << ','
            << tmp->velocity_score << ','
            << tmp->geo_anomaly_score << ','
            << tmp->payment_channel << ','
            << tmp->ip_address << ','
            << tmp->device_hash << '\n';
        tmp = tmp->next;
    }
    out.close();

    std::cout << "Sorted " << count << " items in " << elapsed.count() << " ms. (Logged to sort_log.txt)\n";
    std::cout << "Sorted data written to " << (algo_choice == 2 ? "sort_insertion.txt" : "sort_merge.txt") << std::endl;

    // tmp = head;
    // Transaction* result_head = nullptr;
    // Transaction* result_tail = nullptr;
    // while (tmp != nullptr) {
    //     append_transaction(result_head, result_tail, tmp);
    //     tmp = tmp->next;
    // }
    // std::cout << "Generating JSON file...\n";
    // if(algo_choice == 2){
    //     generate_json_from_linkedlist(result_head, "sort_insertion_result.json");
    // } else {
    //     generate_json_from_linkedlist(result_head, "sort_merge_result.json");
    // }

}
