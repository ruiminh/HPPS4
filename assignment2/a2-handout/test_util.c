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

  list[0] = 0;
  list[1] = 1;
  list[2] = 2;
  list[3] = 3;
  list[4] = 4;

  for(int i = 0; i < 5; i++){
    printf("%d ", list[i]);
  }
  printf("\n");

  insert_index(9,2,5,list);

  for(int i = 0; i < 5; i++){
    printf("%d ", list[i]);
  }
  printf("\n");
}


