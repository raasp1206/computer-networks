#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 1000000 // A value larger than any possible path length

int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k) {
    // 1. Initialize adjacency matrix with INF
    int** graph = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 1; i <= n; i++) {
        graph[i] = (int*)malloc((n + 1) * sizeof(int));
        for (int j = 1; j <= n; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }

    // Populate adjacency matrix from times array
    for (int i = 0; i < timesSize; i++) {
        int u = times[i][0];
        int v = times[i][1];
        int w = times[i][2];
        graph[u][v] = w;
    }

    // 2. Setup Dijkstra arrays (1-indexed to match node labels)
    int* dist = (int*)malloc((n + 1) * sizeof(int));
    bool* visited = (bool*)calloc((n + 1), sizeof(bool));

    for (int i = 1; i <= n; i++) {
        dist[i] = INF;
    }
    dist[k] = 0; // Distance to source is 0

    // 3. Dijkstra's core loop
    for (int count = 1; count <= n; count++) {
        // Find the unvisited node with the minimum distance
        int u = -1;
        int min_dist = INF;
        for (int i = 1; i <= n; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }

        // If no more reachable nodes are found, break early
        if (u == -1) break;
        visited[u] = true;

        // Relax edges from the picked node
        for (int v = 1; v <= n; v++) {
            if (!visited[v] && graph[u][v] != INF) {
                if (dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                }
            }
        }
    }

    // 4. Find the maximum time among all nodes
    int max_time = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INF) {
            max_time = -1; // Node i is unreachable
            break;
        }
        if (dist[i] > max_time) {
            max_time = dist[i];
        }
    }

    // 5. Free all allocated heap memory
    for (int i = 1; i <= n; i++) {
        free(graph[i]);
    }
    free(graph);
    free(dist);
    free(visited);

    return max_time;
}
