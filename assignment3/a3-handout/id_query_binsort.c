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

int compare_ids(const void * ip, const void * jp){
  int64_t id1 = ((struct record *)ip)->osm_id;
  int64_t id2 = ((struct record *)jp)->osm_id;
 // assert(id1 != NULL && id2 != NULL);

  if(id1 < id2){return -1;}
  else if(id1 == id2){return 0;}
  else{return 1;}
}


struct indexed_data* mk_indexed(struct record* rs, int n) {
  assert(n >= 0);
  assert(rs != NULL);

  //struct record* srs = malloc(n*sizeof(struct record));
  //memcpy(srs, rs, n*sizeof(struct record));
  //qsort(srs,n,sizeof(struct record), (int(*)(void*, void*))compare_ids);
  
  qsort(rs,n,sizeof(struct record), (int(*)(const void*, const void*))compare_ids);
  struct indexed_data *ind = malloc(sizeof(struct indexed_data));
  ind->n = n;
  struct index_record *irs = malloc(n*sizeof(struct index_record));
  for(int i = 0; i < n; i++){
    irs[i].osm_id = rs[i].osm_id;
    irs[i].record = &rs[i];
    if(i>0){assert(irs[i-1].osm_id <= irs[i].osm_id);}
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
  //const struct record *r = binary_search(data, needle, 0, data->n-1);
  int lower = 0;
  int upper = data->n-1;
  int mid = (lower + upper)/2;
  while(lower <= upper){
    if(data->irs[mid].osm_id == needle){
      const struct record *r = (data->irs[mid].record);
      return r;
    }
    else if(data->irs[mid].osm_id < needle){
      lower = mid + 1;
    }
    else{
      upper = mid - 1;
    }
    mid = (lower + upper)/2;
  }

  return NULL;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_indexed);
}
