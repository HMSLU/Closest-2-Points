#include "closestpair.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sys/time.h>  // used to seed srand for extra credit
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


// The student's implementation of the O(n log n) divide-and-conquer approach
Outcome efficient(const vector<Point>& data) {
    std::cout << "Cutoff " << CUTOFF << " being used." << std::endl;
    //qsort(data.data(),data.size(),);

    sort(data.begin(),data.end(),compareByX); //inplace sort by x

    int leftIndex{0};
    int rightIndex{data.size()-1};

    divide(data,leftIndex,rightIndex);


    return Outcome();
}

Outcome divide(const vector<Point>& data,int indexLeft,int indexRight){
    if((indexRight - indexLeft + 1) <= CUTOFF){
        brute(data);
    }
    int mid = (indexLeft + indexRight) / 2;

    Outcome lOut = divide(data,indexLeft,mid);
    Outcome rOut = divide(data,mid + 1,indexRight);

    long long delta = min(lOut.dsq,rOut.dsq);

}

Outcome conquer(){}

// An extra credit algorithm
Outcome extra(const vector<Point>& data) {
    srand(time(NULL));  // re-seeding random number generator
    return Outcome();
}
