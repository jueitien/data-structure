#pragma once
#include "node.hpp"
#include <string>

// Helper function declarations
std::string trim(const std::string& s);
std::string clean_string(const std::string& s);
double clean_double(const std::string& s);
int clean_int(const std::string& s);
bool clean_bool(const std::string& s);

// Main loader function
Transaction* load_transactions_from_csv(const std::string& filename);
// Main loader function
Transaction* load_transactions_from_csv_raw(const std::string& filename);

// Global linked list heads for each payment channel
extern Transaction* card_head;
extern Transaction* ACH_head;
extern Transaction* UPI_head;
extern Transaction* wire_transfer_head;

// Function to split the linked list into four global lists by payment_channel
void split_by_payment_channel(Transaction* head);

Transaction* merge_all_payment_channels();