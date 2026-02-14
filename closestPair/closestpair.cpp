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

#include <iterator>



Outcome conquer(const vector<Point>& data, Point* buffer, int iL, int iR, long long delta) {

}

Outcome divide(const vector<Point>& data, Point* buffer, int iL, int iR) {
    
    if ((iR - iL + 1) <= CUTOFF) {
        // Call brute function
        const_iterator start = data->begin() + iL + 1;
        const_iterator end = data->begin() + iR + 1;
        miniData = data;
        brute();
    }
   

    int xDivI = (iL + iR) / 2;



    Outcome lOut = divide(data, buffer, iL, xDivI);

    Outcome rOut = divide(data, buffer, xDivI+1, iR);

    long long delta = min(lOut.dsq, rOut.dsq);

    return conquer(data, buffer, iL, iR, delta);



}

// The student's implementation of the O(n log n) divide-and-conquer approach
Outcome efficient(const vector<Point>& data) {
    std::cout << "Cutoff " << CUTOFF << " being used." << std::endl;
    
    Point* buffer = new Point[data->size()/2 + 1]; // size/2 + 1 because we plan to never sort the left half y-wise.


    delete[] buffer;
    buffer = nullptr;

    return Outcome();
}

// An extra credit algorithm
Outcome extra(const vector<Point>& data) {
    srand(time(NULL));  // re-seeding random number generator
    return Outcome();
}
