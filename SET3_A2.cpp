#include <vector>
#include <functional>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

class MergeSort {
private:
    static void merge(std::vector<int>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<int> leftArray(n1);
        std::vector<int> rightArray(n2);

        for (int i = 0; i < n1; ++i) leftArray[i] = arr[left + i];
        for (int j = 0; j < n2; ++j) rightArray[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (leftArray[i] <= rightArray[j]) {
                arr[k++] = leftArray[i++];
            }
            else {
                arr[k++] = rightArray[j++];
            }
        }

        while (i < n1) arr[k++] = leftArray[i++];
        while (j < n2) arr[k++] = rightArray[j++];
    }

    static void mergeSort(std::vector<int>& arr, int left, int right) {
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

public:
    static void sort(std::vector<int>& arr) {
        mergeSort(arr, 0, arr.size() - 1);
    }
};

// Êëàññ äëÿ ãåíåðàöèè ìàññèâîâ
class ArrayGenerator {
public:
    static std::vector<int> generateRandomArray(int size, int minValue, int maxValue) {
        std::vector<int> array(size);
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(minValue, maxValue);

        for (int& val : array) {
            val = dist(rng);
        }
        return array;
    }

    static std::vector<int> generateReversedArray(int size) {
        std::vector<int> array(size);
        for (int i = 0; i < size; ++i) {
            array[i] = size - i;
        }
        return array;
    }

    static std::vector<int> generateNearlySortedArray(int size, int swaps = 10) {
        std::vector<int> array(size);
        for (int i = 0; i < size; ++i) {
            array[i] = i;
        }

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, size - 1);

        for (int i = 0; i < swaps; ++i) {
            int idx1 = dist(rng);
            int idx2 = dist(rng);
            std::swap(array[idx1], array[idx2]);
        }
        return array;
    }
};

// Ãèáðèäíûé àëãîðèòì Merge + Insertion Sort
class HybridMergeInsertionSort {
private:
    static const int threshold = 15; // Ïîðîã ïåðåêëþ÷åíèÿ íà Insertion Sort

    static void insertionSort(std::vector<int>& arr, int left, int right) {
        for (int i = left + 1; i <= right; ++i) {
            int key = arr[i];
            int j = i - 1;
            while (j >= left && arr[j] > key) {
                arr[j + 1] = arr[j];
                --j;
            }
            arr[j + 1] = key;
        }
    }

    static void merge(std::vector<int>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<int> leftArray(n1);
        std::vector<int> rightArray(n2);

        for (int i = 0; i < n1; ++i) leftArray[i] = arr[left + i];
        for (int j = 0; j < n2; ++j) rightArray[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (leftArray[i] <= rightArray[j]) {
                arr[k++] = leftArray[i++];
            }
            else {
                arr[k++] = rightArray[j++];
            }
        }

        while (i < n1) arr[k++] = leftArray[i++];
        while (j < n2) arr[k++] = rightArray[j++];
    }

    static void hybridSort(std::vector<int>& arr, int left, int right) {
        if (right - left + 1 <= threshold) {
            insertionSort(arr, left, right);
            return;
        }

        int mid = left + (right - left) / 2;
        hybridSort(arr, left, mid);
        hybridSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

public:
    static void sort(std::vector<int>& arr) {
        hybridSort(arr, 0, arr.size() - 1);
    }
};

// Êëàññ äëÿ òåñòèðîâàíèÿ ñîðòèðîâîê
class SortTester {
public:
    static void testSort(
        const std::vector<int>& baseArray,
        const std::function<void(std::vector<int>&)>& sortFunction,
        const std::string& sortName,
        const std::string& arrayType,
        int repetitions,
        const std::string& outputFile
    ) {
        std::ofstream file(outputFile, std::ios::app);

        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << outputFile << "\n";
            return;
        }

        std::cout << "Testing " << sortName << " on " << arrayType << " array...\n";

        for (int size = 500; size <= static_cast<int>(baseArray.size()); size += 100) {
            long long totalTime = 0;

            for (int rep = 0; rep < repetitions; ++rep) {
                std::vector<int> subArray(baseArray.begin(), baseArray.begin() + size);
                totalTime += measureSortTime(subArray, sortFunction);
            }

            long long averageTime = totalTime / repetitions;
            std::cout << "Size: " << size
                << ", Average Time: " << averageTime << " ms\n";

            file << size << "," << sortName << "," << arrayType << "," << averageTime << "\n";
        }

        file.close();
    }

private:
    static long long measureSortTime(
        const std::vector<int>& baseArray,
        const std::function<void(std::vector<int>&)>& sortFunction
    ) {
        std::vector<int> arr = baseArray;

        auto start = std::chrono::high_resolution_clock::now();
        sortFunction(arr);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;

        return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    }
};

int main() {
    const int maxSize = 10000;
    const int repetitions = 5;
    const std::string outputFile = "sort_results.csv";

    auto randomArray = ArrayGenerator::generateRandomArray(maxSize, 0, 6000);
    auto reversedArray = ArrayGenerator::generateReversedArray(maxSize);
    auto nearlySortedArray = ArrayGenerator::generateNearlySortedArray(maxSize);

    std::ofstream clearFile(outputFile, std::ios::trunc);
    clearFile << "size,sort_name,array_type,average_time\n";
    clearFile.close();

    // Òåñòèðîâàíèå Merge Sort
    SortTester::testSort(randomArray, MergeSort::sort,
        "Merge Sort", "random", repetitions, outputFile);

    SortTester::testSort(reversedArray, MergeSort::sort,
        "Merge Sort", "reversed", repetitions, outputFile);

    SortTester::testSort(nearlySortedArray, MergeSort::sort,
        "Merge Sort", "nearly sorted", repetitions, outputFile);

    // Òåñòèðîâàíèå Merge+Insertion Sort
    SortTester::testSort(randomArray, HybridMergeInsertionSort::sort,
        "Hybrid Merge+Insertion Sort", "random", repetitions, outputFile);

    SortTester::testSort(reversedArray, HybridMergeInsertionSort::sort,
        "Hybrid Merge+Insertion Sort", "reversed", repetitions, outputFile);

    SortTester::testSort(nearlySortedArray, HybridMergeInsertionSort::sort,
        "Hybrid Merge+Insertion Sort", "nearly sorted", repetitions, outputFile);

    return 0;
}
