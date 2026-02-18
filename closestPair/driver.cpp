#include <cmath>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>
#include "point.h"
#include "closestpair.h"
#include <fstream>
#include <string>

using namespace std;

#define GRIDSIZE 40000001

void usage() {
    std::cout << "Usage: driver A N [S]" << endl;
    std::cout << "  A=0 brute, A=1 divide/conquer, A=2 extra" << endl;
    std::cout << "  N=number of points (N >= 2)" << endl;
    std::cout << "  S=integer random seed (optional)" << endl;
    exit(-1);
}


void validate(const vector<Point>& data, const Point& p) {
    bool found = false;
    for (unsigned int j=0; j < data.size(); j++) {
        if (data[j].x == p.x && data[j].y == p.y) {
            found = true;
            break;
        }
    }

    if (!found)
        std::cout << "ERROR: reported point " << p.x << "," << p.y << " not in original data set" << endl;
}

vector<Point> randomizeUniform(int N, long seed) {
    vector<Point> data(N);
    unordered_set<Point> used;
    mt19937 mt_rand(seed);
    
    for (int j=0; j < N; j++) {
        do {
            data[j].x = mt_rand() % GRIDSIZE;
            data[j].y = mt_rand() % GRIDSIZE;
        } while (used.find(data[j]) != used.end());
        used.insert(data[j]);
    }
    
    return data;
}

vector<Point> randomizeCluster(int N, long seed) {
    mt19937 mt_rand(seed);
    int K(max(sqrt(N),16.0));           // number of clusters
    int W(GRIDSIZE/K);        // width of one cluster 
    int H=4*W;                // intentionally vertical clusters
    vector<Point> centers(K);

    for (int j=0; j < K; j++) {
        centers[j].x = W + mt_rand() % (GRIDSIZE-2*W);
        centers[j].y = H + mt_rand() % (GRIDSIZE-2*H);
    }
    
    vector<Point> data(N);
    unordered_set<Point> used;
    for (int j=0; j < N; j++) {
        do {
            int c = mt_rand() % K;   // first, pick one cluster to use
            data[j].x = centers[c].x - W + mt_rand() % (2*W);
            data[j].y = centers[c].y - H + mt_rand() % (2*H);
        } while (used.find(data[j]) != used.end());
        used.insert(data[j]);
    }
    
    return data;
}

#include "globals.h"

#define NUM_TRIALS 5
#define ALTERED true
#define FILE_NAME "data.csv"

#define DATA_SIZE_MIN 10
#define DATA_SIZE_MAX 1000
#define DATA_SIZE_GROWTH dataSize *= 10

#define CUTOFF_MIN 3
#define CUTOFF_MAX 12
#define CUTOFF_GROWTH Cutoff *= 2

int Cutoff = CUTOFF_MIN;

int main(int argc, char* argv[]) {
    if (argc < 3)
        usage();
    int A = atoi(argv[1]);
    if (A < 0 || A > 2)
        usage();
    int N = atoi(argv[2]);
    if (N < 2)
        usage();
    long S;
    if (argc > 3) 
        S = atol(argv[3]);
    else {
        S = time(NULL);
        std::cout << "Seed = " << S << endl;
    }

    if (ALTERED) {



        ofstream outFile(FILE_NAME, std::ofstream::out | std::ofstream::trunc);
        outFile << "Cutoff,Data Size,Average Time (ms),# Trials" << endl;

        for (Cutoff = CUTOFF_MIN; Cutoff < CUTOFF_MAX; CUTOFF_GROWTH) {

            for (int dataSize = DATA_SIZE_MIN; dataSize <= DATA_SIZE_MAX; DATA_SIZE_GROWTH) {

                long double totalTime_ms = 0;
                for (int trials = 0; trials < NUM_TRIALS; trials++) {
                    S = time(NULL);

                    vector<Point> data;
                    data = randomizeCluster(dataSize, S);
                    std::cout << "Done generating data." << endl;

                    Outcome ans;
                    auto begin = chrono::steady_clock::now();
                    switch (A) {
                    case 0:
                        ans = brute(data);
                        break;
                    case 1:
                        ans = efficient(data);
                        break;
                    case 2:
                        ans = extra(data);
                        break;
                    }
                    auto end = chrono::steady_clock::now();

                    totalTime_ms += chrono::duration<double, milli>(end - begin).count();

                }

                outFile << Cutoff << "," << dataSize << "," << totalTime_ms / NUM_TRIALS << "," << NUM_TRIALS << endl;
            }

        }

        outFile.close();

        return 0;
    }
    




    vector<Point> data;
    data = randomizeCluster(N,S);
    std::cout << "Done generating data." << endl;

    Outcome ans;
    auto begin = chrono::steady_clock::now();
    switch (A) {
    case 0:
        ans = brute(data);
        break;
    case 1:
        ans = efficient(data);
        break;
    case 2:
        ans = extra(data);
        break;
    }
    auto end = chrono::steady_clock::now();

    std::cout << "minimum distance squared reported as " << ans.dsq << endl;
    std::cout << "achieved by points " << ans.p.x << "," << ans.p.y << " and " << ans.q.x << "," << ans.q.y << endl;
    std::cout << "elapsed time: " << chrono::duration<double, milli> (end-begin).count() << " ms" << endl;

    validate(data, ans.p);
    validate(data, ans.q);
}


