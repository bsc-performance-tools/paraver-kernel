#include <stdio.h>
#include <stdlib.h>
#include <omp.h>       // The OpenMP library

int main(int argc, char * argv[])
{
  int nthreads, nprocs, tid;

  // Request the number of available processors from the
  // OpenMP library
  nprocs = omp_get_num_procs();

  printf("Hello, world! There are %i procs here.\n",nprocs);

  // Use one thread per processor
  omp_set_num_threads(nprocs);

  // fork a team of threads
  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    printf("Hello from thread %i\n",tid);

    if( tid == 0 )
    {
      // get the number of threads from the library
      nthreads = omp_get_num_threads();

      printf("Number of threads = %i\n",nthreads);
    } 
  } // threads join and terminate

  return 0;
}

