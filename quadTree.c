/*----------------------------------------------------------
 * Examen práctico 1: QuadTree
 * Fecha: 15-Sep-2015
 * Autores:
 *           A01169701 Rodolfo Andrés Ramírez Valenzuela
 *           A01371743 Luis Eduardo Ballinas Aguilar
 *----------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#define PARTITION 3
/********************************
QuadTree
*********************************/
//Point struct
typedef struct Points{
  int x;
  int y;
  int quadrant; //Which will determine the position of the point 1: NE  2:SE  3:SW  4:NW
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
  PointArray pointsArrayMean; //Array which will be used to calculate the mean and all the points contained in that node
} Node;

//Struct QuadTree
typedef struct QuadTree{
  Node *root;
} QuadTree;

/**********************
QuadTree routines
***********************/
void insert (Node *node, PointArray *a, Point element);
void subdivision (Node *node, PointArray *a, Point element);
int size (Point arr[]);
void initArray(PointArray *a, size_t initialSize);
void freeArray(PointArray *a);
int getQuadrant(Node *node, Point *element);
void getPointsList(Node *node); //Consulta de datos (local)
void getNodeMean(Node *node);

void insert(Node *node, PointArray *a, Point element) {
  //printf(" Lower bound: %d Upper bound: %d con el punto %d %d ", node->lowerBound_X, node->upperBound_X, element.x, element.y);
  //printf("Size: %zd Used: %zd\n", a->size, a->used );
  node->pointsArrayMean.array[node->pointsArrayMean.used++] = element;
  if (a-> size == 0){
    printf("Inserting point x:%d y:%d \n", element.x, element.y );
    int quadrant = getQuadrant(node, &element);
    if (quadrant == 1){
      insert(node->ne,&(node->ne->pointsArray),element);
    }else if (quadrant == 2){
      insert(node->se,&(node->se->pointsArray),element);
    }else if (quadrant == 3){
      insert(node->sw,&(node->sw->pointsArray),element);
    }else{
      insert(node->nw,&(node->nw->pointsArray),element);

    }
  } if (a->used == a->size-1){
    printf("PARTITIONING INTO CHILDS!\n" );
    subdivision(node, a, element);
    freeArray(a);
  }else{ //Insert into the original node
    a->array[a->used++] = element;
  }
}


