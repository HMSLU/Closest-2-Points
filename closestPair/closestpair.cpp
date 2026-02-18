#include "closestpair.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sys/time.h>  // used to seed srand for extra credit
#include <iterator>
using namespace std;



//comparison function for std::qsort
//int cmp(const void *a, const void *b);
/*
int compX(const void *p,const void *q){
    Point* pp = (Point *)p;
    Point* qq = (Point *)q;

    if (compareByX(*pp,*qq)){ // if qq is bigger than pp, return true
        return 1;
    }else{
        return -1;
    }
}

int compY(const void *p,const void *q){
    Point* pp = (Point *)p;
    Point* qq = (Point *)q;

    if (compareByY(*pp,*qq)){ // if qq is bigger than pp, return true
        return 1;
    }else{
        return -1;
    }
}

*/

void sortX(const vector<Point>& data){
    sort(data.begin(),data.end(),compareByX); //inplace sort by x
}
void sortY(const vector<Point>& data){
    sort(data.begin(),data.end(),compareByY); //inplace sort by y
}

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

Outcome combine(vector<Point>& X_all, Point* buffer, int indexLeft, int indexRight, int xDivI, long long delta_squared,vector<Point>& Y_all) {
    //blah
    int closestLeftPointIndex{-1};
    int closestRightPointIndex{-1};
    while (pow(abs(X_all[indexLeft].x - X_all[xDivI].x), 2) > delta_squared) { // Align iL to the first point on the left that is within the delta region.
        indexLeft++;
    }

    while (pow(abs(X_all[indexRight].x - X_all[xDivI].x), 2) > delta_squared) { // Align iR to the first point on the right that is within the delta region.
        indexRight--;
    }
    /*
    int rPointsWithinDelta = 0;
    int yInDelta = xDivI + 1;
    for (; data[yInDelta].x < data[xDivI].x + delta_squared; yInDelta++) {
        rPointsWithinDelta++;
    } 
    vector<Point>::const_iterator start = data.begin() + xDivI + 1;
    vector<Point>::const_iterator end = data.begin() + xDivI + yInDelta;
    */

    for (int i = 0;i < Y_all.size();i++){ // copy Y points to buffer
        
    }

    for (int i = indexLeft; i <= xDivI; i++) {

        for (int j = 0; j < rPointsWithinDelta; j++) {
            if (buffer[j].y > data[i].y + delta_squared) {
                continue;
            }
            else if (buffer[j].y < data[i].y - delta_squared) {
                break;
            }
            else {

                if (distSquared(data[i],buffer[j]) < delta_squared) {
                    closestLeftPointIndex = i;
                    closestRightPointIndex = j;
                    delta_squared = distSquared(data[i], buffer[j]);

                }

            }
        }

    }
    return Outcome(data[closestLeftPointIndex],buffer[closestRightPointIndex]);
}
Outcome divide(const vector<Point>& X_all,int indexLeft,int indexRight,Point* buffer,vector<Point>& Y_all){
    if((indexRight - indexLeft + 1) <= CUTOFF){
        vector<Point>::const_iterator start = X_all.begin() + indexLeft;
        vector<Point>::const_iterator end = X_all.begin() + indexRight + 1; // +1 because iterator must point 1 past desired last element
        vector<Point> dataSubset(start, end);
        brute(dataSubset);
    }   
    int mid = (indexLeft + indexRight) / 2;

    Outcome lOut = divide(X_all,indexLeft,mid,buffer,Y_all);
    Outcome rOut = divide(X_all,mid + 1,indexRight,buffer,Y_all);

    long long delta_squared = min(lOut.dsq,rOut.dsq);
    Outcome cOut = combine(X_all,buffer,indexLeft,indexRight,mid,delta_squared,Y_all);


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

    vector<Point> X_all = data;
    sortX(X_all);
    vector<Point> Y_all = data;
    sortY(Y_all);

    int leftIndex{0};
    int rightIndex = X_all.size()-1;
    
    Point* buffer = new Point[data.size()];

    divide(X_all,leftIndex,rightIndex,buffer);

    return Outcome();
}
// An extra credit algorithm
Outcome extra(const vector<Point>& data) {
    srand(time(NULL));  // re-seeding random number generator
    return Outcome();
}
