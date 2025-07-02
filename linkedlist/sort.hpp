#pragma once
#include "node.hpp"
#include <string>

// Sort order enum
enum class SortOrder { Ascending, Descending };

// Merge sort for linked list by any field
Transaction* merge_sort_by_field(Transaction* head, const std::string& field, SortOrder order);

// Insertion sort for linked list by any field
Transaction* insertion_sort_by_field(Transaction* head, const std::string& field, SortOrder order);

// User prompt to choose sorting algorithm, field, and order
void sort_transactions(Transaction*& head);
