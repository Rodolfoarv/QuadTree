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
//Point struct which will store points in the form (x,y)
typedef struct Points{
  int x;
  int y;
  int quadrant; //Which will determine the position of the point 1: NE  2:SE  3:SW  4:NW
} Point;

//Struct PointArray which will be used to store instances of the struct Point
typedef struct {
  Point *array;
  size_t used;
  size_t size;
} PointArray;


//Node struct is the main structure
typedef struct Nodes{
  struct Nodes *ne;             //NorthEast Node
  struct Nodes *nw;             //NorthWest Node
  struct Nodes *se;             //SouthEast Node
  struct Nodes *sw;             //SouthWest Node
  struct Nodes *parent;         //The node's parent
  unsigned int currentSize;
  unsigned int limitSize;            // Size of the current node
  unsigned int level;           //Level on the tree i.e (1,2,3,4,5)
  unsigned int upperBound_X;
  unsigned int lowerBound_X;
  unsigned int upperBound_Y;
  unsigned int lowerBound_Y;
  PointArray pointsArray;       //array that will store the points on a node
  PointArray pointsArrayMean; //Array which will be used to calculate the mean and all the points contained in that node

  //Neighbor List
  struct Nodes *leftNeighbor;
  struct Nodes *rightNeighbor;
  struct Nodes *bottomNeighbor;
  struct Nodes *topNeighbor;


} Node;

//The main structure, the root
//Struct QuadTree
typedef struct {
  Node *root;
} QuadTree;

/**********************
QuadTree routines
***********************/
void insert (Node *node, PointArray *a, Point element);       //Routine that inserts an item
void subdivision (Node *node, PointArray *a, Point element);  //Routine that will split the node in 4 nodes i.e (NE,SE,SW,NW)
void initArray(PointArray *a, size_t initialSize);            //Routine used to intialize PointArray
void freeArray(PointArray *a);                                //Routine used to free a PointArray
int getQuadrant(Node *node, Point *element);                  //GetQuadrant depending on point in order to distribute it on the nodes
void getPointsList(Node *node);                               //Get the list of points within a node, it will get the mean if it has childs
void getNodeMean(Node *node);                                 //Mean of all the points within a node
void getNeighborList(Node *node);                             //Get all the points that are neighbors to a determined node
void getListByLevel(Node *node, int level);                   //Get all the points depending on the level

//Neighbor Routines
void leftNeighborList(Node *node);
void leftNeighborListUtil(Node *node);
void rightNeighborList(Node *node);
void rightNeighborListUtil(Node *node);
void bottomNeighborList(Node *node);
void bottomNeighborListUtil(Node *node);
void topNeighborList(Node *node);
void topNeighborListUtil(Node *node);


void insert(Node *node, PointArray *a, Point element) {
  node->currentSize++;
  if (node->currentSize >= node->limitSize){
    printf("The tree has exceeded it's capacity\n" );
  }else{
    node->pointsArrayMean.array[node->pointsArrayMean.used++] = element;
    if (a-> size == 0){
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
    }else if (a->used == a->size-1){
      printf("Number of points exceeded, partitioning into nodes!\n" );
      subdivision(node, a, element);
      freeArray(a);
    }else{ //Insert into the original node
      printf("Inserting point x:%d y:%d \n", element.x, element.y );
      a->array[a->used++] = element;
    }
  }


}


