#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include "search.hpp"
#include "store.hpp"
#include "sort_before_search.hpp"
#include <limits>

using namespace std;
using namespace std::chrono;

// Helper: convert string to lowercase
string to_lowercase(const string &str)
{
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// Search for a specific transaction type in a linked list
void searchByType(Transaction *head, const string &type, int column_input)
{
    Transaction *temp = head;
    bool found = false;

    while (temp != nullptr)
    {
        string field_value;

        switch (column_input)
        {
        case 1:
            field_value = temp->transaction_id;
            break;
        case 2:
            field_value = temp->sender_account;
            break;
        case 3:
            field_value = temp->receiver_account;
            break;
        case 4:
            field_value = std::to_string(temp->amount);
            break;
        case 5:
            field_value = temp->transaction_type;
            break;
        case 6:
            field_value = temp->location;
            break;
        case 7:
            field_value = temp->fraud_type;
            break;
        default:
            std::cout << "Invalid column input.\n";
            return;
        }

        // Convert both to lowercase for case-insensitive match
        if (to_lowercase(field_value) == to_lowercase(type))
        {
            found = true;
            cout << "ID: " << temp->transaction_id
                 << ", Amount: $" << temp->amount
                 << ", Location: " << temp->location
                 << ", Type: " << temp->transaction_type << endl;
        }

        temp = temp->next;
    }

    if (!found)
    {
        cout << "No transactions found for the given search.\n";
    }
}

// This function integrates channel selection + search
void search_by_payment_channel_and_type()
{

    string channel;
    Transaction *selected = nullptr;

    while (true)
    {
        cout << "Enter payment channel (card, ACH, UPI, wire_transfer): ";
        cin >> channel;

        channel = to_lowercase(channel);

        if (channel == "card")
        {
            selected = card_head;
            break;
        }
        else if (channel == "ach")
        {
            selected = ACH_head;
            break;
        }
        else if (channel == "upi")
        {
            selected = UPI_head;
            break;
        }
        else if (channel == "wire_transfer")
        {
            selected = wire_transfer_head;
            break;
        }
        else
        {
            cout << "Invalid payment channel.\n";
        }
    }

    string type;
    int column_input;
    string column_name;
    int method;

    while (true)
    {
        cout << "Which column you want to search? (Transaction ID[1], Sender Account[2], Receiver Account[3], Amount[4], Transaction Type[5], Location[6], Fraud Type[7]): ";
        cin >> column_input;

        if (cin.fail())
        {
            cin.clear();                                                   // clear error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            cout << "Invalid input. Please enter a number (1-7).\n";
            continue;
        }

        switch (column_input)
        {
        case 1:
            column_name = "transaction_id";
            break;
        case 2:
            column_name = "sender_account";
            break;
        case 3:
            column_name = "receiver_account";
            break;
        case 4:
            column_name = "amount";
            break;
        case 5:
            column_name = "transaction_type";
            break;
        case 6:
            column_name = "location";
            break;
        case 7:
            column_name = "fraud_type";
            break;
        default:
            cout << "Invalid input. Please insert (1/2/3/4/5/6/7)\n";
            continue;
        }
        break;
    }

    while (true)
    {
        cout << "Enter the value to search: ";
        cin >> type;

        cout << "Methods to search (linear search[1], Sort before search[2]): ";
        cin >> method;

        if (cin.fail())
        {
            cin.clear();                                                   // clear error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            cout << "Invalid input. Please enter a number (1 or 2).\n";
            continue;
        }

        if (method == 1)
        {
            auto start = high_resolution_clock::now();
            cout << "\n--- Search Results for '" << column_name << "' in channel '" << channel << "' with the value of '" << type << "'(Linear search)---\n";
            searchByType(selected, type, column_input);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            cout << "Search took " << duration.count() << " microseconds.\n";
            return;
        }
        else if (method == 2)
        {
            auto start = high_resolution_clock::now();
            cout << "\n--- Search Results for '" << column_name << "' in channel '" << channel << "' with the value of '" << type << "'(Binary search)---\n";
            searchByType2(selected, type, column_input);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            cout << "Search took " << duration.count() << " microseconds.\n";
            return;
        }

    }
}
