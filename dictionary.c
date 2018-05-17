#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hashmap.h"

#define SEED "rip university square"
#define MAX_KEY_LEN 128

unsigned int universal_hash(char *key) {
  static int *r = NULL;
  if (!r) {
    r = malloc(MAX_KEY_LEN * (sizeof *r));
    assert(r);

    int i;
    for (i = 0; i < MAX_KEY_LEN; i++) {
      r[i] = rand();
    }
  }

  unsigned int h = 0;
  int i;
  for (i = 0; key[i] != '\0'; i++) {
    h = h + r[i] * key[i];
  }
  return h;
}

HashMap *new_dictionary(int n_buckets) {
  return new_hashmap(n_buckets, universal_hash);
}