void subdivision (Node *node, PointArray *a, Point element){
  a->array[a->used++] = element;

  //Allocate memory for the childs
  node->ne = (Node *)malloc(sizeof(Node));
  node->nw = (Node *)malloc(sizeof(Node));
  node->se = (Node *)malloc(sizeof(Node));
  node->sw = (Node *)malloc(sizeof(Node));

  //Set the parent of the nodes
  node->ne->parent = (Node *)malloc(sizeof(Node));
  node->nw->parent = (Node *)malloc(sizeof(Node));
  node->se->parent = (Node *)malloc(sizeof(Node));
  node->sw->parent = (Node *)malloc(sizeof(Node));
  node->ne->parent = node;
  node->nw->parent = node;
  node->se->parent = node;
  node->sw->parent = node;

  //Limit size
  node->ne->limitSize = node->limitSize - node->currentSize;
  node->nw->limitSize = node->limitSize - node->currentSize;
  node->se->limitSize = node->limitSize - node->currentSize;
  node->sw->limitSize = node->limitSize - node->currentSize;

  //Create the array of points for the childs
  initArray(&(node->ne->pointsArray),PARTITION);
  initArray(&(node->nw->pointsArray),PARTITION);
  initArray(&(node->se->pointsArray),PARTITION);
  initArray(&(node->sw->pointsArray),PARTITION);
  initArray(&(node->ne->pointsArrayMean),3000);
  initArray(&(node->nw->pointsArrayMean),3000);
  initArray(&(node->se->pointsArrayMean),3000);
  initArray(&(node->sw->pointsArrayMean),3000);

  //Set the level atribute for the childs
  node->ne->level = node->level + 1;
  node->nw->level = node->level + 1;
  node->se->level = node->level + 1;
  node->sw->level = node->level + 1;

  //Neighbors for the split
    //North East
      node->ne->leftNeighbor = node->nw;
      node->ne->rightNeighbor = NULL;
      node->ne->bottomNeighbor = node->se;
      node->ne->topNeighbor = NULL;

      //South East
      node->se->leftNeighbor = node->sw;
      node->se->rightNeighbor = NULL;
      node->se->bottomNeighbor = NULL;
      node->se->topNeighbor = node->ne;

      //South West
      node->sw->leftNeighbor = NULL;
      node->sw->rightNeighbor = node->se;
      node->sw->bottomNeighbor = NULL;
      node->sw->topNeighbor = node->nw;

      //North West
      node->nw->leftNeighbor = NULL;
      node->nw->rightNeighbor = node->ne;
      node->nw->bottomNeighbor = node->sw;
      node->nw->topNeighbor = NULL;



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


    }else{ //SE
      element->quadrant = 2;

    }
  }else{ //NW, SW
    if ( y > node-> upperBound_Y/2){ //NW
      element->quadrant = 4;

    }else{ //SW
      element->quadrant = 3;

    }
  }
  return element->quadrant;

}

void getPointsList(Node *node){
  if (node == NULL){
    printf("The node is NULL can't get any information about it\n" );
  }else{
    int i;
    printf("******** List of points inside this Node ********* \n" );
    printf("This node has a total of %zd Points\n", node->pointsArrayMean.used  );
    if (node->ne == NULL || node->nw == NULL || node->se == NULL || node->sw == NULL){
      for (i = 0; i < node->pointsArrayMean.used; i++){
        printf("(%d, %d)\n",node->pointsArrayMean.array[i].x,node->pointsArrayMean.array[i].y);
      }
    }else{
      getNodeMean(node);
    }
  }


}

void getNodeMean(Node *node){
  if (node->pointsArrayMean.used == 0){
    printf("This node doesn't have any values stored yet\n" );
  }else{
    int i;
    double x,y;
    for (i = 0; i < node->pointsArrayMean.used; i++){
      x+= node->pointsArrayMean.array[i].x;
      y+= node->pointsArrayMean.array[i].y;
    }
    x = x / node->pointsArrayMean.used;
    y = y / node->pointsArrayMean.used;
    printf("The node mean is (%f, %f)\n",x,y );
  }

}
void getNeighborList(Node *node){
  if (node == NULL){
    printf("This node doesn't exist\n" );
  }else{
    leftNeighborList(node);
    rightNeighborList(node);
    topNeighborList(node);
    bottomNeighborList(node);
  }


}

void leftNeighborList(Node *node){
  if (node->level == 1){
    if (node->leftNeighbor != NULL){
      leftNeighborListUtil(node->leftNeighbor);
    }
  }else if(node->level == 2){
    if (node->leftNeighbor == NULL){
      leftNeighborList(node->parent);
    }else{
      leftNeighborListUtil(node->leftNeighbor);
    }
  }else{
    leftNeighborList(node->parent);
  }
}

void leftNeighborListUtil(Node *node){
  if (node->ne != NULL || node->se != NULL){
    leftNeighborListUtil(node->ne);
    leftNeighborListUtil(node->se);
  }else{
    getPointsList(node);
  }
}



void rightNeighborList(Node *node){
  if (node->level == 1){
    if (node->rightNeighbor != NULL){
      rightNeighborListUtil(node->rightNeighbor);
    }
  }else if(node->level == 2){
    if (node->rightNeighbor == NULL){
      rightNeighborList(node->parent);
    }else{
      rightNeighborListUtil(node->rightNeighbor);
    }
  }else{
    rightNeighborList(node->parent);
  }
}

