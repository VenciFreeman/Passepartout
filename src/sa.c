#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#define  nint(x)    (int)(x+0.5) // nint() aims to do rounding of variable x.
#define  SIZE       48           // The number of cities.
#define  LOOPCNT    9999999

int solveTSP(int matrix[][3]);
// This function will figure out the adjacency matrix, and use it to do the following operation.
int SA(int graph[][SIZE]);

int main(void) {
    FILE *fp = 0;
    int id = 0;                 // Temporarily store city ID.
    int x = 0;                  // Temporarily store city longitude.
    int y = 0;                  // Temporarily store city latitude.

    int matrix[SIZE][3];        // Temporarily store data from file.

    if ((fp = fopen(".\\src\\usa48.dat", "r")) == NULL) {
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

    srand((unsigned) time(NULL));

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
    SA(graph);                     // Generate a MST then traverse it to find a Hamiltonian cycle.
    QueryPerformanceCounter(&end);

    run_time = (double)(end.QuadPart-start.QuadPart) / dqFreq ;
    printf("\n>> The running time of the algorithm is %lf second(s).", run_time);
    return 0;
}

int SA(int graph[][SIZE]) {
    int k1,k2;
    int curr_path[SIZE+1], next_path[SIZE+1];
    int tmp, curr_cost = -1, next_cost;
    double t = 100000;
    double tf = 5;
    int delta;
    int cnt = 0;
    double p;

    for (int i=0; i<SIZE; i++) {
        curr_path[i] = i;
    }
    curr_path[SIZE] = 0;

    while (t > tf) {
        for (int i=0; i < LOOPCNT; i++) {
            cnt++;

            for (int j=0; j<SIZE+1; j++) {
                next_path[j] = curr_path[j];
            }
            k1 = rand() % 47 + 1;
            k2 = rand() % 47 + 1;
            tmp = next_path[k1];
            next_path[k1] = next_path[k2];
            next_path[k2] = tmp;

            curr_cost = 0;
            next_cost = 0;
            for (int j=0; j<SIZE; j++) {
                curr_cost += graph[ curr_path[j] ][ curr_path[j+1] ];
                next_cost += graph[ next_path[j] ][ next_path[j+1] ];
            }

            delta = next_cost - curr_cost;
            if (delta < 0) {
                curr_path[k1] = next_path[k1];
                curr_path[k2] = next_path[k2];
                t = 0.99*t;
            }
            else {
                p = rand()*1.0/1000;
                if (exp((double)(-delta) / t) > p) {
                    printf("\n%f > %f",exp((double)(-delta) / t), p);
                    curr_path[k1] = next_path[k1];
                    curr_path[k2] = next_path[k2];
                    t = 0.99*t;
                    //printf("\n%d",delta);
                }
            }
            //printf("\ntest:%d", cnt);
            //printf(" %d, %d, %d", curr_cost, next_cost, delta);
        }

        if (cnt == LOOPCNT - 1)
            break;
    }

    printf("\nOptimal cost: %d", curr_cost);
    printf("\nLoop_counter: %d", cnt);

    return 0;
}