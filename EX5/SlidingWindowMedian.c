#include <stdlib.h>

// Structure for elements inside the priority queue (heap)
typedef struct {
    long long val;
    int index;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
    int isMinHeap; // 1 for Min-Heap, 0 for Max-Heap
} Heap;

// Function to initialize a heap
Heap* createHeap(int capacity, int isMinHeap) {
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->data = (HeapNode*)malloc(sizeof(HeapNode) * (capacity + 1));
    h->size = 0;
    h->capacity = capacity;
    h->isMinHeap = isMinHeap;
    return h;
}

// Comparison helper function
int compare(HeapNode a, HeapNode b, int isMinHeap) {
    if (a.val == b.val) return 0;
    if (isMinHeap) {
        return (a.val < b.val) ? -1 : 1;
    } else {
        return (a.val > b.val) ? -1 : 1;
    }
}

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Push to heap
void push(Heap* h, long long val, int index) {
    h->size++;
    h->data[h->size].val = val;
    h->data[h->size].index = index;
    
    int i = h->size;
    while (i > 1 && compare(h->data[i], h->data[i / 2], h->isMinHeap) < 0) {
        swap(&h->data[i], &h->data[i / 2]);
        i /= 2;
    }
}

// Pop from heap
void pop(Heap* h) {
    if (h->size == 0) return;
    h->data[1] = h->data[h->size];
    h->size--;
    
    int i = 1;
    while (2 * i <= h->size) {
        int left = 2 * i;
        int right = 2 * i + 1;
        int best = left;
        
        if (right <= h->size && compare(h->data[right], h->data[left], h->isMinHeap) < 0) {
            best = right;
        }
        if (compare(h->data[best], h->data[i], h->isMinHeap) < 0) {
            swap(&h->data[i], &h->data[best]);
            i = best;
        } else {
            break;
        }
    }
}

HeapNode top(Heap* h) {
    return h->data[1];
}

// Prune invalid elements (elements out of the current sliding window index)
void prune(Heap* h, int minIndex) {
    while (h->size > 0 && top(h).index < minIndex) {
        pop(h);
    }
}

double* medianSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    *returnSize = numsSize - k + 1;
    double* result = (double*)malloc(sizeof(double) * (*returnSize));
    
    Heap* maxHeap = createHeap(numsSize, 0); 
    Heap* minHeap = createHeap(numsSize, 1);
    
    int maxHeapValidCount = 0;
    int minHeapValidCount = 0;

    for (int i = 0; i < numsSize; i++) {
        long long val = nums[i];

        // 1. Insert the new element
        if (maxHeapValidCount == 0 || val <= top(maxHeap).val) {
            push(maxHeap, val, i);
            maxHeapValidCount++;
        } else {
            push(minHeap, val, i);
            minHeapValidCount++;
        }

        // 2. Remove the element that slid out of the window lazily
        if (i >= k) {
            int outIndex = i - k;
            long long outVal = nums[outIndex];
            if (outVal <= top(maxHeap).val) {
                maxHeapValidCount--;
            } else {
                minHeapValidCount--;
            }
        }

        // 3. Rebalance the heaps based on valid counts
        while (maxHeapValidCount > minHeapValidCount + 1) {
            prune(maxHeap, i - k + 1);
            HeapNode t = top(maxHeap);
            pop(maxHeap);
            push(minHeap, t.val, t.index);
            maxHeapValidCount--;
            minHeapValidCount++;
        }
        while (minHeapValidCount > maxHeapValidCount) {
            prune(minHeap, i - k + 1);
            HeapNode t = top(minHeap);
            pop(minHeap);
            push(maxHeap, t.val, t.index);
            minHeapValidCount--;
            maxHeapValidCount++;
        }

        // 4. Prune stale nodes from tops of both heaps
        prune(maxHeap, i - k + 1);
        prune(minHeap, i - k + 1);

        // 5. Calculate and record the median
        if (i >= k - 1) {
            if (k % 2 == 1) {
                result[i - k + 1] = (double)top(maxHeap).val;
            } else {
                result[i - k + 1] = ((double)top(maxHeap).val + (double)top(minHeap).val) / 2.0;
            }
        }
    }

    // Clean up memory
    free(maxHeap->data); free(maxHeap);
    free(minHeap->data); free(minHeap);

    return result;
}
