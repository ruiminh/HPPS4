#include "kdtree.h"
#include "sort.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

struct node {
  int point_index;
  int axis;
  struct node *left;
  struct node *right;
};

struct kdtree {
  int d;
  const double *points;
  struct node* root;
};

int compare(const double *points

struct node* kdtree_create_node(int d, const double *points,
                                int depth, int n, int *indexes) {
  struct node *newNode=malloc(sizeof(struct node));
  int ax = depth % d;
  newNode->axis = ax;
  //To Do: here need to sort  points by the axis (a column)
  //using: hpps_sort(points, ax), I need to study sort.c and do it later.
  //update the indexes to a sorted index (increasing?)

  int m = n/2+1;
  newNode->point_index= indexes[m];//use the index of mean value
  //prepair arguments for create two childen:
  
  int l = m-1; // size of left side
  int r = n-m-1;

  //left side
  if(l>1){
      //left half indexes, the points are smaller than mean:
      int *indexes_left = malloc(l*sizeof(int));
      for(int i=0; i<l; i++){
          indexes_left[i]=indexes[i];
      }// new indexes

      //left half points, according left side indexes
      const double *points_left = malloc(l*sizeof(const double));
      for (int i=0; i< m-1; i++){
         points_left[i]=points[indexes[i]];
      } //new points
      newNode->left = kdtree_create_node(d, points_left, depth+1, l, indexes_left );
  }else{newNode->left = NULL; }

  //right side
  if(r>1){
      
      int *indexes_right = malloc(r*sizeof(int));
      for(int i=0; i>r; i++){
          indexes_right[i]=indexes[i+l];
      }// new indexes

  
      const double *points_right = malloc(r*sizeof(const double));
      for (int i=0; i< r; i++){
         points_right[i]=points[indexes[i]];
      } //new points
      newNode->left = kdtree_create_node(d, points_right, depth+1, l, indexes_right );
  }else{newNode->right = NULL;}

  free(indexes);
  
  return newNode;
}

struct kdtree *kdtree_create(int d, int n, const double *points) {
  struct kdtree *tree = malloc(sizeof(struct kdtree));
  tree->d = d;
  tree->points = points;

  int *indexes = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    indexes[i] = i;
  }

  tree->root = kdtree_create_node(d, points, 0, n, indexes);

  free(indexes);

  return tree;
}

void kdtree_free_node(struct node *node) {
  //DFS travsal the tree using recursion postorder
  if (node == NULL){
    return;
  }
  kdtree_free_node(node->left);
  
  kdtree_free_node(node->right);

  free(node);
  
}

void kdtree_free(struct kdtree *tree) {
  kdtree_free_node(tree->root);
  free(tree);
}

void kdtree_knn_node(const struct kdtree *tree, int k, const double* query,
                     int *closest, double *radius,
                     const struct node *node) {
  assert(0);
}

int* kdtree_knn(const struct kdtree *tree, int k, const double* query) {
  int* closest = malloc(k * sizeof(int));
  double radius = INFINITY;

  for (int i = 0; i < k; i++) {
    closest[i] = -1;
  }

  kdtree_knn_node(tree, k, query, closest, &radius, tree->root);

  return closest;
}

void kdtree_svg_node(double scale, FILE *f, const struct kdtree *tree,
                     double x1, double y1, double x2, double y2,
                     const struct node *node) {
  if (node == NULL) {
    return;
  }

  double coord = tree->points[node->point_index*2+node->axis];
  if (node->axis == 0) {
    // Split the X axis, so vertical line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            coord*scale, y1*scale, coord*scale, y2*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, coord, y2,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    coord, y1, x2, y2,
                    node->right);
  } else {
    // Split the Y axis, so horizontal line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            x1*scale, coord*scale, x2*scale, coord*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, x2, coord,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    x1, coord, x2, y2,
                    node->right);
  }
}

void kdtree_svg(double scale, FILE* f, const struct kdtree *tree) {
  assert(tree->d == 2);
  kdtree_svg_node(scale, f, tree, 0, 0, 1, 1, tree->root);
}
