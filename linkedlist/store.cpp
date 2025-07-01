#include "node.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

// Helper to trim whitespace
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// Helper to clean a string field
std::string clean_string(const std::string& s) {
    std::string val = trim(s);
    if (val == "NA" || val == "N/A" || val.empty()) return "";
    return val;
}

// Helper to clean a double field
double clean_double(const std::string& s) {
    std::string val = trim(s);
    if (val == "NA" || val == "N/A" || val.empty()) return 0.0;
    try { return std::stod(val); }
    catch (...) { return 0.0; }
}

// Helper to clean an int field
int clean_int(const std::string& s) {
    std::string val = trim(s);
    if (val == "NA" || val == "N/A" || val.empty()) return 0;
    try { return std::stoi(val); }
    catch (...) { return 0; }
}

// Helper to clean a bool field
bool clean_bool(const std::string& s) {
    std::string val = trim(s);
    if (val == "NA" || val == "N/A" || val.empty()) return false;
    if (val == "1" || val == "true" || val == "True") return true;
    return false;
}

// Reads the CSV and returns head of linked list
Transaction* load_transactions_from_csv(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return nullptr;
    }

    std::string line;
    // Skip header
    std::getline(file, line);

    Transaction* head = nullptr;
    Transaction* tail = nullptr;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        Transaction* node = new Transaction();

        // Read and clean each field
        std::getline(ss, field, ','); node->transaction_id = clean_string(field);
        std::getline(ss, field, ','); node->timestamp = clean_string(field);
        std::getline(ss, field, ','); node->sender_account = clean_string(field);
        std::getline(ss, field, ','); node->receiver_account = clean_string(field);
        std::getline(ss, field, ','); node->amount = clean_double(field);
        std::getline(ss, field, ','); node->transaction_type = clean_string(field);
        std::getline(ss, field, ','); node->merchant_category = clean_string(field);
        std::getline(ss, field, ','); node->location = clean_string(field);
        std::getline(ss, field, ','); node->device_used = clean_string(field);
        std::getline(ss, field, ','); node->is_fraud = clean_bool(field);
        std::getline(ss, field, ','); node->fraud_type = clean_string(field);
        std::getline(ss, field, ','); node->time_since_last_transaction = clean_double(field);
        std::getline(ss, field, ','); node->spending_deviation_score = clean_double(field);
        std::getline(ss, field, ','); node->velocity_score = clean_int(field);
        std::getline(ss, field, ','); node->geo_anomaly_score = clean_double(field);
        std::getline(ss, field, ','); node->payment_channel = clean_string(field);
        std::getline(ss, field, ','); node->ip_address = clean_string(field);
        std::getline(ss, field, ','); node->device_hash = clean_string(field);

        node->next = nullptr;

        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    file.close();
    return head;
}

// Global linked list heads for each transaction type
Transaction* withdrawal_head = nullptr;
Transaction* deposit_head = nullptr;
Transaction* transfer_head = nullptr;
Transaction* payment_head = nullptr;

// Function to split the linked list into four global lists by transaction_type
void split_by_transaction_type(Transaction* head) {
    Transaction *withdrawal_tail = nullptr, *deposit_tail = nullptr, *transfer_tail = nullptr, *payment_tail = nullptr;
    for (auto curr = head; curr != nullptr; ) {
        Transaction* next = curr->next;
        curr->next = nullptr;
        if (curr->transaction_type == "withdrawal") {
            if (!withdrawal_head) {
                withdrawal_head = curr;
                withdrawal_tail = curr;
            } else {
                withdrawal_tail->next = curr;
                withdrawal_tail = curr;
            }
        } else if (curr->transaction_type == "deposit") {
            if (!deposit_head) {
                deposit_head = curr;
                deposit_tail = curr;
            } else {
                deposit_tail->next = curr;
                deposit_tail = curr;
            }
        } else if (curr->transaction_type == "transfer") {
            if (!transfer_head) {
                transfer_head = curr;
                transfer_tail = curr;
            } else {
                transfer_tail->next = curr;
                transfer_tail = curr;
            }
        } else if (curr->transaction_type == "payment") {
            if (!payment_head) {
                payment_head = curr;
                payment_tail = curr;
            } else {
                payment_tail->next = curr;
                payment_tail = curr;
            }
        }
        curr = next;
    }
}