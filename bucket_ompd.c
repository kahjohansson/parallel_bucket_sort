#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define SIZE 100000
#define BUCKETNUM 10
#define NUM_THREADS 4

int compare( const void * n1, const void * n2)
{
    return (*(int*)n1 - *(int*)n2);
}

void insertion_sort(int *arr, int n){
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
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


void bucketSort(int arr[SIZE], int n){
    int max, min;
    minMax(arr,&min,&max);
    float value = (max - min) / (n-1);
    int range = ceil(value);

    int bucket_size = 3*(SIZE/n);
    int b[n][bucket_size];
    int index[n];
    for (int i=0;i<n;i++)
        index[i] = 0;
    
    double max_index = n-2;

    // assign buckets for each element from arr (scatter step)
    for (int i = 0; i < SIZE; i++) {
        int bi = (arr[i] - min) / range; // Index in bucket

        if (bi > max_index){
            bi = max_index;
        }
        b[bi][index[bi]] = arr[i];
        index[bi]++;
    }
    
    // sort the buckets
    
    #pragma omp parallel for schedule(dynamic) num_threads(NUM_THREADS)
    for (int i = 0; i < n; i++)
            // qsort(b[i], index[i],sizeof(int),compare);
            insertion_sort(b[i], index[i]);
    


    // concatenate buckets (gather step)
    int indx = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < index[i]; j++)
            arr[indx++] = b[i][j];
}

int main(){
    srand(time(NULL));
    int arr[SIZE];

    // fill array with random values 
    for(int i=0;i<SIZE;i++){
        arr[i] = rand()%100;
    }

    // printf("Original array:\n");
    // for (int i = 0; i < SIZE; i++)
    //     printf("%d ", arr[i]);
    // printf("\n");

    struct timeval start, end;
    printf("%d\n",NUM_THREADS);
    gettimeofday(&start,0);

    bucketSort(arr, BUCKETNUM);

    gettimeofday(&end,0);

    long sec = end.tv_sec - start.tv_sec;
    long micro = end.tv_usec - start.tv_usec;
    double elapsed = sec + micro*1e-6;
    printf("Elapsed time: %.3f seconds.\n", elapsed);
    
    // printf("Result is: %d\n", ans);

    // printf("\nSorted array:\n");
    // for (int i = 0; i < SIZE; i++)
    //     printf("%d ", arr[i]);
    // printf("\n");
    
    return 0;
    
}
