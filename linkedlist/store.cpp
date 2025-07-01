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

// Global linked list heads for each payment channel
Transaction* card_head = nullptr;
Transaction* ACH_head = nullptr;
Transaction* UPI_head = nullptr;
Transaction* wire_transfer_head = nullptr;

// Function to split the linked list into four global lists by payment_channel
void split_by_payment_channel(Transaction* head) {
    Transaction *card_tail = nullptr, *ACH_tail = nullptr, *UPI_tail = nullptr, *wire_transfer_tail = nullptr;
    for (auto curr = head; curr != nullptr; ) {
        Transaction* next = curr->next;
        curr->next = nullptr;
        if (curr->payment_channel == "card") {
            if (!card_head) {
                card_head = curr;
                card_tail = curr;
            } else {
                card_tail->next = curr;
                card_tail = curr;
            }
        } else if (curr->payment_channel == "ACH") {
            if (!ACH_head) {
                ACH_head = curr;
                ACH_tail = curr;
            } else {
                ACH_tail->next = curr;
                ACH_tail = curr;
            }
        } else if (curr->payment_channel == "UPI") {
            if (!UPI_head) {
                UPI_head = curr;
                UPI_tail = curr;
            } else {
                UPI_tail->next = curr;
                UPI_tail = curr;
            }
        } else if (curr->payment_channel == "wire_transfer") {
            if (!wire_transfer_head) {
                wire_transfer_head = curr;
                wire_transfer_tail = curr;
            } else {
                wire_transfer_tail->next = curr;
                wire_transfer_tail = curr;
            }
        }
        // If payment_channel is not one of the above, ignore or handle as needed
        curr = next;
    }
}