#include <stdlib.h>

// Helper comparison function for qsort to remove duplicate values
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int subarrayBitwiseORs(int* arr, int arrSize) {
    if (arrSize == 0) return 0;

    // Trackers for unique results
    int* uniqueResults = (int*)malloc(arrSize * 32 * sizeof(int));
    int totalUniqueCount = 0;

    // Buffers to track OR results ending at the previous and current index
    int prevOrs[32];
    int prevSize = 0;

    for (int i = 0; i < arrSize; i++) {
        int currOrs[32];
        int currSize = 0;

        // 1. Every element forms a valid single-element subarray
        currOrs[currSize++] = arr[i];
        uniqueResults[totalUniqueCount++] = arr[i];

        // 2. Compute OR with all subarrays ending at the previous index
        for (int j = 0; j < prevSize; j++) {
            int newVal = prevOrs[j] | arr[i];
            
            // Avoid adding immediate duplicates to the current working window
            if (newVal != currOrs[currSize - 1]) {
                currOrs[currSize++] = newVal;
                uniqueResults[totalUniqueCount++] = newVal;
            }
        }

        // 3. Save current results to look back on during the next loop index
        for (int j = 0; j < currSize; j++) {
            prevOrs[j] = currOrs[j];
        }
        prevSize = currSize;
    }

    // 4. Sort all gathered results to easily filter down to unique entries
    qsort(uniqueResults, totalUniqueCount, sizeof(int), compare);

    // 5. Count unique values in-place
    int distinctCount = 0;
    if (totalUniqueCount > 0) {
        distinctCount = 1;
        for (int i = 1; i < totalUniqueCount; i++) {
            if (uniqueResults[i] != uniqueResults[i - 1]) {
                distinctCount++;
            }
        }
    }

    // Clean up allocated storage heap
    free(uniqueResults);

    return distinctCount;
}
