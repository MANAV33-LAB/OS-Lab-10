#include "types.h"
#include "stat.h"
#include "user.h"

#define REF_LEN 20

int refs[REF_LEN] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};

void print_frames(int frames[], int capacity) {
  printf(1, "[");
  for(int i = 0; i < capacity; i++) {
    if(frames[i] == -1) printf(1, " - ");
    else printf(1, " %d ", frames[i]);
  }
  printf(1, "]");
}

int run_fifo(int capacity, int verbose) {
  int frames[10];
  for(int i=0; i<capacity; i++) frames[i] = -1;
  int faults = 0, next_replace = 0;

  if(verbose) printf(1, "\n--- FIFO Page Replacement (Frames: %d) ---\n", capacity);

  for(int i = 0; i < REF_LEN; i++) {
    int page = refs[i];
    int hit = 0;
    for(int j = 0; j < capacity; j++) {
      if(frames[j] == page) { hit = 1; break; }
    }
    if(!hit) {
      faults++;
      frames[next_replace] = page;
      next_replace = (next_replace + 1) % capacity;
    }
    if(verbose) {
      printf(1, "Ref %d: ", page);
      print_frames(frames, capacity);
      if(hit) printf(1, " -> HIT\n");
      else printf(1, " -> FAULT\n");
    }
  }
  return faults;
}

int run_lru(int capacity, int verbose) {
  int frames[10];
  int last_used[10];
  for(int i=0; i<capacity; i++) { frames[i] = -1; last_used[i] = -1; }
  int faults = 0;

  if(verbose) printf(1, "\n--- LRU Page Replacement (Frames: %d) ---\n", capacity);

  for(int i = 0; i < REF_LEN; i++) {
    int page = refs[i];
    int hit = 0;
    for(int j = 0; j < capacity; j++) {
      if(frames[j] == page) {
        hit = 1;
        last_used[j] = i;
        break;
      }
    }
    if(!hit) {
      faults++;
      int empty_idx = -1;
      for(int j = 0; j < capacity; j++) {
        if(frames[j] == -1) { empty_idx = j; break; }
      }
      if(empty_idx != -1) {
        frames[empty_idx] = page;
        last_used[empty_idx] = i;
      } else {
        int lru_idx = 0;
        int min_time = last_used[0];
        for(int j = 1; j < capacity; j++) {
          if(last_used[j] < min_time) {
            min_time = last_used[j];
            lru_idx = j;
          }
        }
        frames[lru_idx] = page;
        last_used[lru_idx] = i;
      }
    }
    if(verbose) {
      printf(1, "Ref %d: ", page);
      print_frames(frames, capacity);
      if(hit) printf(1, " -> HIT\n");
      else printf(1, " -> FAULT\n");
    }
  }
  return faults;
}

int run_optimal(int capacity, int verbose) {
  int frames[10];
  for(int i=0; i<capacity; i++) frames[i] = -1;
  int faults = 0;

  if(verbose) printf(1, "\n--- Optimal Page Replacement (Frames: %d) ---\n", capacity);

  for(int i = 0; i < REF_LEN; i++) {
    int page = refs[i];
    int hit = 0;
    for(int j = 0; j < capacity; j++) {
      if(frames[j] == page) { hit = 1; break; }
    }
    if(!hit) {
      faults++;
      int empty_idx = -1;
      for(int j = 0; j < capacity; j++) {
        if(frames[j] == -1) { empty_idx = j; break; }
      }
      if(empty_idx != -1) {
        frames[empty_idx] = page;
      } else {
        int replace_idx = -1;
        int farthest = i;
        for(int j = 0; j < capacity; j++) {
          int k;
          for(k = i + 1; k < REF_LEN; k++) {
            if(frames[j] == refs[k]) break;
          }
          if(k == REF_LEN) {
            replace_idx = j;
            break;
          }
          if(k > farthest) {
            farthest = k;
            replace_idx = j;
          }
        }
        if(replace_idx == -1) replace_idx = 0;
        frames[replace_idx] = page;
      }
    }
    if(verbose) {
      printf(1, "Ref %d: ", page);
      print_frames(frames, capacity);
      if(hit) printf(1, " -> HIT\n");
      else printf(1, " -> FAULT\n");
    }
  }
  return faults;
}

int main(void) {
  printf(1, "=== Question 4: Page Replacement Algorithms ===\n");
  printf(1, "Reference String: 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1\n");

  run_fifo(3, 1);
  run_lru(3, 1);
  run_optimal(3, 1);

  printf(1, "\n=======================================================\n");
  printf(1, " COMPARISON TABLE FOR 3, 4, AND 5 FRAMES\n");
  printf(1, "=======================================================\n");
  printf(1, "Alg | Frames | Faults | Hits | Hit Ratio | Fault Ratio\n");
  printf(1, "----+--------+--------+------+-----------+------------\n");

  int frame_counts[3] = {3, 4, 5};
  for(int f = 0; f < 3; f++) {
    int cap = frame_counts[f];
    
    int ff = run_fifo(cap, 0);
    int fh = REF_LEN - ff;
    printf(1, "FIFO|   %d    |   %d   |  %d  |   %d%%    |   %d%%\n", cap, ff, fh, (fh*100)/REF_LEN, (ff*100)/REF_LEN);

    int lf = run_lru(cap, 0);
    int lh = REF_LEN - lf;
    printf(1, "LRU |   %d    |   %d   |  %d  |   %d%%    |   %d%%\n", cap, lf, lh, (lh*100)/REF_LEN, (lf*100)/REF_LEN);

    int of = run_optimal(cap, 0);
    int oh = REF_LEN - of;
    printf(1, "OPT |   %d    |   %d   |  %d  |   %d%%    |   %d%%\n", cap, of, oh, (oh*100)/REF_LEN, (of*100)/REF_LEN);
    printf(1, "----+--------+--------+------+-----------+------------\n");
  }

  printf(1, "\nObservation: Optimal Page Replacement performs best because\n");
  printf(1, "it replaces the page that will not be used for the longest time\n");
  printf(1, "in the future, minimizing total page faults.\n");

  exit();
}
