#include "graph.h"

/* function will display the primary user interface
    This is already done for you*/
void prompt(void)
{

    printf("\nMenu:\n");
    printf("1. Display Adjacency List\n");
    printf("2. Perform Breadth-First Search (BFS)\n");
    printf("3. Perform Depth-First Search (DFS)\n");
    printf("4. Find Shortest Path using Dijkstra's Algorithm\n");
    printf("5. Exit\n");
}

/**
 * Reads a graph from a file and constructs the graph structure.
 * @param filename The name of the file containing the adjacency matrix.
 * @return Pointer to the created Graph structure, or NULL if an error occurs.
 */
Graph *readGraph(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "Error: Memory allocation failed for Graph\n");
        fclose(file);
        return NULL;
    }

    int tempMatrix[MAX_VERTICES][MAX_VERTICES] = {0};
    int row = 0, col = 0, numVertices = 0;

    // Read file by each row
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        col = 0; 
        char *token = strtok(line, " ");
        while (token) {
            tempMatrix[row][col++] = atoi(token);
            token = strtok(NULL, " ");
        }
        row++;
    }
    // Number of rows = number of vertices
    numVertices = row; 
    graph->numVertices = numVertices;

    // Make tempMatrix to adjMatrix
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            graph->adjMatrix[i][j] = tempMatrix[i][j];
        }
    }

    fclose(file);
    return graph;
}

/**
 * Creates a new node for the adjacency list.
 * @param vertex The vertex number that this node will represent.
 * @return Pointer to the newly created Node structure.
 */
Node *createNode(int vertex)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

/**
 * Displays the adjacency list of the graph.
 * @param graph Pointer to the Graph structure.
 */
void displayAdjacencyList(Graph *graph)
{
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Vertex %d: ", i + 1);
        Node *current = graph->adjList[i];
        //print each adjacent vertex
        while (current) {
            printf("-> %d ", current->vertex);
            current = current->next;
        }
        //End of adjacency list
        printf("NULL\n");
    }
}

/**
 * Converts the adjacency matrix of the graph to an adjacency list.
 * @param graph Pointer to the Graph structure.
 */
void createAdjacencyList(Graph *graph) 
{
    //Initialize adjacency list
    for (int i = 0; i < graph->numVertices; i++) {
        graph->adjList[i] = NULL;

        for (int j = 0; j < graph->numVertices; j++) {
            if (graph->adjMatrix[i][j] > 0) {
                Node *newNode = createNode(j + 1);
                newNode->next = graph->adjList[i];
                //update head of the list
                graph->adjList[i] = newNode;
            }
        }
    }
}


/**
 * Performs Breadth-First Search (BFS) starting from a given vertex.
 * @param graph Pointer to the Graph structure.
 * @param startVertex The vertex from which BFS starts (0-based index).
 */
void bfs(Graph *graph, int startVertex) 
{
    bool visited[MAX_VERTICES] = {false};
    int queue[MAX_VERTICES], front = 0, rear = 0;

    printf("Final BFS Order:\n");
    visited[startVertex] = true;
    queue[rear++] = startVertex;

    while (front < rear) {
        int currentVertex = queue[front++];
        printf("%d ", currentVertex + 1);

        // Collect neighbors in an array for sorting
        int neighbors[MAX_VERTICES];
        int neighborCount = 0;

        Node *temp = graph->adjList[currentVertex];
        while (temp) {
            neighbors[neighborCount++] = temp->vertex - 1; // Store 0-based index
            temp = temp->next;
        }

        // Sort neighbors array
        for (int i = 0; i < neighborCount - 1; i++) {
            for (int j = i + 1; j < neighborCount; j++) {
                if (neighbors[i] > neighbors[j]) {
                    int temp = neighbors[i];
                    neighbors[i] = neighbors[j];
                    neighbors[j] = temp;
                }
            }
        }

        // Process neighbors in sorted order
        for (int i = 0; i < neighborCount; i++) {
            if (!visited[neighbors[i]]) {
                visited[neighbors[i]] = true;
                queue[rear++] = neighbors[i];
            }
        }
    }
    printf("\n");
}



 void dfsHelp(Graph *graph, int vertex, bool visited[]) 
 {
        visited[vertex] = true;
        printf("%d ", vertex + 1);

        // Collect neighbors in an array
        int neighbors[MAX_VERTICES];
        int neighborCount = 0;

        Node *temp = graph->adjList[vertex];
        while (temp) {
            // Store index
            neighbors[neighborCount++] = temp->vertex - 1; 
            temp = temp->next;
        }

        // Sort neighbors array
        for (int i = 0; i < neighborCount - 1; i++) {
            for (int j = i + 1; j < neighborCount; j++) {
                if (neighbors[i] > neighbors[j]) {
                    int temp = neighbors[i];
                    neighbors[i] = neighbors[j];
                    neighbors[j] = temp;
                }
            }
        }

        for (int i = 0; i < neighborCount; i++) {
            if (!visited[neighbors[i]]) {
                dfsHelp(graph, neighbors[i], visited);
            }
        }
    }
/**
 * Performs Depth-First Search (DFS) starting from a given vertex.
 * @param graph Pointer to the Graph structure.
 * @param startVertex The vertex from which DFS starts (0-based index).
 */
void dfs(Graph *graph, int startVertex)
{
    bool visited[MAX_VERTICES] = {false};
   // printf("DFS Order:\n");
    dfsHelp(graph, startVertex, visited);
    printf("\n");
}


/**
 * Finds the shortest path from the start vertex to all other vertices using Dijkstra's algorithm.
 * @param graph Pointer to the Graph structure.
 * @param startVertex The vertex from which to start the algorithm (0-based index).
 */
void dijkstra(Graph *graph, int startVertex)
{
     int distances[MAX_VERTICES], visited[MAX_VERTICES] = {false};
    for (int i = 0; i < graph->numVertices; i++) {
        distances[i] = INT_MAX;
    }
    distances[startVertex] = 0;

    for (int i = 0; i < graph->numVertices; i++) {
        int minDistance = INT_MAX, minVertex = -1;

        for (int j = 0; j < graph->numVertices; j++) {
            if (!visited[j] && distances[j] < minDistance) {
                minDistance = distances[j];
                minVertex = j;
            }
        }
        // If no vertex is reachable, stop processing
        if (minVertex == -1) break;
        visited[minVertex] = true;
        // Update distances of adjacent vertices
        for (int j = 0; j < graph->numVertices; j++) {
            if (graph->adjMatrix[minVertex][j] > 0 && !visited[j]) {
                int newDistance = distances[minVertex] + graph->adjMatrix[minVertex][j];
                if (newDistance < distances[j]) {
                    distances[j] = newDistance;
                }
            }
        }
    }
    //print shortest distances
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Shortest distance from vertex %d to vertex %d: %d\n", startVertex + 1, i + 1, distances[i]);
    }
}

/**
 * Frees the memory allocated for the graph, including the adjacency list.
 * @param graph Pointer to the Graph structure to be freed.
 */
void freeGraph(Graph *graph)
{
    for (int i = 0; i < graph->numVertices; i++) {
        Node *current = graph->adjList[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}
