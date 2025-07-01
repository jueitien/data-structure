#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Store.hpp"
#include "Data_structure.hpp" //Transaction
using namespace std;

#define MAXCHAR 1000
#define MAX_FILTERED_ROWS 5000000



Transaction* filtered_transactions = new Transaction[MAX_FILTERED_ROWS]; 

bool parseTransaction(const string& line, Transaction &tx) {
    stringstream ss(line);
    string token;
    int field = 0;

    while (getline(ss, token, ',')) {
        string value = token;

        switch (field) {
            case 0: tx.transaction_id = value; break;
            case 1: tx.timestamp = value; break;
            case 2: tx.sender_account = value; break;
            case 3: tx.receiver_account = value; break;
            case 4: tx.amount = token.empty() ? 0.0 : atof(token.c_str()); break;
            case 5: tx.transaction_type = value; break;
            case 6: tx.merchant_category = value; break;
            case 7: tx.location = value; break;
            case 8: tx.device_used = value; break;
            case 9: tx.is_fraud = value; break;
            case 10: tx.fraud_type = token.empty() ? "N/A" : token; break;
            case 11: tx.time_since_last_transaction = token.empty() ? 0.0 : atof(token.c_str()); break;
            case 12: tx.spending_deviation_score = token.empty() ? 0.0 : atof(token.c_str()); break;
            case 13: tx.velocity_score = token.empty() ? 0 : atoi(token.c_str()); break;
            case 14: tx.geo_anomaly_score = token.empty() ? 0.0 : atof(token.c_str()); break;
            case 15: tx.payment_channel = value; break;
            case 16: tx.ip_address = value; break;
            case 17: tx.device_hash = value; break;
        }

        field++;
    }

    return (field == 18);
}

int transactionCount = 0;
void storeSeperate() {
    FILE *fp;
    char row[MAXCHAR];
    char input[50];
    transactionCount = 0;
    cout << "Enter payment channel to filter (card, wire_transfer, ACH, UPI): ";
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;  // Remove newline

    fp = fopen("C:/Users/GF63/Downloads/financial_fraud_detection_dataset.csv", "r");
    if (!fp) {
        cout << "Error opening file.\n";
        return;
    }

    // Skip header
    fgets(row, MAXCHAR, fp);

    while (fgets(row, MAXCHAR, fp) != NULL && transactionCount < MAX_FILTERED_ROWS) {
        if (strstr(row, input) != NULL) {
            char row_copy[MAXCHAR];
            strcpy(row_copy, row); // Make a copy since strtok modifies it
            if (parseTransaction(row_copy, filtered_transactions[transactionCount])) {
                transactionCount++;
            }
        }
    }

    fclose(fp);

    cout << "Filtered and stored " << transactionCount << " rows for channel \"" << input << "\".\n";
}

void storeAll(){

    FILE *fp;
    char row[MAXCHAR];
    transactionCount = 0;

    fp = fopen("C:/Users/GF63/Downloads/financial_fraud_detection_dataset.csv", "r");
    if (!fp) {
        cout << "Error opening file.\n";
        return;
    }

    // Skip header
    fgets(row, MAXCHAR, fp);

    while (fgets(row, MAXCHAR, fp) != NULL && transactionCount < MAX_FILTERED_ROWS) {
        char row_copy[MAXCHAR];
        strcpy(row_copy, row); // Make a copy since stringstream doesn't work on C strings directly
        if (parseTransaction(string(row_copy), filtered_transactions[transactionCount])) {
            transactionCount++;
        }

        if (transactionCount % 1000000 == 0) {
            cout << "Parsed " << transactionCount << " transactions...\n";
        }
    }

    fclose(fp);

    cout << "Finished storing " << transactionCount << " transactions.\n";
}