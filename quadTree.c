/*----------------------------------------------------------
 * Examen práctico 1: QuadTree
 * Fecha: 15-Sep-2015
 * Autores:
 *           A01169701 Rodolfo Andrés Ramírez Valenzuela
 *           A01371743 Luis Eduardo Ballinas Aguilar
 *----------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#define SIZE 10



typedef struct Points{
  int x;
  int y;
} Point ;

typedef struct Node Node;

typedef struct Node{
  Point value;
  Node *northEast;
  Node *northWest;
  Node *southEast;
  Node *southWest;
  double prom;
}Node;


typedef struct Tree{
  Node *root;
} QuadTree;

Node insert(Node *parentRoot, Node *node);

Node insert(Node *parentRoot, Node *node){
  //We are going to compare with 4 ifs, the node will have 4 childs which will be divided in 4 zones
  // North east, North west, South east and South west, the position will depend if x is higher or less and y is higher or less
  // from the current node, i.e the root
  Node temp = *node;
  Node tempRoot = *parentRoot;

  if (temp.value.x > tempRoot.value.x ){ //then it will be northeast or southeast
    if (temp.value.y > tempRoot.value.y){ //then it will be northeast
      if(tempRoot.northEast){ //if northeast is null
        tempRoot.northEast = &temp;
        printf("North east inserted!");
      }else{
        printf("It has already inserted, don't even bother\n" );
        /*
        Node *returnStatement1 = tempRoot.northEast;
        Node *returnStatement2 = node;
        return insert(returnStatement1, returnStatement2);
        */
      }
    }else{ //then it will be southeast

    }
  }else{ //then it will be north west or south west
    if (temp.value.y > tempRoot.value.y){ //then it will be northwest

    }else{ //then it will be southwest

    }
  }


  if (!temp.northWest ){ //If this object is null
    printf("%d\n",temp.value.y );
  }

  return temp;


}

int main(){
  //Initilization of root and quad tree
  Point pt1;
  pt1.x = 0;
  pt1.y = 0;

  Node root;
  root.value = pt1;
  Node *rootPointer = &root;

  /****************************************** */

  //trying to insert a new node into the tree
  Point pt2;
  pt2.x = 1;
  pt2.y = 1;

  Node test;
  test.value = pt2;
  Node *testPointer = &test;

  insert(rootPointer, testPointer);






  return 1;

}
