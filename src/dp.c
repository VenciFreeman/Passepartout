#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define min(a,b) ((a>b) ? b : a)  // Use macro to implement min function.

short size;  // Use a global variable to represent the size of the TSP.
int solve_tsp(short graph[][size]);

int main(void) {
    FILE *fp = 0;
    int row = 0;
    int col = 0;
    short var = 0;
    double time;
    clock_t start, end;

    printf(">> Which size TSP do you need to solve? Please enter the num of cities <7, 20, 29>: ");
    scanf("%hd", &size);
    short graph[size][size];
    switch(size) {
        case 7:
            if ((fp = fopen(".\\data\\att7.txt", "r")) == NULL) {
                printf("\nCannot open file since it's null.");
                exit(EXIT_FAILURE);
            }
            break;
        case 20:
            if ((fp = fopen(".\\data\\att20.txt", "r")) == NULL) {
                printf("\nCannot open file since it's null.");
                exit(EXIT_FAILURE);
            }
            break;
        case 29:
            if ((fp = fopen(".\\data\\bays29.txt", "r")) == NULL) {
                printf("\nCannot open file since it's null.");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            printf("\nIllegal input value.");
            return -1;
    }
    printf("  Please wait a minute...");
    for (int i=0; i<size; i++) {
        graph[i][i]=0;
    }
    for (int count=0; count<size*(size-1); count++) {
        fscanf(fp, "%d", &row);
        fscanf(fp, "%d", &col);
        fscanf(fp, "%hd", &var);
        graph[row][col] = var;
    }

    fclose(fp);
    start = clock();
    solve_tsp(graph);
    end = clock();
    time = (double)(end - start) /  CLOCKS_PER_SEC;
    printf("\n>> The running time of the algorithm is %lf second(s).", time);
    return 0;
}

int solve_tsp(short graph[][size]) {
    int INF = 0x3f3f3f3f;  // A sentinel element represents infinity.
    int V = (1 << size);   // Count of states. (1 << size) means 2^(size) status.
    short dp[size][V];       // dp[i][V'], i means current reached city node and V' stores set by bits.
    memset(dp, INF, size * V * sizeof(short));
    int opt = INF;         // Store current optimal cost.
    short path[size][V];
    memset(path, INF, size * V * sizeof(short));
    dp[0][1] = 0;          // Initial states is begin at node 0, all nodes haven't been reached.
    path[0][1] = 0;        // The start and the end is both 0. At initial status, the distance between 0 to 0 is 0.
    int i;
    short j,k;// Loop variable
    int cnt1 = 0;
    int cnt2 = 0;
    int pos = -1;
    int pre;

    for (i = 0; i < size; i++)
        dp[i][1 << i] = graph[i][0];      // Initialize the dynamic programming array/

    for (i = 0; i < V; i++) {             // Traversing all situations.
        for (j = size - 1; j > 0; j--) {  // Choose the next city node who join the set
            cnt2++;
            for (k = 1; k < size; k++) {  // Try each node in V' and find optimal solution.
                if ((dp[k][i & (~(1 << j))] != INF) // Is there already a value?
                    && ((i >> k) & 1)     // Is k in the set V'?
                    && ((i >> j) & 1)) {  // Is j in the set V'?
                    if(dp[j][i] > dp[k][i&(~(1<<j))] + graph[j][k]) {
                        path[j][i] = k;
                    }
                    dp[j][i] = min(dp[j][i], dp[k][i & (~(1 << j))] + graph[j][k]); // Recursive equation
                    cnt1++;
                }  // end if
            }  // end k
        }  // end j
    }  // end i
    for (i = 0; i < size; i++) {
        if(opt > dp[i][V - 2] + graph[0][i]){
            pos = i;
        }
        opt = min(opt, dp[i][V - 2] + graph[0][i]);
    }

    printf("\n>> The cost of optimal path is %d;", opt);
    printf("\n>> The optimal path is: ");
    printf("0 ==> ");
    for(i=V-2; i>1;i ^= (1 << pre)){
        printf("%d ==> ", pos);
        pre = pos;
        pos = path[pos][i];
    }
    printf("0;");
    printf("\n>> The num of subproblems is: %d;", cnt1);
    printf("\n>> The num of non-repetitive subproblems is: %d;", cnt2);
    return 0;
}