#include <stdio.h>
#include <stdlib.h>   // For perror and exit
#include <sys/mman.h> // For mmap

#ifdef __linux__
#include <numa.h>   // For NUMA functions and types like nodemask_t
#include <numaif.h> // For mbind and other NUMA-related functions
#endif

void *mlm_malloc(size_t size, int level) {
  void *ptr;

  ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
             -1, 0);
  if (ptr == MAP_FAILED) {
    perror("mmap2");
    exit(1);
  }

#ifdef __linux__
  nodemask_t nodemask;
  nodemask_zero(&nodemask);
  nodemask_set_compat(&nodemask, level);

  if (mbind(ptr, size, MPOL_PREFERRED, &nodemask, sizeof(unsigned long),
            MPOL_MF_MOVE) < 0) {
    perror("mbind");
    exit(1);
  }
#endif

  return ptr;
}

void ariel_mlm_free(void *ptr) {
  // Freeing logic here (if needed)
}
