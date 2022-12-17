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

struct sort_env{
    int c;
    int d;
    const double *points;
};


//compare function for sorting
int cmp_indexes(const int *ip, const int *jp, struct sort_env* env);

struct node* kdtree_create_node(int d, const double *points,
                                int depth, int n, int *indexes) {
  struct node *newNode=malloc(sizeof(struct node));
  int ax = depth % d;
  newNode->axis = ax;

  // sort  points' indexes by the axis (a column)

   struct sort_env env;
   env.points=points;
   env.d = d;
   env.c = ax;
   //sort the index
   hpps_quicksort(indexes,n, sizeof(int),
                          (int(*)(const void*, const void*, void*))cmp_indexes,
			  &env);

  int m = (n - n%2)/2 - 1;
  newNode->point_index= indexes[m];//use the index of median value
  //prepair arguments for create two childen:

  int l = m; // size of left side
  int r = n-m-1;

  //left side
  if(l>1){
    newNode->left = kdtree_create_node(d, points, depth+1, l, indexes);
  }else{newNode->left = NULL; }

  //right side
  if(r>1){
    newNode->right = kdtree_create_node(d, &points[(m+1)*d], depth+1, r, &indexes[m+1]);
  }else{newNode->right = NULL;}

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
  if(node == NULL){
    return;
  }
  else insert_if_closer(k, tree->d, tree->points, closest, query, node->point_index);

  double diff = tree->points[node->point_index + node->axis] - query[node->axis];

  *radius = distance(tree->d, query, &tree->points[closest[k-1]*tree->d]);

  if (diff >= 0 && *radius > fabs(diff)){
    kdtree_knn_node(tree, k, query, closest, radius, node->left);
  }
  if (diff <= 0 && *radius > fabs(diff)){
    kdtree_knn_node(tree, k, query, closest, radius, node->right);
  }
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


int cmp_indexes(const int *ip, const int *jp, struct sort_env* env) {
  int i = *ip;
  int j = *jp;
  const double *x = &env->points[i*env->d];
  const double *y = &env->points[j*env->d];
  int c = env->c;

  if (x[c] < y[c]) {
    return -1;
  }
  else if (x[c] == y[c]) {
    return 0;
  }
  else {
    return 1;
  }
}
