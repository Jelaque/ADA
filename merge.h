
#include <cstdlib>
#include <algorithm>
#include <iostream>
using namespace std;
typedef double carr;
void CopyArray(carr A[], int iBegin, int iEnd, carr B[])
{
    for(int k = iBegin; k < iEnd; k++)
        B[k] = A[k];
}

void TopDownMerge(carr A[], int iBegin, int iMiddle, int iEnd, carr B[])
{
    int i = iBegin, j = iMiddle;
    for (int k = iBegin; k < iEnd; k++) {
        if (i < iMiddle && (j >= iEnd || A[i] <= A[j])) {
            B[k] = A[i];
            i = i + 1;
        } else {
            B[k] = A[j];
            j = j + 1;
        }
    }
}

void TopDownSplitMerge(carr B[], int iBegin, int iEnd,carr  A[])
{
    if(iEnd - iBegin < 2)
        return;

    int iMiddle = (iEnd + iBegin) / 2;
    TopDownSplitMerge(A, iBegin,  iMiddle, B);
    TopDownSplitMerge(A, iMiddle,    iEnd, B);
    TopDownMerge(B, iBegin, iMiddle, iEnd, A);
}



void MergeSort(carr A[], carr B[], int n)
{
    CopyArray(A, 0, n, B);
    TopDownSplitMerge(B, 0, n, A);
}
