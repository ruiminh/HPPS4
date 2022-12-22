#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

#include "record.h"
#include "coord_query.h"

struct naive_data {
  struct record *rs;
  int n;
};

double rec_distance(double rlon, double rlat, double qlon, double qlat){
  double safeqlon = 0;
  double safeqlat = 0;

  if(qlon < 0 || qlon > 0){safeqlon = qlon;}
  if(qlat < 0 || qlat > 0){safeqlat = qlat;}

  double dlon = pow(rlon - safeqlon,2);
  double dlat = pow(rlat - safeqlat,2);

  double dist = sqrt(dlon + dlat);
  assert(dist >= 0);
  return dist;
}

struct naive_data* mk_naive(struct record* rs, int n) {
  assert(rs != NULL);
  assert(n > 0);

  struct naive_data* data = malloc(sizeof(struct naive_data));

  data->n = n;
  data->rs = rs;
  return data;
}

void free_naive(struct naive_data* data) {
  assert(data != NULL);
  free(data);
}

const struct record* lookup_naive(struct naive_data *data, double lon, double lat) {
  assert(data != NULL);
  double closest = -1;
  struct record *cr;
  double distance;

  for(int i = 0; i<data->n; i++){
    if((distance = rec_distance(lon, lat, data->rs[i].lon, data->rs[i].lat)) < closest || closest < 0){
      closest = distance;
      cr = &data->rs[i];
    }
  }
  return cr;
}

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_naive,
                          (free_index_fn)free_naive,
                          (lookup_fn)lookup_naive);
}
