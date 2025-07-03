#pragma once
#include <string>
#include "store.hpp" // for Transaction struct

void generate_json_from_linkedlist(Transaction* head, const std::string& filename);