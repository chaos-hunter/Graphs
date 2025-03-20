#include "graph.h"

/*
 * A basic menu based structure has been provided
 */

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    Graph *graph = readGraph(argv[1]);
    if (!graph) {
        return 1;
    }
    //printAdjMatrix(graph); Debugging 
    createAdjacencyList(graph);
    //displayAdjacencyList(graph); Debugging
    int choice;
    // int startVertex = 1;

    do
    {

        // Display the options
        prompt();

        // Get user input
        printf("Enter a Choice: ");
        scanf("%d", &choice);

        /*
         * You will need to add all the cases here.
         * Call the relevant functions in each case.
         * Make sure to handle invalid inputs or edge cases.
         */

        switch (choice) {
            case 1:
                displayAdjacencyList(graph);
                break;
            case 2:
            //Start BFS from vertex 1
            bfs(graph, 0);
            break;
            case 3:
            //Start DFS from vertex 1
             dfs(graph, 0);
             break;
             case 4: 
             //Find shortest paths from vertex 1
             dijkstra(graph, 0);
             break;
             case 5:
             printf("Program ended");
             freeGraph(graph);
             return 0;
            default:
                printf("Invalid option, Please try again.\n");
                break;
        }

    } while (true);

    return 0;
}
