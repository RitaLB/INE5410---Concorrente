#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char *argv[]) {
  unsigned int nthreads;
  double wall_timer;

  for (nthreads = 1; nthreads <= 10; ++nthreads) {
    wall_timer = omp_get_wtime();
    
    #pragma omp parallel for num_threads(nthreads)
    for (unsigned long int i = 0; i < 10000000000; ++i);


    printf("threads: %d - time (s): %f\n", nthreads,  omp_get_wtime() - wall_timer);
  }

  return 0;
}
