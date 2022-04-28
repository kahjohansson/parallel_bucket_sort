#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<mpi.h>
#include<math.h>
#include<time.h>

// Struct que define opções de config
typedef struct {
  int rank, size, array_size, slice, max_num;
} Param;

// Struct que define o bucket
typedef struct {
  int * array;
  int index;
} Bucket;

void bucket_insert(Bucket *b, int x){
  b->array[b->index] = x;
  b->index = b->index+1;
}

int compare( const void * n1, const void * n2)
{
    return (*(int*)n1 - *(int*)n2);
}

void mpi_bucket(Param * p) {

  // Timer
  double start, end;

  int i;

  // número de baldes
  int num_buckets = p->size;

  // tamanho de cada balde
  int bucket_size = ceil(p->array_size/(float)p->size);

  int * array;

  if(p->rank == 0){
    start = MPI_Wtime();

    array = (int *) malloc(sizeof(int)*p->array_size);

    srand(time(NULL));

    for(int i = 0; i < p->array_size; i++)
      array[i] = rand()%(p->max_num);
  
    fprintf(stderr, "\n\nOriginal Array:\n");
    for(i=0;i<p->array_size;i++)
      fprintf(stderr, "%d ",array[i]);
    fprintf(stderr, "\n");
    
  }
  Bucket ** buckets = (Bucket **) malloc(sizeof(Bucket*)*num_buckets);
  for(i=0;i<num_buckets;i++){
    buckets[i] = (Bucket *) malloc(sizeof(Bucket));
    buckets[i]->array = (int*) malloc(sizeof(int)*bucket_size*2);
    buckets[i]->index = 0;
  }

  Bucket bucket;

  int *my_bucket_array = (int*) malloc(sizeof(int)*bucket_size*4);
  bucket.array = my_bucket_array;
  bucket.index = 0;

  // espalha o array entre os baldes
  MPI_Scatter(array, bucket_size, MPI_INT, bucket.array, bucket_size, MPI_INT,0,MPI_COMM_WORLD);
  int dest;

  for(i=0;i<bucket_size;i++){
    dest = (bucket.array[i] * num_buckets)/p->max_num;

    if(dest==p->rank)
      bucket_insert(&bucket, bucket.array[i]);
    else
      bucket_insert(buckets[dest],bucket.array[i]);
  }
  MPI_Request * requests = (MPI_Request *) malloc(sizeof(MPI_Request) * p->size);

  for(i=0; i<p->size; i++) 
    if(i != p->rank)
      MPI_Isend(buckets[i]->array, bucket_size*2,MPI_INT,i,buckets[i]->index,MPI_COMM_WORLD, &requests[i]);
  MPI_Status status;
  
  int current = bucket.index;

  for(i=0;i<p->size-1;i++){
    MPI_Recv(&bucket.array[current],bucket_size*2,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
    current+=status.MPI_TAG;
  }
  bucket.index = current;

  qsort(&bucket.array[0], current, sizeof(int),compare);

  int * sizes = (int *) malloc(sizeof(int)*p->size);
  MPI_Gather(&current,1,MPI_INT,sizes,1,MPI_INT,0,MPI_COMM_WORLD);

  int * disp = (int *) malloc(sizeof(int)*p->size);
  if(p->rank == 0){
    disp[0] = 0;
    for(i=1; i<p->size+1;i++)
      disp[i] = disp[i-1] + sizes[i-1];
  }

  MPI_Gatherv(bucket.array,current,MPI_INT,array,sizes,disp,MPI_INT,0,MPI_COMM_WORLD);

  if (p->rank == 0) {
    end = MPI_Wtime();
    fprintf(stderr,"\nElapsed time: %f\n",end-start);
    for(i=0;i<p->array_size;i++){
      fprintf(stdout,"%d ", array[i]);
    }
    fprintf(stdout,"\n ");
    free(array);
  }
  return;
}


int main(int argc, char **argv){
  int rank, size;

  if ( MPI_Init(&argc, &argv) != MPI_SUCCESS) {
    fprintf(stderr, "Não foi possível inicializar o MPI.\n");
    return -1;
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  Param p;

  p.rank = rank;
  p.size = size;

  p.array_size = 100000;
  p.max_num = 1000;


  mpi_bucket(&p);
  MPI_Finalize();
  return 0;
}