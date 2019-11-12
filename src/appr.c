#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#define  START_CITY 0            // Prim algorithm can specify the starting node, so I chose node 0.
#define  nint(x)    (int)(x+0.5) // nint() aims to do rounding of variable x.
#define  SIZE       48           // The number of cities.

int solveTSP(int matrix[][3]);
    // This function will figure out the adjacency matrix, and use it to do the following operation.
int PrimMST(int graph[][SIZE]);
    // This function will generate the minimum spanning tree (aka. MST).
int getHamilton(int i,int prim[][SIZE]);
    // This function will traverse the MST then find a Hamiltonian cycle.

int main(void) {
    FILE *fp = 0;
    int id = 0;                 // Temporarily store city ID.
    int x = 0;                  // Temporarily store city longitude.
    int y = 0;                  // Temporarily store city latitude.

    int matrix[SIZE][3];        // Temporarily store data from file.

    if ((fp = fopen(".\\data\\us48.dat", "r")) == NULL) {
        printf("\nCannot open file since it's null.");
        exit(EXIT_FAILURE);
    }
    printf("  Please wait a minute...");

    for (int count=0; count<SIZE; count++) {    // Read data from file.
        fscanf(fp, "%d", &id);
        matrix[count][0] = id;
        fscanf(fp, "%d", &x);
        matrix[count][1] = x;
        fscanf(fp, "%d", &y);
        matrix[count][2] = y;
    }
    fclose(fp);
    solveTSP(matrix);
    return 0;
}

int solveTSP(int matrix[][3]) {
    int xd, yd;                         // Current and next city ID
    int tij, dij;
    double rij;
    int graph[SIZE][SIZE];              // The adjacency matrix

    double run_time;
    LARGE_INTEGER start, end, freq;
    double dqFreq;                      // Line 50 to 54 is a tool aims to measure code segment run time exactly.
    QueryPerformanceFrequency(&freq);
    dqFreq=(double)freq.QuadPart;

    for (int i=0; i<SIZE; i++) {        // Calculate the distances then store them in the adjacency matrix.
        for (int j=0; j<SIZE; j++) {
            xd = matrix[i][1] - matrix[j][1];
            yd = matrix[i][2] - matrix[j][2];
            rij = sqrt( (xd*xd + yd*yd) / 10.0 );
            tij = nint(rij);
            if (tij<rij)
                dij = tij + 1;
            else
                dij = tij;
            graph[i][j] = dij;
        }
    }

    QueryPerformanceCounter(&start);
    PrimMST(graph);                     // Generate a MST then traverse it to find a Hamiltonian cycle.
    QueryPerformanceCounter(&end);

    run_time = 100.0 * (double)(end.QuadPart-start.QuadPart) / dqFreq ;
    printf("\n>> The running time of the algorithm is %lf second(s).", run_time);
    return 0;
}

int PrimMST(int graph[][SIZE]) {
    int INF = 0x3f3f3f3f;       // A sentinel element represents infinity.
    int min;
    int curr = START_CITY;      // Specify node 0 as the starting node.
    int next = 0;               // Initialize next node.
    int V[SIZE];                // Set V will mark node which is already traversed as 1, otherwise 0.
    memset(V, 0, SIZE * sizeof(int));
    V[curr] = 1;                // Cause node 0 is starting node, it's already traversed.
    int prim[SIZE][SIZE];       // Set prim will store edge which is already traversed as 1, otherwise 0. In fact it's a MST.
    memset(prim, 0, SIZE * SIZE * sizeof(int));
    int cost = 0;               // Record current cost.

    for(int i=0; i<SIZE-1; i++) {   // Generate MST by Prim algorithm, then store the MST as a matrix (Values are still in graph).
        min = INF;              // Initialize min as infinite.
        for(int j=0; j<SIZE; j++) {
            if(V[j] == 0) {     // If j is already traversed or not
                for(int k=0; k<SIZE; k++) {
                    if(V[k] == 1) { // If k is already traversed or not
                        if(graph[j][k] < min) { // Is it a closer path?
                            min = graph[j][k];
                            curr = j;           // Move.
                            next = k;
                        }
                    }
                }
            }
        }
        prim[curr][next] = 1;       // Store the MST as a matrix.
        prim[next][curr] = 1;
        cost += graph[curr][next];  // Calculate new cost.
        V[curr] = 1;                // Mark current node.
    }

    printf("\n>> The approximate optimal path is: %d ", START_CITY+1);
    getHamilton(START_CITY, prim);    // Traverse MST to find a Hamiltonian cycle.
    printf("=> %d ", START_CITY+1);

    printf("\n>> And the cost is: %d", cost);
    return 0;
}

int getHamilton(int i,int prim[][SIZE]) {
    int j;
    for(j=1; j<SIZE; j++) {           // Traverse.
        if(prim[i][j] != 0) {
            prim[i][j] = 0;
            prim[j][i] = 0;
            printf("=> %d ",j+1);
            getHamilton(j, prim); // Recursive.
        }
    }
    return 0;
}