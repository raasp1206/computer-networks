#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 1e9

int findCheapestPrice(int n, int** flights, int flightsSize, int* flightsColSize, int src, int dst, int k) {
    // 1. Allocate space for current and temporary distance tracking arrays
    int* dist = (int*)malloc(n * sizeof(int));
    int* temp = (int*)malloc(n * sizeof(int));

    // Initialize all distances to infinity
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    dist[src] = 0; // Distance to the source city is 0

    // 2. Relax edges up to k + 1 times (representing up to k stops)
    for (int i = 0; i <= k; i++) {
        // Copy current distances to temp to prevent updates using flights processed in the same layer
        memcpy(temp, dist, n * sizeof(int));

        for (int j = 0; j < flightsSize; j++) {
            int u = flights[j][0];
            int v = flights[j][1];
            int price = flights[j][2];

            // Only expand from a city that has already been reached in a previous layer
            if (dist[u] != INF) {
                if (dist[u] + price < temp[v]) {
                    temp[v] = dist[u] + price;
                }
            }
        }
        // Update the main distance array with values computed in this layer
        memcpy(dist, temp, n * sizeof(int));
    }

    // 3. Extract final answer and cleanup allocated heap buffers
    int result = dist[dst] == INF ? -1 : dist[dst];

    free(dist);
    free(temp);

    return result;
}
