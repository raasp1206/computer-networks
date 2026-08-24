#include <stdio.h>
#include <stdbool.h>

#define INF 999
#define MAX 20

int graph[MAX][MAX];
int final_cost[MAX][MAX];
int next_hop_matrix[MAX][MAX];
int parent_matrix[MAX][MAX];
int V;

int charToIndex(char c) {
    return c - 'a';
}

char indexToChar(int i) {
    return i + 'a';
}

void readinput() {
    int E;
    char u_char, v_char;
    int cost;
    int i, j;

    printf("Enter the total number of routers (vertices): ");
    scanf("%d", &V);

    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }

    printf("Enter the total number of connections (edges): ");
    scanf("%d", &E);

    printf("Enter connections in format: [Source] [Destination] [Cost] (e.g., a b 5)\n");
    for (i = 0; i < E; i++) {
        printf("Edge %d: ", i + 1);
        scanf(" %c %c %d", &u_char, &v_char, &cost);

        int u = charToIndex(u_char);
        int v = charToIndex(v_char);

        graph[u][v] = cost;
        graph[v][u] = cost;
    }
}

void displayInitialGrid() {
    int i, j;
    printf("\n=========================================================");
    printf("\n             INITIAL LINK VERIFICATION MATRIX            ");
    printf("\n=========================================================\n");

    printf("Src\\Dest\t");
    for (j = 0; j < V; j++) {
        printf("%c\t", indexToChar(j));
    }
    printf("\n---------------------------------------------------------\n");

    for (i = 0; i < V; i++) {
        printf("%c\t\t", indexToChar(i));
        for (j = 0; j < V; j++) {
            if (i == j) {
                printf("0\t");
            }
            else if (graph[i][j] != INF) {
                printf("1\t");
            }
            else {
                printf("∞\t");
            }
        }
        printf("\n");
    }
    printf("=========================================================\n");
}

void printPathSequence(int src, int dest) {
    if (parent_matrix[src][dest] == -1) {
        printf("%c", indexToChar(dest));
        return;
    }
    printPathSequence(src, parent_matrix[src][dest]);
    printf(" -> %c", indexToChar(dest));
}

void calculateDijkstra(int src) {
    bool visited[MAX];
    int parent[MAX];
    int i, count, v, dest;

    for (i = 0; i < V; i++) {
        final_cost[src][i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    final_cost[src][src] = 0;

    for (count = 0; count < V - 1; count++) {
        int min = INF, u = -1;
        for (v = 0; v < V; v++) {
            if (!visited[v] && final_cost[src][v] <= min) {
                min = final_cost[src][v];
                u = v;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        for (v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] != INF && final_cost[src][u] != INF && final_cost[src][u] + graph[u][v] < final_cost[src][v]) {
                final_cost[src][v] = final_cost[src][u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    for (dest = 0; dest < V; dest++) {
        parent_matrix[src][dest] = parent[dest];

        if (dest == src) {
            next_hop_matrix[src][dest] = src;
        } else if (final_cost[src][dest] == INF) {
            next_hop_matrix[src][dest] = -1;
        } else {
            int curr = dest;
            while (parent[curr] != src && parent[curr] != -1) {
                curr = parent[curr];
            }
            next_hop_matrix[src][dest] = curr;
        }
    }
}

void displayFinalGrid() {
    int i, j;
    printf("\n=========================================================");
    printf("\n             FINAL OPTIMIZED ROUTING COST MATRIX         ");
    printf("\n=========================================================\n");

    printf("Src\\Dest\t");
    for (j = 0; j < V; j++) {
        printf("%c\t", indexToChar(j));
    }
    printf("\n---------------------------------------------------------\n");

    for (i = 0; i < V; i++) {
        printf("%c\t\t", indexToChar(i));
        for (j = 0; j < V; j++) {
            if (final_cost[i][j] == INF) {
                printf("∞\t");
            } else {
                printf("%d\t", final_cost[i][j]);
            }
        }
        printf("\n");
    }
    printf("=========================================================\n");
}

void displaySeparateTables() {
    int i, j;
    printf("\n==========================================================================");
    printf("\n             INDIVIDUAL ROUTER TABLES WITH SHORTEST PATHS                 ");
    printf("\n==========================================================================\n");

    for (i = 0; i < V; i++) {
        printf("\nRouting Table for Router %c:\n", indexToChar(i));
        printf("Destination\tCost\tNext Hop\tShortest Path\n");
        printf("--------------------------------------------------------------------------\n");
        for (j = 0; j < V; j++) {
            printf("%c\t\t", indexToChar(j));

            if (final_cost[i][j] == INF) {
                printf("∞\t∞\t\tUnreachable\n");
            } else {
                printf("%d\t", final_cost[i][j]);
                if (i == j) {
                    printf("-\t\t%c\n", indexToChar(i));
                } else {
                    printf("%c\t\t%c -> ", indexToChar(next_hop_matrix[i][j]), indexToChar(i));
                    printPathSequence(i, j);
                    printf("\n");
                }
            }
        }
    }
    printf("==========================================================================\n");
}

void runRoutingPipeline() {
    int i;
    displayInitialGrid();
    for (i = 0; i < V; i++) {
        calculateDijkstra(i);
    }
    displayFinalGrid();
    displaySeparateTables();
}

int main() {
    char choice;
    char u_char, v_char;
    int new_cost;

    readinput();

    printf("\n--- INITIAL STATE GENERATION ---");
    runRoutingPipeline();

    printf("\nDo you want to change any edge weight? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        printf("Enter the edge to update and its new cost [Source] [Destination] [New Cost] (e.g., a c 2): ");
        scanf(" %c %c %d", &u_char, &v_char, &new_cost);

        int u = charToIndex(u_char);
        int v = charToIndex(v_char);

        graph[u][v] = new_cost;
        graph[v][u] = new_cost;

        printf("\n=========================================================");
        printf("\n             UPDATED NETWORK TOPOLOGY RUN                ");
        printf("\n=========================================================");
        runRoutingPipeline();
    } else {
        printf("\nNo changes made. Exiting program execution safely.\n");
    }

    return 0;
}
