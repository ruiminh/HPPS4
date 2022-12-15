#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

double* read_points(FILE *f, int* n_out, int *d_out) {
  int read;
  int32_t n,d;

  read = fread(&n, sizeof(int32_t), 1, f);
  assert(read == 1);
  read = fread(&d, sizeof(int32_t), 1, f);
  assert(read == 1);

  double *data = malloc(n*d*sizeof(double));
  read = fread(data, sizeof(double), n*d, f);
  assert(read == n*d);

  *n_out = n;
  *d_out = d;
  return data;
}

int* read_indexes(FILE *f, int *n_out, int *d_out) {
  int read;
  int32_t n,d;

  read = fread(&n, sizeof(int32_t), 1, f);
  assert(read == 1);
  read = fread(&d, sizeof(int32_t), 1, f);
  assert(read == 1);

  int* data = malloc(n*d*sizeof(int));
  read = fread(data, sizeof(int), n*d, f);
  assert(read == n*d);

  *n_out = n;
  *d_out = d;
  return data;
}

int write_points(FILE *f, int32_t n, int32_t d, double *data) {
  int write;
  if(n <= 0 || d <= 0){return 1;}
  if(data == NULL){return 1;}

  write = fwrite(&n, sizeof(int32_t), 1, f);
  if(write == 0){return 1;}
  write = fwrite(&d, sizeof(int32_t), 1, f);
  if(write == 0){return 1;}

  write = fwrite(data, d*sizeof(double), n, f);
  if(write == n){return 0;}
  else{return 1;}
}

int write_indexes(FILE *f, int32_t n, int32_t k, int *data) {
  int write;
  if(n <= 0 || k <= 0){return 1;}
  if(data == NULL){return 1;}

  write = fwrite(&n, sizeof(int32_t), 1, f);
  if(write == 0){return 1;}
  write = fwrite(&k, sizeof(int32_t), 1, f);
  if(write == 0){return 1;}

  write = fwrite(data, k*sizeof(int), n, f);
  if(write == n){return 0;}
  else{return 1;}
}
