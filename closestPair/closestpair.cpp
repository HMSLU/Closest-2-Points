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

Outcome combine(const vector<Point>& data, Point* buffer, int iL, int iR, int xDivI, long long delta);
Outcome divide(const vector<Point>& data, Point* buffer, int iL, int iR);

void sortX(vector<Point>& data) {
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


Outcome combine(vector<Point>* X_list, Point* buffer, int xDivI, long long deltaSquared, vector<Point>* Y_list /*Sorted high point to low*/) {
    
    int buffLeft = 0;
    int buffRight = X_list->size() - 1;

    for (int i = 0; i < Y_list->size(); i++) { // Distribute all points within delta region to left and right lists
        if (Y_list->at(i).x - X_list->at(xDivI).x < 0 && pow(Y_list->at(i).x - X_list->at(xDivI).x,2) < deltaSquared) {
            buffer[buffLeft] = Y_list->at(i);
            buffLeft++;
        }
        else if (Y_list->at(i).x - X_list->at(xDivI).x >= 0 && pow(Y_list->at(i).x - X_list->at(xDivI).x, 2) < deltaSquared) {
            buffer[buffRight] = Y_list->at(i);
            buffRight--;
        }
    }

    int leftPointI = 0;
    int rightPointI = Y_list->size()-1;

    for (int i = 0; i < buffLeft; i++) { // Compare all left points to right points 

        for (int j = Y_list->size() - 1; j > buffRight; j--) {

            if (buffer[i].y - buffer[j].y < 0 && pow(buffer[i].y - buffer[j].y, 2) > deltaSquared) {
                continue;
            }
            else if (buffer[i].y - buffer[j].y >= 0 && pow(buffer[i].y - buffer[j].y, 2) > deltaSquared) {
                break;
            }
            else if (pow(buffer[i].y - buffer[j].y, 2) < deltaSquared) {

                if (distSquared(buffer[i],buffer[j]) < deltaSquared) {
                    deltaSquared = distSquared(buffer[i], buffer[j]);
                    leftPointI = i;
                    rightPointI = j;
                }

            }

        }

    }
    
    return Outcome(buffer[leftPointI], buffer[rightPointI]);

}

Outcome divide(vector<Point>& data, Point* buffer, int iL, int iR) {

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


    if (min(lOut.dsq, rOut.dsq, cOut.dsq) == lOut.dsq) {
        return lOut;
    }
    else if (min(lOut.dsq, rOut.dsq, cOut.dsq) == rOut.dsq) {
        return rOut;
    }
    else if (min(lOut.dsq, rOut.dsq, cOut.dsq) == cOut.dsq){
        return cOut;
    }


    cout << "Error in divide: No outcome." << endl;
    exit(-1);

}

// The student's implementation of the O(n log n) divide-and-conquer approach
Outcome efficient(const vector<Point>& data) {
    std::cout << "Cutoff " << CUTOFF << " being used." << std::endl;
    vector<Point> workableData = data;
    sortX(workableData);

    Point* buffer = new Point[data.size()/2 + 1]; // size/2 + 1 because we plan to never sort the left half y-wise.

    Outcome final = divide(workableData, buffer, 0, data.size()-1);

    delete[] buffer;
    buffer = nullptr;

    return final;
}


 

// An extra credit algorithm
Outcome extra(const vector<Point>& data) {
    srand(time(NULL));  // re-seeding random number generator
    return Outcome();
}
