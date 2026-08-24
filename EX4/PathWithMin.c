#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define INF 10000000

// Structure to represent a cell in the Min-Heap
typedef struct {
    int r;
    int c;
    int effort;
} HeapNode;

// Structure for the Min-Heap / Priority Queue
typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

// Helper to create a new Min-Heap
MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// Swap function for heap elements
void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Push an item into the Min-Heap
void push(MinHeap* heap, HeapNode node) {
    heap->data[heap->size] = node;
    int idx = heap->size;
    heap->size++;

    // Up-heapify
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[idx].effort < heap->data[parent].effort) {
            swap(&heap->data[idx], &heap->data[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

// Pop the minimum item from the Min-Heap
HeapNode pop(MinHeap* heap) {
    HeapNode minNode = heap->data[0];
    heap->size--;
    heap->data[0] = heap->data[heap->size];

    int idx = 0;
    // Down-heapify
    while (2 * idx + 1 < heap->size) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = left;

        if (right < heap->size && heap->data[right].effort < heap->data[left].effort) {
            smallest = right;
        }

        if (heap->data[idx].effort > heap->data[smallest].effort) {
            swap(&heap->data[idx], &heap->data[smallest]);
            idx = smallest;
        } else {
            break;
        }
    }
    return minNode;
}

int minimumEffortPath(int** heights, int heightsSize, int* heightsColSize) {
    int rows = heightsSize;
    int cols = heightsColSize[0];

    // 2D matrix to store the minimum effort to reach each cell
    int** effortMatrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        effortMatrix[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            effortMatrix[i][j] = INF;
        }
    }

    // Create priority queue bounded by total cells
    MinHeap* pq = createMinHeap(rows * cols * 4);

    // Base Case: Starting point
    effortMatrix[0][0] = 0;
    push(pq, (HeapNode){0, 0, 0});

    // Direction arrays for Up, Down, Left, Right movement
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (pq->size > 0) {
        HeapNode curr = pop(pq);
        int r = curr.r;
        int c = curr.c;
        int currEffort = curr.effort;

        // If we reached the destination cell, return the result early
        if (r == rows - 1 && c == cols - 1) {
            int result = currEffort;
            // Cleanup heap memory
            for (int i = 0; i < rows; i++) free(effortMatrix[i]);
            free(effortMatrix);
            free(pq->data);
            free(pq);
            return result;
        }

        // If we found a path with worse effort than already recorded, skip processing
        if (currEffort > effortMatrix[r][c]) continue;

        // Explore 4-directional neighbors
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Check boundary limits
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                // The effort for this specific step is the absolute height difference
                int stepEffort = abs(heights[r][c] - heights[nr][nc]);
                // The overall route effort is the maximum step effort seen along this route path
                int routeEffort = stepEffort > currEffort ? stepEffort : currEffort;

                // If this path offers a lower maximum effort to the neighbor, update it
                if (routeEffort < effortMatrix[nr][nc]) {
                    effortMatrix[nr][nc] = routeEffort;
                    push(pq, (HeapNode){nr, nc, routeEffort});
                }
            }
        }
    }

    return 0;
}
