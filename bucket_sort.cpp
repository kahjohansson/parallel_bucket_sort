#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <time.h>
using namespace std;

#define SIZE 50
#define BUCKETNUM 50

void BucketSort(float arr[SIZE], int n){

    vector<float> b[n];
    float max = *max_element(arr, arr + SIZE);
    float min = *min_element(arr, arr + SIZE);
    float range = (max - min) / (n-1);
    for (int i = 0; i < n; i++) {
        int bi = (arr[i] - min) / range; // Index in bucket
        b[bi].push_back(arr[i]);
    }

    for (int i = 0; i < n; i++)
        sort(b[i].begin(), b[i].end());

    int index = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < b[i].size(); j++)
            arr[index++] = b[i][j];
}

int main(){
    srand(time(NULL));
    float arr[SIZE];

    for(int i=0;i<SIZE;i++){
        arr[i] = rand();
    }

    cout << "Original array is: ";
    for (int i = 0; i < SIZE; i++)
        cout << arr[i] << " ";

    BucketSort(arr, BUCKETNUM);

    cout << "\n ************************************\n";
    cout << "\n Sorted array is: ";
    for (int i = 0; i < SIZE; i++)
        cout << arr[i] << " ";
    
    return 0;
    
}
