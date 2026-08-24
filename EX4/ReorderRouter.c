#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to represent a directed edge in the adjacency list
typedef struct {
    int to;
    int is_original; // 1 if original edge (away from capital), 0 if virtual reverse edge
} Edge;

int minReorder(int n, int** connections, int connectionsSize, int* connectionsColSize) {
    // 1. Count the degree of each node to allocate precise memory
    int* degree = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < connectionsSize; i++) {
        int u = connections[i][0];
        int v = connections[i][1];
        degree[u]++;
        degree[v]++;
    }

    // 2. Allocate the adjacency list
    Edge** adj = (Edge**)malloc(n * sizeof(Edge*));
    int* head_idx = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        adj[i] = (Edge*)malloc(degree[i] * sizeof(Edge));
    }

    // 3. Populate the adjacency list
    for (int i = 0; i < connectionsSize; i++) {
        int u = connections[i][0];
        int v = connections[i][1];

        // Original road: u -> v (Points away from 0 during outward BFS, needs 1 cost to flip)
        adj[u][head_idx[u]++] = (Edge){v, 1};

        // Virtual road: v -> u (Points toward 0, needs 0 cost to flip)
        adj[v][head_idx[v]++] = (Edge){u, 0};
    }

    // 4. Set up BFS tracking structures
    int change_count = 0;
    bool* visited = (bool*)calloc(n, sizeof(bool));
    int* queue = (int*)malloc(n * sizeof(int));
    int head = 0, tail = 0;

    // Start traversal from the capital (city 0)
    queue[tail++] = 0;
    visited[0] = true;

    // 5. Run BFS Loop
    while (head < tail) {
        int curr = queue[head++];

        // Explore all connected roads for the current city
        for (int i = 0; i < degree[curr]; i++) {
            Edge edge = adj[curr][i];
            int neighbor = edge.to;

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                change_count += edge.is_original; // Add 1 if pointing away from capital
                queue[tail++] = neighbor;
            }
        }
    }

    // 6. Free all allocated memory blocks
    for (int i = 0; i < n; i++) {
        free(adj[i]);
    }
    free(adj);
    free(degree);
    free(head_idx);
    free(visited);
    free(queue);

    return change_count;
}
