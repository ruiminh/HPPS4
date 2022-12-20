#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h"

struct naive_data {
  struct record *rs;
  int n;
};

struct naive_data* mk_naive(struct record* rs, int n) {
  assert(n >= 0);
  assert(rs != NULL);
  struct naive_data *nd = malloc(sizeof(struct naive_data));
  nd->rs = rs;
  nd->n = n;
  return nd;
}

void free_naive(struct naive_data* data) {
  assert(data != NULL);
  free(data);
}

const struct record* lookup_naive(struct naive_data *data, int64_t needle) {
  struct record *r = NULL;
  if(needle < 0){printf("Received negative index, quitting.\n"); exit(0);}
  for(int i = 0; i < data->n; i++){
    if(data->rs[i].osm_id == needle){
      r = &data->rs[i];
      break;
    }
  }
  return r;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_naive,
                    (free_index_fn)free_naive,
                    (lookup_fn)lookup_naive);
}
