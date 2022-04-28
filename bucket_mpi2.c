#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <mpi.h>


#define SIZE 200
#define BUCKETNUM 10

#define send_data_tag 2001
#define return_data_tag 2002

int arr2[SIZE];

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



int main(int argc, char **argv){

    int my_id, root_process, ierr, num_procs, an_id;
    MPI_Status status;

    srand(time(NULL));
    int arr[SIZE];

    // fill array with random values 
    for(int i=0;i<SIZE;i++){
        arr[i] = rand()%100;
    }
   
    int max, min;
    minMax(arr,&min,&max);
    float value = (max - min) / (BUCKETNUM-1);
    int range = ceil(value);
    
    int bucket_size = 3*(SIZE/BUCKETNUM);
    int b[BUCKETNUM][bucket_size];
    int b2[BUCKETNUM][bucket_size];

    int index[BUCKETNUM];
    for (int i=0;i<BUCKETNUM;i++)
        index[i] = 0;
    double max_index = BUCKETNUM-2;

    int bucket_per_proc = BUCKETNUM / num_procs;

    int proc_range[2], proc_range2[2];

    ierr = MPI_Init(&argc, &argv);

    root_process = 0;

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);


    // bucketSort(arr, BUCKETNUM, my_id, num_procs, ierr);

    if (my_id == root_process){
     

      // assign buckets for each element from arr (scatter step)
      for (int i = 0; i < SIZE; i++) {
        int bi = (arr[i] - min) / range; // Index in bucket

        if (bi > max_index){
            bi = max_index;
        }
        b[bi][index[bi]] = arr[i];
        index[bi]++;
      }

      for(int an_id = 1; an_id < num_procs; an_id++){
        proc_range[0] = an_id*bucket_per_proc+1;
        proc_range[1] = (an_id+1)*bucket_per_proc;

        if ((BUCKETNUM-proc_range[1])<bucket_per_proc)
            proc_range[1] = BUCKETNUM-1;
        
        int buckets_to_send = proc_range[1] - proc_range[0] + 1;

        ierr = MPI_Send(&proc_range, 2, MPI_INT,
                an_id, send_data_tag, MPI_COMM_WORLD);
        ierr = MPI_Send(&b, buckets_to_send*bucket_size, MPI_INT,
                an_id, send_data_tag, MPI_COMM_WORLD);

      }

        
    }
    else {
      ierr = MPI_Recv(&proc_range2, 2, MPI_INT,
            root_process, send_data_tag, MPI_COMM_WORLD, &status);
      ierr = MPI_Recv(&b2, (proc_range2[1] - proc_range2[0] + 1)*bucket_size, MPI_INT,
            root_process, send_data_tag, MPI_COMM_WORLD, &status);


      for(int i=proc_range[0];i<proc_range[1];i++){
        insertion_sort(b2[i],index[i]);
      }

      ierr = MPI_Send( &b2, (proc_range2[1] - proc_range2[0] + 1)*bucket_size, MPI_INT, 
            root_process, return_data_tag, MPI_COMM_WORLD);
    }
    ierr = MPI_Finalize();
   
   
   
   
   
   
  
    return 0;
}
