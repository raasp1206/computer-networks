#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STOPS 1000000

// Structure to build an adjacency list of stops pointing to buses
typedef struct {
    int* bus_ids;
    int size;
    int capacity;
} StopToBuses;

// Queue node structure for BFS
typedef struct {
    int bus_id;
    int count;
} QueueNode;

int numBusesToDestination(int** routes, int routesSize, int* routesColSize, int source, int target) {
    // 1. Edge Case: Already at target
    if (source == target) return 0;

    // 2. Allocate and initialize the Stop-to-Bus mapping table
    StopToBuses* stop_to_buses = (StopToBuses*)calloc(MAX_STOPS, sizeof(StopToBuses));

    // Populate the stop-to-bus map
    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            int stop = routes[i][j];
            if (stop_to_buses[stop].size == stop_to_buses[stop].capacity) {
                stop_to_buses[stop].capacity = stop_to_buses[stop].capacity == 0 ? 4 : stop_to_buses[stop].capacity * 2;
                stop_to_buses[stop].bus_ids = (int*)realloc(stop_to_buses[stop].bus_ids, stop_to_buses[stop].capacity * sizeof(int));
            }
            stop_to_buses[stop].bus_ids[stop_to_buses[stop].size++] = i;
        }
    }

    // 3. Setup Visited Trackers
    bool* visited_buses = (bool*)calloc(routesSize, sizeof(bool));
    bool* visited_stops = (bool*)calloc(MAX_STOPS, sizeof(bool));

    // 4. Initialize BFS Queue
    // Maximum possible queue items is bounded by the total number of buses
    QueueNode* queue = (QueueNode*)malloc(routesSize * sizeof(QueueNode));
    int head = 0, tail = 0;

    // Enqueue all buses passing through the initial source stop
    visited_stops[source] = true;
    for (int i = 0; i < stop_to_buses[source].size; i++) {
        int bus_id = stop_to_buses[source].bus_ids[i];
        queue[tail++] = (QueueNode){bus_id, 1};
        visited_buses[bus_id] = true;
    }

    // 5. BFS Loop
    while (head < tail) {
        QueueNode curr = queue[head++];
        int curr_bus = curr.bus_id;
        int count = curr.count;

        // Traverse all stops within this bus route
        for (int i = 0; i < routesColSize[curr_bus]; i++) {
            int stop = routes[curr_bus][i];

            if (stop == target) {
                // Free memory before exiting
                for (int j = 0; j < MAX_STOPS; j++) {
                    if (stop_to_buses[j].bus_ids) free(stop_to_buses[j].bus_ids);
                }
                free(stop_to_buses); free(visited_buses); free(visited_stops); free(queue);
                return count;
            }

            // Expand to neighboring buses if the stop has not been explored yet
            if (!visited_stops[stop]) {
                visited_stops[stop] = true;
                for (int j = 0; j < stop_to_buses[stop].size; j++) {
                    int next_bus = stop_to_buses[stop].bus_ids[j];
                    if (!visited_buses[next_bus]) {
                        visited_buses[next_bus] = true;
                        queue[tail++] = (QueueNode){next_bus, count + 1};
                    }
                }
            }
        }
    }

    // 6. Cleanup allocated heap memory on path failure
    for (int j = 0; j < MAX_STOPS; j++) {
        if (stop_to_buses[j].bus_ids) free(stop_to_buses[j].bus_ids);
    }
    free(stop_to_buses); free(visited_buses); free(visited_stops); free(queue);

    return -1;
}
