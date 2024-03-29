#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#define  SIZE       358          // The number of cities
#define  IN_LOOP    10000        // The number of each in loop
#define  ALPHA      0.9999       // The cooling coefficient
#define  TF         1            // The ideal temperature
#define  INF        999999       // Maximum value

int SA(int graph[][SIZE]);
// This function will solve TSP via Simulate Anneal Algorithm.

int main(void) {
    FILE *fp = 0;
    int dis = 0;                 // Temporarily store city ID.
    int x = 0;                   // Temporarily store city longitude.
    int y = 0;                   // Temporarily store city latitude.
    int graph[SIZE][SIZE];       // Temporarily store data from file.

    double run_time;
    LARGE_INTEGER start, end, freq;
    double dqFreq;                      // Line 53 to 57 is a tool aims to measure code segment run time exactly.
    QueryPerformanceFrequency(&freq);
    dqFreq=(double)freq.QuadPart;

    if ((fp = fopen(".\\data\\att358.dat", "r")) == NULL) {
        printf("\nCannot open file since it's null.");
        exit(EXIT_FAILURE);     // Please check the data path.
    }
    printf("  Please wait a minute...");

    for (x=0; x<SIZE; x++) {    // Read data from file.
        for (y=0; y<SIZE; y++) {
            fscanf(fp, "%d", &dis);
            graph[x][y] = dis;
        }
    }
    fclose(fp);

    srand((unsigned) time(NULL));   // Generate random number seeds.

    QueryPerformanceCounter(&start);
    SA(graph);  // Solve TSP via Simulate Anneal Algorithm
    QueryPerformanceCounter(&end);

    run_time = (double)(end.QuadPart-start.QuadPart) / dqFreq ;
    printf("\n>> The running time of the algorithm is %lf second(s).", run_time);

    return 0;
}

int SA(int graph[][SIZE]) {
    int k1,k2;  // k1, k2 are the random exchanging index.
    int curr_path[SIZE+1], next_path[SIZE+1]; // Store the path.
    int tmp, curr_cost = INF, next_cost, opt_cost = INF;    // Store the cost.
    double t = 1000000;  // The initial temperature. The higher temperature, the higher probability to obtain the optimal solution.
    double delta;   // The optimization degree of the next solution
    int cnt = 0;    // Counter

    for (int i=0; i<SIZE; i++) {
        curr_path[i] = i;   // initialization
    }
    curr_path[SIZE] = 0;    // Cause the path is a circle, we should add node 0 into the path.

    /* Store all intermediate data in a csv file for visualization. Check Line 114 and Line 117.
    FILE *fp = 0;
    if ((fp = fopen(".\\test.csv", "w")) == NULL) {
        printf("\nCannot open file since it's null.");
        exit(EXIT_FAILURE);
    }
    */

    while (t > TF) {    // If the target cooling temperature is reached or not.
        for (int i=0; i < IN_LOOP; i++) {
            cnt++;      // Count the in loop.

            for (int j=0; j<SIZE+1; j++) {
                next_path[j] = curr_path[j];    // Copy the path to next_path[].
            }

            k1 = rand() % (SIZE - 1) + 1;
            k2 = rand() % (SIZE - 1) + 1;

            tmp = next_path[k1];    // Exchange the two nodes then get a new path.
            next_path[k1] = next_path[k2];
            next_path[k2] = tmp;

            curr_cost = 0;
            next_cost = 0;
            for (int j=0; j<SIZE; j++) {
                curr_cost += graph[ curr_path[j] ][ curr_path[j+1] ];   // Calculate the cost of the current path
                next_cost += graph[ next_path[j] ][ next_path[j+1] ];   // Calculate the cost of the next path
            }

            if (opt_cost > curr_cost) {
                opt_cost = curr_cost;   // Record the best cost in all solutions.
            }

            delta = (double)(next_cost - curr_cost);
            if (delta < 0) {    // If next solution is better than current solution.
                curr_path[k1] = next_path[k1];
                curr_path[k2] = next_path[k2];  // Copy the better path.
                t = ALPHA * t;  // Cool down
            }
            else {  // Accept a worse solution with a probability.
                if ( exp(-delta / t) > (rand()*1.0) / RAND_MAX) {   // Accept function
                    curr_path[k1] = next_path[k1];
                    curr_path[k2] = next_path[k2];
                    t = ALPHA * t;  // Cool down
                }
            }
            //fprintf(fp, "%d,%d\n",cnt, curr_cost);
        }
    }
    //fclose(fp);

    if (opt_cost < curr_cost) { // If the historical optimal solution is better than the current solution.
        curr_cost = opt_cost;
    }

    printf("\nOptimal cost: %d\n", curr_cost);  // Print cost.
    printf("\nLoop_counter: %d", cnt);  // Print in loop count.

    return 0;
}