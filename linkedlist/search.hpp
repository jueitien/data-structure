#pragma once
#include <string>
#include "node.hpp"

// Search a specific transaction type in a linked list
void searchByType(Transaction* head, const std::string& type);

std::string to_lowercase(const std::string& str);

// Handles user input for selecting payment channel and transaction type
void search_by_payment_channel_and_type();