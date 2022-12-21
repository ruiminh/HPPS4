#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h"

struct index_record {
  int64_t osm_id;
  const struct record *record;
};

struct indexed_data {
  struct index_record *irs;
  int n;
};

struct indexed_data* mk_indexed(struct record* rs, int n) {
  assert(n >= 0);
  assert(rs != NULL);
  struct indexed_data *ind = malloc(sizeof(struct indexed_data));
  ind->n = n;
  struct index_record *irs = malloc(n*sizeof(struct index_record));
  for(int i = 0; i < n; i++){
    irs[i].osm_id = rs[i].osm_id;
    irs[i].record = &rs[i];
  }
  ind->irs = irs;
  return ind;
}

void free_indexed(struct indexed_data* data) {
  assert(data != NULL);
  free(data->irs);
  free(data);
}

const struct record* lookup_indexed(struct indexed_data *data, int64_t needle) {
  if(needle < 0){printf("Received negative index, quitting.\n"); exit(0);}
  for(int i = 0; i < data->n; i++){
    if(data->irs[i].osm_id == needle){
      const struct record *r = data->irs[i].record;
      return r;
    }
  }
  return NULL;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_indexed);
}
