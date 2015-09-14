/*----------------------------------------------------------
 * Examen práctico 1: QuadTree
 * Fecha: 15-Sep-2015
 * Autores:
 *           A01169701 Rodolfo Andrés Ramírez Valenzuela
 *           A01371743 Luis Eduardo Ballinas Aguilar
 *----------------------------------------------------------*/




#include <stdio.h>
#include <stdlib.h>



/********************************
QuadTree
*********************************/

//Point struct
typedef struct Points{
  int x;
  int y;
} Point;

typedef struct {
  Point *array;
  size_t used;
  size_t size;
} PointArray;

//Node struct
typedef struct Nodes{
  struct Nodes *ne;
  struct Nodes *nw;
  struct Nodes *se;
  struct Nodes *sw;
  unsigned int size;
  unsigned int width;
  unsigned int height;
  char level[30];

  double mean;
  //Poblation of this node which will be represented by the points
  PointArray pointsArray;


} Node;
//Struct QuadTree

typedef struct QuadTree{
  Node *root;
} QuadTree;


/**********************
QuadTree routines
***********************/
void insert (Node *node, PointArray *a, Point element);
int size (Point arr[]);
void initArray(PointArray *a, size_t initialSize);
void freeArray(PointArray *a);


void insert(Node *node, PointArray *a, Point element) {
  PointArray pointsArray = node->pointsArray;
  printf("Used : %zd\n",pointsArray.used );
  if (a->used == a->size){
    printf("Doing a partition\n" );
    //if the node doesn't have any space then partition the Array
  }else{
    a->array[a->used++] = element;
  }

}


/**********************
Dynamic Point Array Structure
***********************/

int size(Point arr[]){
  int n = sizeof(arr) / sizeof(Point) ;
  return n;
}


void initArray(PointArray *a, size_t initialSize) {
  a->array = (Point *)malloc(initialSize * sizeof(Point));
  a->used = 0;
  a->size = initialSize;
}
/*
void insertArray(PointArray *a, Point element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (Point *)realloc(a->array, a->size * sizeof(Point));
  }
  a->array[a->used++] = element;
}
*/

void freeArray(PointArray *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}



int main(){
  //Create the initial QuadTree Structure
  //QuadTree quadTree;
  Node root;
  root.size = 20;
  root.width = 20;
  root.height = 20;
  //quadTree.root = &root;
  initArray(&root.pointsArray,3);

  Point pt1;
  pt1.x = 3;
  pt1.y = 4;
  Point pt2;
  pt2.x = 10;
  pt2.y = 4;
  Point pt3;
  pt3.x = 20;
  pt3.y = 4;

  insert(&root, &root.pointsArray, pt1);
  insert(&root, &root.pointsArray, pt2);
  insert(&root, &root.pointsArray, pt3);

  printf("%d\n",root.pointsArray.array[0].x );
  printf("%d\n",root.pointsArray.array[1].x );
  printf("%d\n",root.pointsArray.array[2].x );
















  return 1;
}
