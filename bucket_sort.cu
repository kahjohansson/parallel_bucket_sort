#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define SIZE 200
#define BUCKETNUM 10

__global__ void insertionSort(int *b, int *index, int bucket_size, int n_buckets){
    
    int thread_id = blockIdx.x * blockDim.x + threadIdx.x;

    if (thread_id <= n_buckets){

        int i, key, j;
        for (i = thread_id*bucket_size+1; i <= thread_id*bucket_size+index[thread_id]; i++) {
            key = b[i];
            j = i - 1;
    
            while (j >= thread_id*bucket_size && b[j] > key) {
                b[j + 1] = b[j];
                j = j - 1;
            }
            b[j + 1] = key;
        }

    }

}

void bucketSort(int arr[SIZE],int n){
    int max,min;
    minMax(arr,&min,&max);
    float value = (max - min) / (n-1);
    int range = ceil(value);

    int bucket_size = 3*(SIZE/n);
    int *b = (int*) malloc(n*bucket_size*sizeof(int));
    int *index = (int*) malloc(n*sizeof(int));
    for (int i=0;i<n;i++)
        index[i] = 0;
    
    double max_index = n-1;

    for (int i = 0; i < SIZE; i++) {
        int bi = (arr[i] - min) / range; // Index in bucket

        if (bi > max_index){
            bi = max_index;
        }
        b[bi*bucket_size+index[bi]] = arr[i];
        index[bi]++;
    }

    // sort the buckets cuda 
    int num_threads_per_block = 512;
    int num_blocks = ceil(n/num_threads_per_block);

    int *d_b;
    int *d_index;

    cudaMalloc(&d_b, n*bucket_size*sizeof(int));
    cudaMalloc(&d_index, n*sizeof(int));
    cudaMemcpy(d_b,b,n*bucket_size*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(d_index, index, n*sizeof(int), cudaMemcpyHostToDevice);

    insertionSort<<<num_blocks,num_threads_per_block>>>(d_b, d_index, bucket_size, n);

    cudaDeviceSynchronize();

    free(b);
    free(index);
    cudaFree(d_b);
    cudaFree(d_index);

}

float minMax(int array[SIZE], int *min, int *max)
{
  int length;
  length = SIZE;
  int i = 1;
  *max = array[0];
  *min = array[0];

  for (i; i<length; i++) {
    if (*max < array[i])
      *max = array[i];

    if (*min > array[i])
      *min = array[i];
  }
 }
 
int main(){

    srand(time(NULL));
    int *arr = (int*) malloc(SIZE * sizeof(int));

    // fill array with random values 
    for(int i=0;i<SIZE;i++){
        arr[i] = rand()%100;
    }

    printf("Original array:\n");
    for (int i = 0; i < SIZE; i++)
        printf("%d ", arr[i]);
    printf("\n");

    bucketSort<<>>(arr, BUCKETNUM);

    printf("\nSorted array:\n");
    for (int i = 0; i < SIZE; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
