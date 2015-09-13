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
  unsigned int width;
  unsigned int height;
  unsigned int level;
  unsigned int size;
  double mean;
  //Poblation of this node which will be represented by the points
  PointArray pointsArray;


} Node;
//Struct QuadTree

typedef struct QuadTree{
  Node *root;
  unsigned int size;
} QuadTree;



/**********************
Dynamic Point Array Structure
***********************/


void initArray(PointArray *a, size_t initialSize) {
  a->array = (Point *)malloc(initialSize * sizeof(Point));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(PointArray *a, Point element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (Point *)realloc(a->array, a->size * sizeof(Point));
  }
  a->array[a->used++] = element;
}

void freeArray(PointArray *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}







int main(){
  PointArray a;
  initArray(&a,1);

  Point pt1;
  Point pt2;
  Point pt3;

  pt1.x = 1;
  printf("%d\n",pt1.x );

  pt2.x = 2;
  pt3.x = 3;

  insertArray(&a,pt1);
  insertArray(&a,pt2);
  insertArray(&a,pt3);
  printf("%d\n",a.array[1].x );

  //initArray(&a,pt1);
  //initArray(&a,pt2);
  //initArray(&a,pt3);





  return 1;
}
