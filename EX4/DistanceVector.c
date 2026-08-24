#include <stdio.h>
#include <stdbool.h>

#define INF 999
#define MAX 20

int graph[MAX][MAX];
int final_cost[MAX][MAX];
int next_hop_matrix[MAX][MAX];
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

/* Core Distance Vector Computation using the Bellman-Ford Principle */
void calculateDistanceVector() {
    int i, j, k, count;

    // Step 1: Initialize the routing tables with direct neighbor graphs
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            final_cost[i][j] = graph[i][j];
            if (graph[i][j] != INF && i != j) {
                next_hop_matrix[i][j] = j;
            } else {
                next_hop_matrix[i][j] = -1;
            }
        }
    }

    // Step 2: Iteratively share tables with neighbors (Relaxation V-1 times)
    for (count = 0; count < V - 1; count++) {
        for (i = 0; i < V; i++) {
            for (j = 0; j < V; j++) {
                for (k = 0; k < V; k++) {
                    if (final_cost[i][k] != INF && graph[k][j] != INF) {
                        if (final_cost[i][k] + graph[k][j] < final_cost[i][j]) {
                            final_cost[i][j] = final_cost[i][k] + graph[k][j];
                            next_hop_matrix[i][j] = next_hop_matrix[i][k];
                        }
                    }
                }
            }
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
    printf("\n             INDIVIDUAL ROUTER DISTANCE VECTOR TABLES                     ");
    printf("\n==========================================================================\n");

    for (i = 0; i < V; i++) {
        printf("\nRouting Table for Router %c:\n", indexToChar(i));
        printf("Destination\tCost\tNext Hop\n");
        printf("--------------------------------------------------------------------------\n");
        for (j = 0; j < V; j++) {
            printf("%c\t\t", indexToChar(j));

            if (final_cost[i][j] == INF) {
                printf("∞\t∞\n");
            } else {
                printf("%d\t", final_cost[i][j]);
                if (i == j) {
                    printf("-\n");
                } else {
                    printf("%c\n", indexToChar(next_hop_matrix[i][j]));
                }
            }
        }
    }
    printf("==========================================================================\n");
}

void runRoutingPipeline() {
    displayInitialGrid();
    calculateDistanceVector();
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
