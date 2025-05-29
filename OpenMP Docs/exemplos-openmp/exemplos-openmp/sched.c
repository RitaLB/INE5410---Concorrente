#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>

#define SIZE 20

void compute_static(unsigned int nthreads) {
  char str[SIZE * 4], strtmp[10];
  
  sprintf(str, " ");
  
  #pragma omp for schedule(static)
  for (unsigned int i = 0; i < SIZE; ++i) {
    sprintf(strtmp, "%d ", i);
    strcat(str, strtmp);
    usleep(100);
  }

  #pragma omp critical
  {
    printf("thread %d:%s\n", omp_get_thread_num(), str);
    fflush(stdout);
  }
}

void compute_dynamic(unsigned int nthreads) {
  char str[SIZE * 4], strtmp[10];
  
  sprintf(str, " ");
  
  #pragma omp for schedule(dynamic)
  for (unsigned int i = 0; i < SIZE; ++i) {
    sprintf(strtmp, "%d ", i);
    strcat(str, strtmp);
    usleep(100);
  }

  #pragma omp critical
  {
    printf("thread %d:%s\n", omp_get_thread_num(), str);
    fflush(stdout);
  }
}

int main(int argc, char *argv[]) {
  
  for (unsigned int nthreads = 1; nthreads <= 4; ++nthreads) {
    printf("\nRunning with %d thread(s)\n", nthreads);
    #pragma omp parallel num_threads(nthreads) 
    {
      #pragma omp single
      printf("Static scheduling:\n");
      
      compute_static(nthreads);
      
      #pragma omp barrier

      #pragma omp single
      printf("Dynamic scheduling:\n");
      
      compute_dynamic(nthreads);
    }
  }

  return 0;
}
