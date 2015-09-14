/*----------------------------------------------------------
 * Examen práctico 1: QuadTree
 * Fecha: 15-Sep-2015
 * Autores:
 *           A01169701 Rodolfo Andrés Ramírez Valenzuela
 *           A01371743 Luis Eduardo Ballinas Aguilar
 *----------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#define partition 3
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
  char level[30];
  double mean;
  int upperBound_X;
  int lowerBound_X;
  int upperBound_Y;
  int lowerBound_Y;
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
  //WE'RE STILL MISSING TO SET A CONDITION IF IT EXCEEDS THE SIZE OF THE TREE

  if (a->used == a->size){
    a->array[a->used++] = element;

    //Allocate memory for the childs
    node->ne = (Node *)malloc(sizeof(Node));
    node->nw = (Node *)malloc(sizeof(Node));
    node->se = (Node *)malloc(sizeof(Node));
    node->sw = (Node *)malloc(sizeof(Node));

    //Create the array of points for the childs
    initArray(&(node->ne->pointsArray),3);
    initArray(&(node->nw->pointsArray),3);
    initArray(&(node->se->pointsArray),3);
    initArray(&(node->sw->pointsArray),3);

    //Set the width and height for the childs
    //NORTH EAST
    node->ne->upperBound_X = node->upperBound_X;
    node->ne->lowerBound_X = node->upperBound_X/2;
    node->ne->upperBound_Y = node->upperBound_Y;
    node->ne->lowerBound_Y = node->upperBound_Y/2;
    printf("***** NorthEAST ********\n" );
    printf("New lowerBound_X : %d   New upperBound_X:  %d\n",node->ne->lowerBound_X,node->ne->upperBound_X );
    printf("New lowerBound_Y : %d   New upperBound_Y:  %d\n\n\n",node->ne->lowerBound_Y,node->ne->upperBound_Y );

    //SOUTH EAST
    node->se->upperBound_X = node->upperBound_X;
    node->se->lowerBound_X = node->upperBound_X/2;
    node->se->upperBound_Y = node->upperBound_Y/2;
    node->se->lowerBound_Y = node->lowerBound_Y;
    printf("***** SouthEAST ********\n" );
    printf("New lowerBound_X : %d   New upperBound_X:  %d\n",node->se->lowerBound_X,node->se->upperBound_X );
    printf("New lowerBound_Y : %d   New upperBound_Y:  %d\n\n\n",node->se->lowerBound_Y,node->se->upperBound_Y );

    //NORTH WEST
    node->nw->upperBound_X = node->upperBound_X/2;
    node->nw->lowerBound_X = node->lowerBound_X;
    node->nw->upperBound_Y = node->upperBound_Y;
    node->nw->lowerBound_Y = node->upperBound_Y/2;
    printf("***** NorthWest ********\n" );
    printf("New lowerBound_X : %d   New upperBound_X:  %d\n",node->nw->lowerBound_X,node->nw->upperBound_X );
    printf("New lowerBound_Y : %d   New upperBound_Y:  %d\n\n\n",node->nw->lowerBound_Y,node->nw->upperBound_Y );


    //SOUTH WEST
    node->sw->upperBound_X = node->upperBound_X/2;
    node->sw->lowerBound_X = node->lowerBound_X;
    node->sw->upperBound_Y = node->upperBound_Y/2;
    node->sw->lowerBound_Y = node->lowerBound_Y;
    printf("***** SouthWest ********\n" );
    printf("New lowerBound_X : %d   New upperBound_X:  %d\n",node->sw->lowerBound_X,node->sw->upperBound_X );
    printf("New lowerBound_Y : %d   New upperBound_Y:  %d\n\n\n",node->sw->lowerBound_Y,node->sw->upperBound_Y );


    //Distribute the points between the children
    int i,x,y;
    for (i = 0; i < a->used; i++) {
      x = a->array[i].x;
      y = a->array[i].y;
      printf("Punto x:%d  y:%d ",x,y );
      if (x > node->upperBound_X/2){ //NE or SE
        if (y > node -> upperBound_Y / 2){ //NE
          insert(node->ne,&(node->ne->pointsArray), a->array[i]);
          printf("Inserting Node in Northeast\n" );
        }else{ //SE
          printf("Inserting Node in Southeast\n" );
          insert(node->se,&(node->se->pointsArray), a->array[i]);
        }
      }else{ //NW, SW
        if ( y > node-> upperBound_Y/2){ //NW
          printf("Inserting Node in Northwest\n" );
          insert(node->nw,&(node->nw->pointsArray), a->array[i]);

        }else{ //SW
          printf("Inserting Node in Southwest\n" );
          insert(node->sw,&(node->sw->pointsArray), a->array[i]);
        }
      }
    }
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

  root.lowerBound_X = 0;
  root.upperBound_X = 20;
  root.lowerBound_Y = 0;
  root.upperBound_Y = 20;


  //quadTree.root = &root;
  initArray(&root.pointsArray,3);

  Point pt1;
  pt1.x = 3;
  pt1.y = 4;
  Point pt2;
  pt2.x = 11;
  pt2.y = 20;
  Point pt3;
  pt3.x = 20;
  pt3.y = 4;
  Point pt4;
  pt4.x = 30;
  pt4.y = 4;

  insert(&root, &root.pointsArray, pt1);
  insert(&root, &root.pointsArray, pt2);
  insert(&root, &root.pointsArray, pt3);
  insert(&root, &root.pointsArray, pt4);
  return 1;
}
