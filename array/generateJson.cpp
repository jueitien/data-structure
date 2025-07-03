#include "../lib/json.hpp"
#include "Store.hpp"
#include "Data_structure.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>


void generateJsonReport(Transaction transactions[],int size) {
    nlohmann::json jsonReport;

    
    // Populate the JSON object with transaction data
    for (int i = 0; i < size; ++i) {
        nlohmann::json transactionJson;
        transactionJson["id"] = transactions[i].transaction_id;
        transactionJson["amount"] = transactions[i].amount;
        transactionJson["timestamp"] = transactions[i].timestamp;
        transactionJson["sender_account"] = transactions[i].sender_account;
        transactionJson["receiver_account"] = transactions[i].receiver_account;
        transactionJson["transaction_type"] = transactions[i].transaction_type;
        transactionJson["merchant_category"] = transactions[i].merchant_category;
        transactionJson["location"] = transactions[i].location;
        transactionJson["device_used"] = transactions[i].device_used;
        transactionJson["is_fraud"] = transactions[i].is_fraud;
        transactionJson["fraud_type"] = transactions[i].fraud_type;
        transactionJson["time_since_last_transaction"] = transactions[i].time_since_last_transaction;
        transactionJson["spending_deviation_score"] = transactions[i].spending_deviation_score;
        transactionJson["velocity_score"] = transactions[i].velocity_score;
        transactionJson["geo_anomaly_score"] = transactions[i].geo_anomaly_score;
        transactionJson["payment_channel"] = transactions[i].payment_channel;
        transactionJson["ip_address"] = transactions[i].ip_address;
        transactionJson["device_hash"] = transactions[i].device_hash;

        jsonReport.push_back(transactionJson); // Add the transaction to the report
    }
 
    // Optionally, write JSON to a file
    std::ofstream outputFile("transactions_report.json");
    if (outputFile.is_open()) {
        outputFile << jsonReport.dump(4); // Write to file with pretty print
        outputFile.close();
        std::cout << "JSON report generated: transactions_report.json" << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
}