void rightNeighborListUtil(Node *node){

  if (node->nw != NULL || node->sw != NULL){
    rightNeighborList(node->nw);
    rightNeighborList(node->sw);
  }else{
    getPointsList(node);
  }
}

void topNeighborList(Node *node){
  if (node->level == 1){
    if (node->topNeighbor != NULL){
      topNeighborListUtil(node->topNeighbor);
    }
  }else if(node->level == 2){
    if (node->topNeighbor == NULL){
      topNeighborList(node->parent);
    }else{
      topNeighborListUtil(node->topNeighbor);
    }
  }else{
    topNeighborList(node->parent);
  }
}

void topNeighborListUtil(Node *node){
  if (node->se != NULL || node->sw != NULL){
    topNeighborListUtil(node->se);
    topNeighborListUtil(node->sw);
  }else{
    getPointsList(node);
  }
}

void bottomNeighborList(Node *node){
  if (node->level == 1){
    if (node->bottomNeighbor != NULL){
      bottomNeighborListUtil(node->bottomNeighbor);
    }
  }else if(node->level == 2){
    if (node->bottomNeighbor == NULL){
      bottomNeighborList(node->parent);
    }else{
      bottomNeighborListUtil(node->bottomNeighbor);
    }
  }else{
    bottomNeighborList(node->parent);
  }
}

void bottomNeighborListUtil(Node *node){
  if (node->ne != NULL || node->nw != NULL){
    topNeighborListUtil(node->ne);
    topNeighborListUtil(node->nw);
  }else{
    getPointsList(node);
  }
}

void getListByLevel(Node *node, int level){
  if (node == NULL){
    return;
  }else if(node->level == level){
    getPointsList(node);
  }else{
    getListByLevel(node->ne,level);
    getListByLevel(node->nw,level);
    getListByLevel(node->se,level);
    getListByLevel(node->sw,level);
  }
}


/**********************
Dynamic Point Array Structure
***********************/

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
  QuadTree quadTree;
  Node root;
  root.limitSize = 20;
  root.currentSize = 0;

  root.lowerBound_X = 0;
  root.upperBound_X = 20;
  root.lowerBound_Y = 0;
  root.upperBound_Y = 20;
  root.level = 0;
  quadTree.root = &root;
  initArray(&root.pointsArray,PARTITION);
  initArray(&root.pointsArrayMean,3000);



  printf("\n\n********************* Testing the routine insert ********************* \n" );

  Point pt1; pt1.x = 1 ; pt1.y = 1; insert(quadTree.root, &root.pointsArray, pt1);
  Point pt2; pt2.x = 2 ; pt2.y = 3; insert(quadTree.root, &root.pointsArray, pt2);
  Point pt3; pt3.x = 15 ; pt3.y = 17; insert(quadTree.root, &root.pointsArray, pt3);

  printf("\n\n********************* Testing the routine subdivision (inserting a 4th point) ********************* \n" );
  Point pt4; pt4.x = 10 ; pt4.y = 16; insert(quadTree.root, &root.pointsArray, pt4);
  Point pt5; pt5.x = 13 ; pt5.y = 13; insert(quadTree.root, &root.pointsArray, pt5);
  Point pt6; pt6.x = 9 ; pt6.y = 12; insert(quadTree.root, &root.pointsArray, pt6);
  Point pt7; pt7.x = 8 ; pt7.y = 12; insert(quadTree.root, &root.pointsArray, pt7);
  Point pt8; pt8.x = 5 ; pt8.y = 15; insert(quadTree.root, &root.pointsArray, pt8);
  Point pt9; pt9.x = 12 ; pt9.y = 17; insert(quadTree.root, &root.pointsArray, pt9);
  Point pt10; pt10.x = 11 ; pt10.y = 12; insert(quadTree.root, &root.pointsArray, pt10);

  printf("\n\n********************* Testing the routine mean on the north east, node level 2 ********************* \n" );
  getNodeMean(quadTree.root->ne);

  printf("\n\n********************* Testing the routine getPointsList with northWest Node (prints the mean)  ********************* \n" );
  getPointsList(quadTree.root->nw);

  printf("\n\n********************* Testing the routine getPointsList with NorthWest -> SouthEast (prints points)  ********************* \n" );
  getPointsList(quadTree.root->nw->se);

  printf("\n\n********************* Testing the routine getNeighborList ********************* \n" );
  getNeighborList(quadTree.root->se);

  printf("\n\n********************* Testing the routine getListByLevel ********************* \n" );
  getListByLevel(quadTree.root, 2);
  return 1;
}
