#include "bruteforce.h"
#include "util.h"
#include <stdlib.h>
#include <assert.h>

int* knn(int k, int d, int n, const double *points, const double* query) {
  assert(k > 0 && d > 0 && n > 0 && points != NULL && query != NULL);
  int* closest = malloc(k*sizeof(int)); //remember to free() this sometime later!!
  for(int i = 0; i < n; i++){
    insert_if_closer(k, d, points, closest, query, n);
  }
  assert(closest != NULL);
  return closest;
}

