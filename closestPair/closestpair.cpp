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


Outcome combine(vector<Point>* X_list, vector<Point>* Y_list, Point* buffer, const long long xDivI, const long long deltaSquaredOriginal ) {
    long long unsigned int X_listSize = X_list->size();
    long long unsigned int Y_listSize = Y_list->size();

    long long unsigned int buffLeft = 0;
    long long unsigned int buffRight = X_listSize - 1;

    long long deltaSquared = deltaSquaredOriginal;

    cout << "Starting y-distribution." << endl;
    for (long long unsigned int i = 0; i < Y_listSize; i++) { // Distribute all points within delta region to left and right lists
        if (Y_list->at(i).x - X_list->at(xDivI).x <= 0 && (Y_list->at(i).x - X_list->at(xDivI).x) * (Y_list->at(i).x - X_list->at(xDivI).x) < deltaSquared) {
            buffer[buffLeft] = Y_list->at(i);
            buffLeft++;
        }
        else if (Y_list->at(i).x - X_list->at(xDivI).x > 0 && (Y_list->at(i).x - X_list->at(xDivI).x)*(Y_list->at(i).x - X_list->at(xDivI).x) < deltaSquared) {
            buffer[buffRight] = Y_list->at(i);
            buffRight--;
        }
    }
    cout << "ending y-distribution." << endl;

    long long unsigned int leftPointI = 0;
    long long unsigned int rightPointI = Y_listSize-1;

    cout << "left, right indeces: " << leftPointI << ", " << rightPointI << endl;

    for (long long unsigned int i = 0; i < buffLeft; i++) { // Compare all left points to right points 
        // cout << "Looking at index " << i << " for left." << endl;
        for (long long unsigned int j = Y_listSize - 1; j > buffRight; j--) {
            // cout << "Looking at index " << j << " for right." << endl;
            if (buffer[i].y - buffer[j].y < 0 && (buffer[i].y - buffer[j].y)*(buffer[i].y - buffer[j].y) > deltaSquaredOriginal) {
                continue;
            }
            else if (buffer[i].y - buffer[j].y >= 0 && (buffer[i].y - buffer[j].y)*(buffer[i].y - buffer[j].y) > deltaSquaredOriginal) {
                break;
            }
            else if ((buffer[i].y - buffer[j].y)*(buffer[i].y - buffer[j].y) <= deltaSquaredOriginal) {

                if (distSquared(buffer[i],buffer[j]) < deltaSquared) {
                    deltaSquared = distSquared(buffer[i], buffer[j]);
                    leftPointI = i;
                    rightPointI = j;
                }

            }

        }

    }
    if (buffer[leftPointI] == buffer[rightPointI]) {
        cout << "ERROR: Same point chosen for p1 and p2!" << endl;
    }

    if (leftPointI == 0 || rightPointI == Y_listSize - 1) {
        Point dummy1{ 0,0 };
        Point dummy2{ 0,0 };

        return Outcome(dummy1, dummy2, deltaSquaredOriginal + 1);
    }

    return Outcome(buffer[leftPointI], buffer[rightPointI]);

}

Outcome divide(vector<Point>* X_data,vector<Point>* Y_data, Point* buffer, long long iL, long long iR) {

    if ((iR - iL + 1) <= CUTOFF) { // Call brute function
        vector<Point>::iterator start = X_data->begin() + iL;
        vector<Point>::iterator end = X_data->begin() + iR + 1; // +1 because end iterator must point 1 past desired last element
        vector<Point> dataSubset(start, end);

        cout << "Data subset size: " << dataSubset.size() << endl;
        cout << "Data subset: ";
        for (unsigned long long t = 0; t < dataSubset.size(); t++) {
            cout << dataSubset[t].x << ", " << dataSubset[t].y << " | ";
        }
        cout << endl;

        return brute(dataSubset);
    }


    long long xDivI = (iL + iR) / 2;



    Outcome lOut = divide(X_data, Y_data, buffer, iL, xDivI);

    Outcome rOut = divide(X_data, Y_data, buffer, xDivI + 1, iR);

    long long deltaSquared = min(lOut.dsq, rOut.dsq);

    Outcome cOut = combine(X_data, Y_data, buffer, xDivI, deltaSquared);


    if (lOut.dsq == 0) {
        cout << "ERROR: lOut.dsq = " << lOut.dsq << ". lOut p1 : " << lOut.p.x << ", " << lOut.p.y << " | lOut.p2 : " << lOut.q.x << ", " << lOut.q.y << endl;
        exit(-2);
    }
    else if (rOut.dsq == 0) {
        cout << "ERROR: rOut.dsq = " << rOut.dsq << ". rOut p1 : " << rOut.p.x << ", " << rOut.p.y << " | rOut.p2 : " << rOut.q.x << ", " << rOut.q.y << endl;
        exit(-2);
    }
    else if (cOut.dsq == 0) {
        cout << "ERROR: cOut.dsq = " << cOut.dsq << ". cOut p1 : " << cOut.p.x << ", " << cOut.p.y << " | cOut.p2 : " << cOut.q.x << ", " << cOut.q.y << endl;
        exit(-2);
    }



    if (min({ lOut.dsq, rOut.dsq, cOut.dsq }) == lOut.dsq) {
        cout << "Returning lOut! | " << lOut.dsq << endl;
        return lOut;
    }
    else if (min({ lOut.dsq, rOut.dsq, cOut.dsq }) == rOut.dsq) {
        cout << "Returning rOut! | " << rOut.dsq << endl;
        return rOut;
    }
    else if (min({ lOut.dsq, rOut.dsq, cOut.dsq }) == cOut.dsq) {
        cout << "Returning cOut! | " << cOut.dsq << endl;
        return cOut;
    }


    cout << "Error in divide: No outcome." << endl;
    exit(-1);

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
