#include "generate.hpp"
#include <fstream>
#include "../lib/json.hpp"


// Generate JSON file from linked list
void generate_json_from_linkedlist(Transaction* head, const std::string& filename) {
    using nlohmann::json;
    json jarray = json::array();
    Transaction* curr = head;
    while (curr) {
        json j;
        j["transaction_id"] = curr->transaction_id;
        j["timestamp"] = curr->timestamp;
        j["sender_account"] = curr->sender_account;
        j["receiver_account"] = curr->receiver_account;
        j["amount"] = curr->amount;
        j["transaction_type"] = curr->transaction_type;
        j["merchant_category"] = curr->merchant_category;
        j["location"] = curr->location;
        j["device_used"] = curr->device_used;
        j["is_fraud"] = curr->is_fraud;
        j["fraud_type"] = curr->fraud_type;
        j["time_since_last_transaction"] = curr->time_since_last_transaction;
        j["spending_deviation_score"] = curr->spending_deviation_score;
        j["velocity_score"] = curr->velocity_score;
        j["geo_anomaly_score"] = curr->geo_anomaly_score;
        j["payment_channel"] = curr->payment_channel;
        j["ip_address"] = curr->ip_address;
        j["device_hash"] = curr->device_hash;
        jarray.push_back(j);
        curr = curr->next;
    }
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    out << jarray.dump(4) << std::endl; // Pretty print with indent=4
    out.close();
}