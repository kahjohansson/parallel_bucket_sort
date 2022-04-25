#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SIZE 50
#define BUCKETNUM 50

int cmpfunc (const void * a, const void * b)
{
 return ( *(float*)a - *(float*)b );
}


float minMax(float array[SIZE],float *min,float *max)
{
  int length;
  length = SIZE;
  int i = 1;
  *max = array[0];
  *min = array[0];

  for(i; i<length; i++)
  {
    if(*max < array[i])
    {
      *max = array[i];
    }

    if(*min > array[i])
    {
      *min = array[i];
    }
  }
  printf("min: %f max: %f", *min,*max);
 }  


void bucketSort(float arr[SIZE], int n){
    int bucket_size = 3*(SIZE/n);
    float b[n][bucket_size];
    int index[n];
    for(int i=0;i<n;i++){
        index[i] = 0;
    }
    float max, min;
    minMax(arr,&min,&max);
    float value = (max - min) / (n-1);
    int range = ceil(value);
    double max_index = n-2;

    // // assign buckets for each element from arr (scatter step)
    for (int i = 0; i < n; i++) {
        int bi = (arr[i] - min) / range; // Index in bucket

        if (bi > max_index){
            bi = max_index;
        }
        b[bi][index[bi]] = arr[i];
        index[bi]++;
    }

    // sort the buckets
    for (int i = 0; i < n; i++)
        qsort(b[i], index[i],sizeof(float),cmpfunc);

    // concatenate buckets (gather step)
    int indx = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < index[i]; j++)
            arr[indx++] = b[i][j];
}

int main(){
    srand(time(NULL));
    float arr[SIZE];

    // fill array with random values 
    for(int i=0;i<SIZE;i++){
        arr[i] = rand()+30;
    }

    printf("Original array: ");
    for (int i = 0; i < SIZE; i++)
        printf("%f ", arr[i]);

    bucketSort(arr, BUCKETNUM);

    printf("\n ************************************\n");
    printf("\nSorted array:");
    for (int i = 0; i < SIZE; i++)
        printf("%f ", arr[i]);
    
    return 0;
    
}
