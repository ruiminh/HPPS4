#include "util.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

int main(){
  double* a = malloc(5*sizeof(double));
  double* b = malloc(5*sizeof(double));
  double* c = malloc(5*sizeof(double));

  a[0] = 8.99;
  a[1] = 90;
  a[2] = 4.77;
  a[3] = 67;
  a[4] = 99;

  b[0] = 55;
  b[1] = 88;
  b[2] = 99.666;
  b[3] = 16;
  b[4] = 1;

  c[0] = 23;
  c[1] = 6666;
  c[2] = 1222.2222;
  c[3] = 2;
  c[4] = 2;

  printf("%f\n", distance(5, a, b));

  printf("%f\n", distance(5, c, b));

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

  //use 5_5.points to test insert closest

  FILE *f= fopen("5_5.points","r");
  int n,d;
  double *points = read_points(f,&n, &d);

  double *query = malloc(5*sizeof(double));
  query[0]=0.123;
  query[1]= 0.677;
  query[2]= 0.321;
  query[3] = 0.874;
  query[4] = 0.465;

  int *closest = malloc(3*sizeof(int));
  closest[0]=-1;
  closest[1]=-1;
  closest[2]=-1;

  for(int i=0;i<3;i++){
    printf("%d  ",closest[i]);}

  printf("\n");

  insert_if_closer(3, 5, points, closest, query, 0);
  

  for(int i=0;i<3;i++){
    printf("%d",closest[i]);}
  printf("\n");

  insert_if_closer(3, 5, points, closest, query,1); 

    for(int i=0;i<3;i++){
      printf("%d  ",closest[i]);}

    printf("\n");

    insert_if_closer(3, 5, points, closest, query,2);

    for(int i=0;i<3;i++){
      printf("%d  ",closest[i]);}

    printf("\n");
  
 insert_if_closer(3, 5, points, closest, query, 3);
  

 for(int i=0;i<3;i++){
    printf("%d  ",closest[i]);}
  printf("\n");

  insert_if_closer(3, 5, points, closest, query, 2);
  

 for(int i=0;i<3;i++){
    printf("%d  ",closest[i]);}
  printf("\n");


  insert_if_closer(3, 5, points, closest, query, 4);
  for(int i=0;i<3;i++){
    printf("%d  ",closest[i]);}
  printf("\n");
}