void subdivision (Node *node, PointArray *a, Point element){
  //printf("\nPARTITIONING NODE\n" );
  a->array[a->used++] = element;

  //Allocate memory for the childs
  node->ne = (Node *)malloc(sizeof(Node));
  node->nw = (Node *)malloc(sizeof(Node));
  node->se = (Node *)malloc(sizeof(Node));
  node->sw = (Node *)malloc(sizeof(Node));

  //Create the array of points for the childs
  initArray(&(node->ne->pointsArray),PARTITION);
  initArray(&(node->nw->pointsArray),PARTITION);
  initArray(&(node->se->pointsArray),PARTITION);
  initArray(&(node->sw->pointsArray),PARTITION);
  initArray(&(node->ne->pointsArrayMean),PARTITION);
  initArray(&(node->nw->pointsArrayMean),PARTITION);
  initArray(&(node->se->pointsArrayMean),PARTITION);
  initArray(&(node->sw->pointsArrayMean),PARTITION);


  //Set the width and height for the childs
  //NORTH EAST
  node->ne->upperBound_X = node->upperBound_X;
  node->ne->lowerBound_X = (node->upperBound_X + node->lowerBound_X) / 2;
  node->ne->upperBound_Y = node->upperBound_Y;
  node->ne->lowerBound_Y = (node->upperBound_Y + node->lowerBound_Y)/2;
  /*
  printf("***** NorthEAST ********\n" );
  printf("New lowerBound_X : %d   New upperBound_X:  %d\n",node->ne->lowerBound_X,node->ne->upperBound_X );
  printf("New lowerBound_Y : %d   New upperBound_Y:  %d\n\n\n",node->ne->lowerBound_Y,node->ne->upperBound_Y );
  */
  //SOUTH EAST
  node->se->upperBound_X = node->upperBound_X;
  node->se->lowerBound_X = (node->upperBound_X + node->lowerBound_X) / 2;
  node->se->upperBound_Y = (node->upperBound_Y + node->lowerBound_Y) / 2;
  node->se->lowerBound_Y = node->lowerBound_Y;
  /*
  printf("***** SouthEAST ********\n" );
  printf("New lowerBound_X : %d   New upperBound_X:  %d\n",node->se->lowerBound_X,node->se->upperBound_X );
  printf("New lowerBound_Y : %d   New upperBound_Y:  %d\n\n\n",node->se->lowerBound_Y,node->se->upperBound_Y );
*/
  //NORTH WEST
  node->nw->upperBound_X = (node->upperBound_X + node->lowerBound_X)/2;
  node->nw->lowerBound_X = node->lowerBound_X;
  node->nw->upperBound_Y = node->upperBound_Y;
  node->nw->lowerBound_Y = (node->upperBound_Y + node->lowerBound_Y)/2;
  /*
  printf("***** NorthWest ********\n" );
  printf("New lowerBound_X : %d   New upperBound_X:  %d\n",node->nw->lowerBound_X,node->nw->upperBound_X );
  printf("New lowerBound_Y : %d   New upperBound_Y:  %d\n\n\n",node->nw->lowerBound_Y,node->nw->upperBound_Y );
  */

  //SOUTH WEST
  node->sw->upperBound_X = (node->upperBound_X + node->lowerBound_X)/2;
  node->sw->lowerBound_X = node->lowerBound_X;
  node->sw->upperBound_Y = (node->upperBound_Y + node->lowerBound_Y) / 2;
  node->sw->lowerBound_Y = node->lowerBound_Y;
  /*
  printf("***** SouthWest ********\n" );
  printf("New lowerBound_X : %d   New upperBound_X:  %d\n",node->sw->lowerBound_X,node->sw->upperBound_X );
  printf("New lowerBound_Y : %d   New upperBound_Y:  %d\n\n\n",node->sw->lowerBound_Y,node->sw->upperBound_Y );
  */

  //Distribute the points between the children
  int i,x,y;
  for (i = 0; i < a->size; i++) {
    x = a->array[i].x;
    y = a->array[i].y;
    //printf("Punto x:%d  y:%d ",x,y );
    if (x > (node->upperBound_X + node->lowerBound_X)/2){ //NE or SE
      if (y > (node -> upperBound_Y + node->lowerBound_Y) / 2){ //NE
        insert(node->ne,&(node->ne->pointsArray), a->array[i]);
        printf("Inserting Node in Northeast Point: x :%d  y: %d \n" ,x,y ); //1,2,3
      }else{ //SE
        printf("Inserting Node in Southeast Point: x :%d  y: %d \n",x,y);
        insert(node->se,&(node->se->pointsArray), a->array[i]);
      }
    }else{ //NW, SW
      if ( y > (node-> upperBound_Y + node->lowerBound_Y)/2){ //NW
        printf("Inserting Node in Northwest Point: x :%d  y: %d \n",x,y );
        insert(node->nw,&(node->nw->pointsArray), a->array[i]);

      }else{ //SW
        printf("Inserting Node in Southwest Point: x :%d  y: %d \n",x,y );
        insert(node->sw,&(node->sw->pointsArray), a->array[i]);
      }
    }
  }
  printf("END OF PARTITIONING\n" );
}

