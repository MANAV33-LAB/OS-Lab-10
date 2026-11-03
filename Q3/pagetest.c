#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  printf(1, "--- Question 3: Page Protection Test ---\n");
  char *ptr = sbrk(4096);
  ptr[0] = 'A';
  printf(1, "Allocated 1 page at VA: 0x%x\n", (int)ptr);

  printf(1, "\nPage Info BEFORE setreadonly:\n");
  pageinfo((int)ptr);

  printf(1, "\nCalling setreadonly()...\n");
  setreadonly((int)ptr);

  printf(1, "\nPage Info AFTER setreadonly:\n");
  pageinfo((int)ptr);

  printf(1, "\nAttempting to write to read-only page (Triggers Trap)...\n");
  ptr[0] = 'B';

  printf(1, "If you see this, trap failed.\n");
  exit();
}
