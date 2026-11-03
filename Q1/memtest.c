#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  printf(1, "Initial Memory Stats:\n");
  memstats();
  printf(1, "\nAllocating memory using sbrk(4096 * 5)...\n");
  sbrk(4096 * 5);
  printf(1, "\nMemory Stats After Allocation:\n");
  memstats();
  printf(1, "\nFreeing memory...\n");
  sbrk(-(4096 * 5)); 
  printf(1, "\nMemory Stats After Freeing:\n");
  memstats();
  exit();
}
