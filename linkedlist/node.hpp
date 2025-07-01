#pragma once
#include <string>
#include <iostream>
using std::string;

struct Transaction {
    string transaction_id;
    string timestamp;
    string sender_account;
    string receiver_account;
    double amount;
    string transaction_type;
    string merchant_category;
    string location;
    string device_used;
    bool is_fraud;
    string fraud_type;
    double time_since_last_transaction;
    double spending_deviation_score;
    int velocity_score;
    double geo_anomaly_score;
    string payment_channel;
    string ip_address;
    string device_hash;
    Transaction* next = nullptr;
};

inline std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    os << "Transaction ID: " << t.transaction_id
       << ", Timestamp: " << t.timestamp
       << ", Sender: " << t.sender_account
       << ", Receiver: " << t.receiver_account
       << ", Amount: " << t.amount
       << ", Type: " << t.transaction_type
       << ", Fraud: " << (t.is_fraud ? "Yes" : "No");
    return os;
}