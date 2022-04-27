#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <time.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
using namespace std;

#define SIZE 50
#define BUCKETNUM 50

// __global__ void SortBuckets(){

// }

int scatter(float arr[SIZE], int n){

    thrust::host_vector<thrust::host_vector<float>> buckets(n);
    float max = *max_element(arr, arr + SIZE);
    float min = *min_element(arr, arr + SIZE);
    float range = (max - min) / (n-1);

    // assign buckets for each element from arr (scatter step)
    for (int i = 0; i < n; i++) {
        int bi = (arr[i] - min) / range; // Index in bucket
        buckets[bi].push_back(arr[i]);
    }

    thrust::device_vector<thrust::device_vector<float>> device_buckets;

    return 0;
}

int main(){

    srand(time(NULL));

    // float *d_arr;

    float arr[SIZE];

    for(int i=0; i<SIZE; i++) {
        arr[i] = rand() % 1000;
    }

    scatter(arr, BUCKETNUM);

    // cudaMalloc(&d_arr, SIZE*sizeof(float));

    // cudaMemcpy(d_arr, arr, SIZE*sizeof(float), cudaMemcpyHostToDevice);

    // BucketSort<<1,1>>();

    // cudaMemcpy(arr, d_arr, SIZE*sizeof(float), cudaMemcpyDeviceToHost);

    // cudaFree(d_arr);

}