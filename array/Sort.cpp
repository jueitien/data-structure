#include "Store.hpp"
#include "Data_structure.hpp"
#include <iostream>
#include <chrono>
using namespace std;

int transactionCount = 0;


void heapify(Transaction* arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    // Compare with left child
    if (l < n && arr[l].location > arr[largest].location)
        largest = l;

    // Compare with right child
    if (r < n && arr[r].location > arr[largest].location)
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort() {
    cout << "Sorting " << transactionCount << " transactions by location..." << endl;
    auto start = std::chrono::high_resolution_clock::now();

    int n = transactionCount;
    Transaction* arr = filtered_transactions;

    // Build heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extract elements from heap
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    cout << "Heap Sort Time: " << duration.count() << " seconds" << endl;

    std::string ans;
    cout << "Do you want to generate a result in JSON format? (Y/N)\nEnter your choice: ";
    getline(cin >> ws, ans); 

    if (ans == "Y" || ans == "y") {
        generateJsonReport(arr, n);  
    }  else {
        return;
    }
}


void merge(int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Transaction* L = new Transaction[n1];
    Transaction* R = new Transaction[n2];

    for (int i = 0; i < n1; i++)
        L[i] = filtered_transactions[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = filtered_transactions[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].location <= R[j].location) {
            filtered_transactions[k] = L[i];
            i++;
        } else {
            filtered_transactions[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        filtered_transactions[k++] = L[i++];
    }

    while (j < n2) {
        filtered_transactions[k++] = R[j++];
    }

    delete[] L;
    delete[] R;
}

void mergeSort(int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(left, mid);
    mergeSort(mid + 1, right);
    merge(left, mid, right);

}

void sortByMergeSort() {
    cout << "Sorting " << transactionCount << " transactions by location..." << endl;
    auto start = std::chrono::high_resolution_clock::now();

    mergeSort(0, transactionCount - 1);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Merge Sort by Location Time: " << duration.count() << " seconds\n";

    string ans;
    cout << "Do you want to generate a result in JSON format? (Y/N)\nEnter your choice: ";
    getline(cin >> ws, ans); 

    if (ans == "Y" || ans == "y") {
        generateJsonReport(filtered_transactions, transactionCount);  
    } else {
        return;
    }
}
