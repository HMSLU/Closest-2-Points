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

Outcome combine(const vector<Point>& data, Point* buffer, int iL, int iR, int xDivI, long long delta);
Outcome divide(const vector<Point>& data, Point* buffer, int iL, int iR);

void sortX(const vector<Point>& data) {
    sort(data.begin(), data.end(), compareByX); //inplace sort by x
}

void sortY(const vector<Point>& data, vector<Point>::const_iterator start, vector<Point>::const_iterator end, Point* buffer) {
    // sort by Y,push values to scratch buffer space.
    int i = 0;
    vector<Point>::const_iterator it;
    for (it = start; it != end; it++) {
        buffer[i] = *it;
        i++;
    }

    sort(buffer, buffer + i + 1, compareByY); //must add one because i is an index, not n, where n = size of the array
}


Outcome combine(const vector<Point>& data, Point* buffer, int iL, int iR, int xDivI, long long delta) {
    int closestLeftPointIndex;
    int closestRightPointIndex;

    while (data[iL].x < data[xDivI].x - delta) {
        iL++;
    }

    while (data[iR].x > data[xDivI].x + delta) {
        iR--;
    }

    int rPointsWithinDelta = 0;
    int yInDelta = xDivI + 1;
    for (; data[yInDelta].x < data[xDivI].x + delta; yInDelta++) {
        rPointsWithinDelta++;
    } 

    vector<Point>::const_iterator start = data.begin() + xDivI + 1;
    vector<Point>::const_iterator end = data.begin() + yInDelta;
    sortY(data, start, end, buffer);


    for (int i = iL; i <= xDivI; i++) {

        for (int j = 0; j < rPointsWithinDelta; j++) {
            if (buffer[j].y > data[i].y + delta) {
                continue;
            }
            else if (buffer[j].y < data[i].y - delta) {
                break;
            }
            else {

                if (distSquared(data[i],buffer[j]) < delta) {
                    closestLeftPointIndex = i;
                    closestRightPointIndex = j;
                    delta = distSquared(data[i], buffer[j]);

                }

            }
        }

    }

    return Outcome(data[closestLeftPointIndex], buffer[closestRightPointIndex]);

}

Outcome divide(const vector<Point>& data, Point* buffer, int iL, int iR) {

    if ((iR - iL + 1) <= CUTOFF) { // Call brute function
        vector<Point>::const_iterator start = data.begin() + iL;
        vector<Point>::const_iterator end = data.begin() + iR + 1; // +1 because iterator must point 1 past desired last element
        vector<Point> dataSubset(start, end);
        return brute(dataSubset);
    }


    int xDivI = (iL + iR) / 2;



    Outcome lOut = divide(data, buffer, iL, xDivI);

    Outcome rOut = divide(data, buffer, xDivI + 1, iR);

    long long delta = min(lOut.dsq, rOut.dsq);

    Outcome cOut = combine(data, buffer, iL, iR, xDivI, delta);

    if (lOut.dsq <= rOut.dsq && lOut.dsq <= cOut.dsq) {
        return lOut;
    }
    else if (rOut.dsq <= lOut.dsq && rOut.dsq <= cOut.dsq) {
        return rOut;
    }
    else {
        return cOut;
    }

}

// The student's implementation of the O(n log n) divide-and-conquer approach
Outcome efficient(const vector<Point>& data) {
    std::cout << "Cutoff " << CUTOFF << " being used." << std::endl;
    Point* buffer = new Point[data.size()/2 + 1]; // size/2 + 1 because we plan to never sort the left half y-wise.

    divide(data, buffer, 0, data.size()-1);

    delete[] buffer;
    buffer = nullptr;

    return Outcome();
}


 

// An extra credit algorithm
Outcome extra(const vector<Point>& data) {
    srand(time(NULL));  // re-seeding random number generator
    return Outcome();
}
