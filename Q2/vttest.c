#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  printf(1, "--- Virtual-to-Physical Address Translation ---\n");

  int local_var = 100;
  int va1 = (int)&local_var;
  int pa1 = vtop(va1);
  printf(1, "Stack Var   -> VA: 0x%x | PA: 0x%x\n", va1, pa1);

  char *heap_mem = sbrk(4096);
  int va2 = (int)heap_mem;
  int pa2 = vtop(va2);
  printf(1, "sbrk Heap   -> VA: 0x%x | PA: 0x%x\n", va2, pa2);

  int va_invalid = 0x7FFFFFFF;
  int pa_invalid = vtop(va_invalid);
  printf(1, "Invalid VA  -> VA: 0x%x | Result: %d (Unmapped)\n", va_invalid, pa_invalid);

  exit();
}