int getQuadrant(Node *node, Point *element){
  int x, y;
  x = element->x;
  y = element->y;
  if (x > node->upperBound_X/2){ //NE or SE
    if (y > node -> upperBound_Y / 2){ //NE
      element->quadrant = 1;
    //printf("Point x: %d   y: %d   Inserting Node in Northeast\n",x,y );

    }else{ //SE
      element->quadrant = 2;
      //  printf("Point x: %d   y: %d   Inserting Node in Southeast\n",x,y );
    }
  }else{ //NW, SW
    if ( y > node-> upperBound_Y/2){ //NW
      element->quadrant = 4;
        //printf("Point x: %d   y: %d   Inserting Node in Northwest\n",x,y );
    }else{ //SW
      element->quadrant = 3;
        //printf("Point x: %d   y: %d   Inserting Node in Southwest\n",x,y );
    }
  }
  return element->quadrant;

}

void getPointsList(Node *node){
  int i;
  printf("******** List of points inside this Node ********* \n" );
  printf("This node has a total of %zd Points\n", node->pointsArrayMean.used  );
  for (i = 0; i < node->pointsArrayMean.used; i++){
    printf("(%d, %d)\n",node->pointsArrayMean.array[i].x,node->pointsArrayMean.array[i].y);
  }
}

void getNodeMean(Node *node){
  if (node->pointsArrayMean.used == 0){
    printf("This node doesn't have any values stored yet\n" );
  }else{
    int i;
    double x,y;
    for (i = 0; i < node->pointsArrayMean.used; i++){
      printf("Point :%d , %d\n",node->pointsArrayMean.array[i].x,node->pointsArrayMean.array[i].y );
      x+= node->pointsArrayMean.array[i].x;
      y+= node->pointsArrayMean.array[i].y;
    }
    x = x / node->pointsArrayMean.used;
    y = y / node->pointsArrayMean.used;
    printf("The node mean is (%f, %f)\n",x,y );
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
  a->array = (Point *)malloc((initialSize+1) * sizeof(Point));
  a->used = 0;
  a->size = initialSize+1;
}

void freeArray(PointArray *a) {
  free(a->array);
  //a->array = NULL;
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
  initArray(&root.pointsArray,PARTITION);
  initArray(&root.pointsArrayMean,3000);

  Point pt1; pt1.x = 12 ; pt1.y = 15;
  Point pt2; pt2.x = 11; pt2.y = 20;
  Point pt3; pt3.x = 15; pt3.y = 11;
  Point pt4; pt4.x = 20; pt4.y = 20;
  Point pt5; pt5.x = 10; pt5.y = 5;
  Point pt6; pt6.x = 3; pt6.y = 4;
  Point pt7; pt7.x = 12; pt7.y = 20;

/*
Point pt8; pt8.x = 5; pt8.y = 5;
Point pt9; pt9.x = 20; pt9.y = 20;
Point pt10; pt10.x = 10; pt10.y = 5;
Point pt11; pt11.x = 6; pt11.y = 6;
Point pt12; pt12.x = 7; pt12.y = 7;
Point pt13; pt13.x = 8; pt13.y = 8;

*/
  insert(&root, &root.pointsArray, pt1);
  insert(&root, &root.pointsArray, pt2);
  insert(&root, &root.pointsArray, pt3);
  insert(&root, &root.pointsArray, pt4);
  insert(&root, &root.pointsArray, pt5);
  insert(&root, &root.pointsArray, pt6);
  insert(&root, &root.pointsArray, pt7);


  //printf("%d\n",root.ne->sw->pointsArray.array[0].y );
  printf("%d\n",root.ne->nw->pointsArray.array[0].x );
  getNodeMean(root.ne->ne);
  getPointsList(root.ne);
/*


  insert(&root, &root.pointsArray, pt7);
  insert(&root, &root.pointsArray, pt8);
  insert(&root, &root.pointsArray, pt11);
    printf("*********** %d\n",root.se->pointsArray.array[0].x );
  insert(&root, &root.pointsArray, pt12);
  //insert(&root, &root.pointsArray, pt9);
  //insert(&root, &root.pointsArray, pt10);

*/
  return 1;
}
