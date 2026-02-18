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
#include <cmath>

Outcome combine(vector<Point>* X_list, vector<Point>* Y_list, Point* buffer, long long xDivI, long long deltaSquared);
Outcome divide(vector<Point>* X_data, vector<Point>* Y_data, Point* buffer, long long iL, long long iR);

void sortX(vector<Point>& data) {
    sort(data.begin(), data.end(), compareByX); //inplace sort by x
}

void sortY(vector<Point>& data) {
    sort(data.begin(), data.end(), compareByY); //inplace sort by y
}


Outcome combine(vector<Point>* X_list,
                vector<Point>* Y_list,
                Point* buffer,
                long long xDivI,
                long long deltaSquared)
{
    long long midX = (*X_list)[xDivI].x;

    int stripSize = 0;

    // Build strip (already sorted by y)
    for (int i = 0; i < Y_list->size(); i++) {
        long long dx = (*Y_list)[i].x - midX;
        if (dx * dx < deltaSquared) {
            buffer[stripSize++] = (*Y_list)[i];
        }
    }

    Outcome best;
    best.dsq = deltaSquared;

    for (int i = 0; i < stripSize; i++) {
        for (int j = i + 1; j < stripSize; j++) {
            long long dy = buffer[j].y - buffer[i].y;

            if (dy * dy >= best.dsq)
                break;

            long long d = distSquared(buffer[i], buffer[j]);
            if (d < best.dsq) {
                best = Outcome(buffer[i], buffer[j], d);
            }
        }
    }

    return best;
}


/*
Outcome divide(vector<Point>* X_data,vector<Point>* Y_data, Point* buffer, long long iL, long long iR) {

    if ((iR - iL + 1) <= CUTOFF) { // Call brute function
        auto start = X_data->begin() + iL;
        auto end   = X_data->begin() + iR + 1;
        return bruteUtility(start, end);
;
    }


    long long xDivI = (iL + iR) / 2;



    Outcome lOut = divide(X_data, Y_data, buffer, iL, xDivI);

    Outcome rOut = divide(X_data, Y_data, buffer, xDivI + 1, iR);

    long long deltaSquared = min(lOut.dsq, rOut.dsq);

    Outcome cOut = combine(X_data, Y_data, buffer, xDivI, deltaSquared);


    if (min({ lOut.dsq, rOut.dsq, cOut.dsq }) == lOut.dsq) {
        //cout << "Returning lOut! | " << lOut.dsq << endl;
        return lOut;
    }
    else if (min({ lOut.dsq, rOut.dsq, cOut.dsq }) == rOut.dsq) {
        //cout << "Returning rOut! | " << rOut.dsq << endl;
        return rOut;
    }
    else if (min({ lOut.dsq, rOut.dsq, cOut.dsq }) == cOut.dsq) {
        //cout << "Returning cOut! | " << cOut.dsq << endl;
        return cOut;
    }


    cout << "Error in divide: No outcome." << endl;
    exit(-1);

}
*/
Outcome divide(vector<Point>* X_data,
               vector<Point>* Y_data,
               Point* buffer,
               long long iL,
               long long iR)
{
    if ((iR - iL + 1) <= CUTOFF) {
        auto start = X_data->begin() + iL;
        auto end   = X_data->begin() + iR + 1;
        return bruteUtility(start, end);
    }

    long long xDivI = (iL + iR) / 2;
    long long midX  = (*X_data)[xDivI].x;

    // --- Split Y into left and right ---
    vector<Point> Y_left;
    vector<Point> Y_right;

    Y_left.reserve(xDivI - iL + 1);
    Y_right.reserve(iR - xDivI);

    for (const Point& p : *Y_data) {
        if (p.x <= midX)
            Y_left.push_back(p);
        else
            Y_right.push_back(p);
    }

    // --- Recurse ---
    Outcome lOut = divide(X_data, &Y_left, buffer, iL, xDivI);
    Outcome rOut = divide(X_data, &Y_right, buffer, xDivI + 1, iR);

    long long deltaSquared = min(lOut.dsq, rOut.dsq);

    // --- Combine ---
    Outcome cOut = combine(X_data, Y_data, buffer, xDivI, deltaSquared);

    if (lOut.dsq <= rOut.dsq && lOut.dsq <= cOut.dsq)
        return lOut;
    if (rOut.dsq <= lOut.dsq && rOut.dsq <= cOut.dsq)
        return rOut;

    return cOut;
}

// The student's implementation of the O(n log n) divide-and-conquer approach
Outcome efficient(const vector<Point>& data) {
    std::cout << "Cutoff " << CUTOFF << " being used." << std::endl;
    vector<Point> X_data = data;
    vector<Point> Y_data = data;
    sortX(X_data);
    sortY(Y_data);


    Point* buffer = new Point[data.size()];

    Outcome final = divide(&X_data, &Y_data, buffer, 0, data.size()-1);

    delete[] buffer;
    buffer = nullptr;

    return final;
}
 
// An extra credit algorithm
Outcome extra(const vector<Point>& data) {
    srand(time(NULL));  // re-seeding random number generator
    return Outcome();
}
