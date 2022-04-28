#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <mpi.h>


#define SIZE 200
#define BUCKETNUM 10

void insertion_sort(int arr[SIZE], int n){
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


void bucketSort(int arr[SIZE], int n, int pid, int bucket_per_proc){

    if (pid == 0){

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
    
    }
    
    // sort the buckets
    for (int i = 0; i < n; i++)
        insertion_sort(b[i], index[i]);

    if (pid == 0){

    // concatenate buckets (gather step)
    int indx = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < index[i]; j++)
            arr[indx++] = b[i][j];
    }
}

int main(int argc, char **argv){

    int my_id, root_process, ierr, num_procs, an_id;
    MPI_Status status;
   
    ierr = MPI_Init(&argc, &argv);

    root_process = 0;

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if(my_id == root_process){

        bucket_per_proc = BUCKETNUM / num_procs;

        srand(time(NULL));
        int arr[SIZE];

        // fill array with random values 
        for(int i=0;i<SIZE;i++){
            arr[i] = rand()%100;
        }

    }
    bucketSort(arr, BUCKETNUM, my_id, bucket_per_proc);

    if(my_id == root_process) {

        // printf("\nSorted array:\n");
        // for (int i = 0; i < SIZE; i++)
        //     printf("%d ", arr[i]);
        // printf("\n\n");
    }
    
    return 0;
}
