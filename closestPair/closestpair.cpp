#include "closestpair.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sys/time.h>  // used to seed srand for extra credit
using namespace std;

// For convenience, we provide a brute-force implementation that can be applied
// to any portion of a larger sequence by providing start and stop iterators.
template <typename IT>
Outcome bruteUtility(IT start, IT stop) {
    IT second = start;
    ++second;
    Outcome best(*start, *second, distSquared(*start, *second));
    for (IT k=start; k != stop; ++k)
        for (IT j=start; j != k; ++j) {
            long long temp = distSquared(*j, *k);
            if (temp < best.dsq) {
                best = Outcome(*j, *k, temp);
            }
        }
    return best;
}

// The public facing brute-force function that takes a vector as input
Outcome brute(const vector<Point>& data) {
    return bruteUtility(data.begin(), data.end());
}

/*------------------------------------------------------------
| Here we define a cutoff value at which the divide-and-conquer
| approach should resort to a base-case in which brute-force is applied.
| You are free to change this cutoff to optimize, but please make sure
| that the logic of your implementation relies on this defined CUTOFF constant.
--------------------------------------------------------------*/
#ifndef CUTOFF
#define CUTOFF 3          // you may change this or use the makefile to redefine the value
#endif

//int splitEqualXs(const vector<Point>& data, int xDivI) {
//    long midpointX = (data->at(xDivI).x);
//    int i, j = xDivI;
//    
//    while (i > 0 && (data->at(xDivI - 1)).x == midpointX) {
//        i--;
//    }
//    while (j < data->size()-1 && (data->at(xDivI+1)).x == midpointX) {
//        j++;
//    }
//
//}

Outcome conquer(const vector<Point>& data, int iL, int iR, long long delta) {

}

Outcome divide(const vector<Point>& data, int iL, int iR) {
    

    
    int xDivI = (iL + iR) / 2;

    if ((iR - iL + 1) <= CUTOFF) {
        // Call brute function
        //float xDivI; // Use a float to safeguard against the case that no points are assigned to a side.
    }

    Outcome lOut = divide(data, iL, xDivI);

    Outcome rOut = divide(data, xDivI+1, iR);

    long long delta = min(lOut.dsq, rOut.dsq);

    return conquer(data, iL, iR, delta);



}

// The student's implementation of the O(n log n) divide-and-conquer approach
Outcome efficient(const vector<Point>& data) {
    std::cout << "Cutoff " << CUTOFF << " being used." << std::endl;
    
    return Outcome();
}

// An extra credit algorithm
Outcome extra(const vector<Point>& data) {
    srand(time(NULL));  // re-seeding random number generator
    return Outcome();
}
