#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

int main(){
  double* a = malloc(5*sizeof(double));
  double* b = malloc(5*sizeof(double));
  double* c = malloc(5*sizeof(double));

  a[0] = 0;
  a[1] = 0;
  a[2] = 0;
  a[3] = 0;
  a[4] = 0;

  b[0] = 1;
  b[1] = 1;
  b[2] = 1;
  b[3] = 1;
  b[4] = 1;

  c[0] = 2;
  c[1] = 2;
  c[2] = 2;
  c[3] = 2;
  c[4] = 2;

  printf("%f\n", distance(5, &a[0], &b[0]));

  printf("%f\n", distance(5, &c[0], &b[0]));

  int* list = malloc(5*sizeof(int));


}


