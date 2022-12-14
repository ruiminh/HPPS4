#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

double distance(int d, const double *x, const double *y) {
  assert(d > 0);
  double sum = 0;
  for(int i = 0; i < d; i++){
    sum += pow(x[i] - y[i], 2);
  }
  assert(sum >= 0);
  return sqrt(sum);
}

//function that moves all index values from 'position' one index down, and then inserts the new 'index'.
void insert_index(int index, int position, int k, int* closest){
  for(int i = k-2; i >= position; i--){
    closest[i+1] = closest[i];
  }
  closest[position] = index;
}

int insert_if_closer(int k, int d,
                     const double *points, int *closest, const double *query,
                     int candidate) {
  //k number of closest elements.
  //d number of dimensions in the space.
  //points is the array of all reference points.
  //closest is an array of length 'k' that contains valid indexes into
  //'points', or -1 to indicate the absance of an element.
  //query is the query point from which distances are computed.
  //candidate is the index of a point in points.

  //we need to update closest to contain candidate if it is closer to query than
  //any other point in closest.

  //in pseudo ish code:
  //firstly, determine whether there are empty entries in closest.
  //we do this by initially checking if the last entry is -1, and if it is
  //we check the first entry - if it is -1, we add our entry here. Otherwise,
  //we check the next entry - etc...


  if(closest[0] == -1){
    closest[0] = candidate;
    return 1;
  }


  //before any of that, check if the candidate is already on the list
  for(int i = 0; i < k; i++){
    if(closest[i] == candidate){
      return 0;
    }
  }

  //if there are no empty entries, then we must compare distances.
  //since we are not saving the distances (predetermined from the prototype), we have to compute them
  //everytime we need to compare. Fortunately we can temporarily save distance of candidate and reuse.

  double cadist = distance(d,&points[candidate*d], query);

  //We start at the middle entry in closest and work our way up or down the list depending on whether
  //the candidate point is closer or further away than that point.
  //this way, our max 'travel distance' is half of the array.
  bool ismid = false;
  int midid = (k - (k%2))/2;

  while(closest[midid] == -1){
    midid--;
  }

  double middist = distance(d, &points[closest[midid]*d], query);
  if(cadist <= middist){
    for(int i = 0; i < midid; i++){
      if(cadist <= distance(d, &points[closest[i]*d], query)){
	insert_index(candidate, i, k, closest);
	return 1;
      }
    }
    ismid = true;
  }
  if(ismid){
    insert_index(candidate, midid, k, closest);
    return 1;
  }
  else{
    for(int i = midid + 1; i < k; i++){
      if(closest[i] == -1){
	insert_index(candidate, i, k, closest);
	return 1;
      }
      else if(cadist <= distance(d, &points[closest[i]*d], query)){
	insert_index(candidate, i, k, closest);
	return 1;
      }
    }
  }
  return 0;
}
