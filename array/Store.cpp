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
        switch (field) {
            case 0: tx.transaction_id = token; break;
            case 1: tx.timestamp = token; break;
            case 2: tx.sender_account = token; break;
            case 3: tx.receiver_account = token; break;
            case 4: tx.amount = token.empty() ? 0.0 : atof(token.c_str()); break;
            case 5: tx.transaction_type = token; break;
            case 6: tx.merchant_category = token; break;
            case 7: tx.location = token; break;
            case 8: tx.device_used = token; break;
            case 9: tx.is_fraud = token; break;
            case 10: tx.fraud_type = token.empty() ? "N/A" : token; break;
            case 11: tx.time_since_last_transaction = token.empty() ? 0.0 : atof(token.c_str()); break;
            case 12: tx.spending_deviation_score = token.empty() ? 0.0 : atof(token.c_str()); break;
            case 13: tx.velocity_score = token.empty() ? 0 : atoi(token.c_str()); break;
            case 14: tx.geo_anomaly_score = token.empty() ? 0.0 : atof(token.c_str()); break;
            case 15: tx.payment_channel = token; break;
            case 16: tx.ip_address = token; break;
            case 17: tx.device_hash = token; break;
        }

        field++;
    }

    return (field == 18);
}

void storeSeperate() {
    FILE *fp;
    char row[MAXCHAR];
    int count = 0;
    char input[50];

    cout << "Enter payment channel to filter (card, wire_transfer, ACH, UPI): ";
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;  // Remove newline

    if (strcmp(input, "card") != 0 &&
        strcmp(input, "wire_transfer") != 0 &&
        strcmp(input, "ACH") != 0 &&
        strcmp(input, "UPI") != 0) {
        cout << "Please key in correct filter (card, wire_transfer, ACH, UPI)\n";
        return;
    }   


    fp = fopen("array/financial_fraud_detection_dataset.csv", "r");
    if (!fp) {
        cout << "Error opening file.\n";
        return;
    }

    // Skip header
    fgets(row, MAXCHAR, fp);

    while (fgets(row, MAXCHAR, fp) != NULL && count < MAX_FILTERED_ROWS) {
        if (strstr(row, input) != NULL) {
            char row_copy[MAXCHAR];
            strcpy(row_copy, row); // Make a copy since strtok modifies it
            if (parseTransaction(row_copy, filtered_transactions[count])) {
                count++;
            }
        }
    }

    fclose(fp);
    transactionCount = count;
    cout << "Filtered and stored " << count << " rows for channel \"" << input << "\".\n";
}

void storeAll(){

    FILE *fp;
    char row[MAXCHAR];
    int count = 0;

    fp = fopen("array/financial_fraud_detection_dataset.csv", "r");
    if (!fp) {
        cout << "Error opening file.\n";
        return;
    }

    // Skip header  
    fgets(row, MAXCHAR, fp);

    while (fgets(row, MAXCHAR, fp) != NULL && count < MAX_FILTERED_ROWS) {
        char row_copy[MAXCHAR];
        strcpy(row_copy, row); // Make a copy since strtok modifies it
        if (parseTransaction(string(row_copy), filtered_transactions[count])) {
            count++;
        }

        if (count % 1000000 == 0) {
            cout << "Parsed " << count << " transactions...\n";
        }
    }

    fclose(fp);
    transactionCount = count;
    cout << "Finished storing " << count << " transactions.\n";
}
