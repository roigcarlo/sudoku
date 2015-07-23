/* Sudoku Breaker v0.4 Ophelia */
/* By Charlie contact at roigcarlo@gmail.com 2005-2013 */

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define DEBUG 0

#define LENGHT 9
#define CELLS  LENGHT * LENGHT

#define BLOC_COLOR(i,j) i &= ~(1<<(j))
#define FREE_COLOR(i,j) i |=  (1<<(j))
#define CHEK_COLOR(i,j) i &   (1<<(j))

#define OFFSET CELLS

//Mapped access
unsigned char DAT[3][CELLS] = {
  {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8},
  {0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8},
  {0,0,0,1,1,1,2,2,2,0,0,0,1,1,1,2,2,2,0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,3,3,3,4,4,4,5,5,5,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,6,6,6,7,7,7,8,8,8,6,6,6,7,7,7,8,8,8}
};

typedef struct {

  short color;
  short index;

  short posible[9];
  short fix[9];

} Node;

typedef struct {

  int solutions;
  int recursion_level;
  int depht;
  int elegible_size;
  int elegible_itr;

  int row_block[LENGHT];
  int col_block[LENGHT];
  int chu_block[LENGHT];
  int max_block[LENGHT];

  Node * cell[CELLS];
  Node * elegible_nodes[CELLS];

} Graph;

void InitializeGraph(Graph * G)
{
  int i;

  G->solutions            = 0;
  G->recursion_level      = 0;
  G->depht                = 0;
  G->elegible_size        = 0;
  G->elegible_itr         = 0;

  printf("Allocating memory...\n");

  for(i = 0; i < LENGHT; i++) {
    G->row_block[i] = -1;
    G->col_block[i] = -1;
    G->chu_block[i] = -1;
    G->max_block[i] =  9;
  }

  for(i = 0; i < CELLS; i++) {
    G->cell[i] = (Node *)malloc(sizeof(Node));
    G->cell[i]->color = 0;
    G->cell[i]->index = i;
  }
}

void ReadGraph(Graph * G)
{
  printf("Reading graph...\n");

  int r_color;
  int index;
  int i;

  for(i = 0; i < CELLS; i++) {
    scanf("%u", (unsigned int*)&r_color);

    G->cell[i]->color = r_color;
    index = G->cell[i]->index;

    if (G->cell[i]->color != 0) {
      unsigned char * ROW = &DAT[0][index];
      unsigned char * COL = ROW + OFFSET;
      unsigned char * CHU = COL + OFFSET;

      BLOC_COLOR(G->row_block[*ROW],r_color-1);
      BLOC_COLOR(G->col_block[*COL],r_color-1);
      BLOC_COLOR(G->chu_block[*CHU],r_color-1);

      G->max_block[r_color-1]--;
      G->depht++;
    } else {
      G->elegible_nodes[G->elegible_size++] = G->cell[i];
    }
  }
}

void PrintGraph(Graph * G)
{
  printf("Printing graph...\n");

  int i;

  for(i = 0; i < CELLS; i++) {
    if ((i % 9)==0 && i!=0) printf("\n");
    printf("%d ",G->cell[i]->color);
  }

  printf("\n");
}

void Solve(Graph * sudo)
{
  int j;

  sudo->recursion_level++;

  if(sudo->elegible_itr == sudo->elegible_size || sudo->depht == 81) {
    sudo->solutions++;
    return;
  }

  Node * n = sudo->elegible_nodes[sudo->elegible_itr];

  unsigned char * ROW;
  unsigned char * COL;
  unsigned char * CHU;

  for(j = 0; j < 9; j++) {
    ROW = &DAT[0][n->index];
    COL = ROW + OFFSET;
    CHU = COL + OFFSET;

    int & MAB = sudo->max_block[j];

    if( MAB &&
        CHEK_COLOR(sudo->row_block[*ROW],j) &&
        CHEK_COLOR(sudo->col_block[*COL],j) &&
        CHEK_COLOR(sudo->chu_block[*CHU],j) ) {

      MAB--;

      BLOC_COLOR(sudo->row_block[*ROW],j);
      BLOC_COLOR(sudo->col_block[*COL],j);
      BLOC_COLOR(sudo->chu_block[*CHU],j);

      n->color = j+1;
      sudo->depht++;
      sudo->elegible_itr++;

      Solve(sudo);

      sudo->elegible_itr--;
      sudo->depht--;
      n->color = 0;

      MAB++;

      FREE_COLOR(sudo->row_block[*ROW],j);
      FREE_COLOR(sudo->col_block[*COL],j);
      FREE_COLOR(sudo->chu_block[*CHU],j);
    }
  }
}

int main( )
{
  printf("Initializing...\n");

  Graph * sudoku = (Graph*)malloc(sizeof(Graph));
  
  InitializeGraph(sudoku);
  ReadGraph(sudoku);
  Solve(sudoku);

  printf("RC: %d\n",sudoku->recursion_level);
  printf("SO: %d\n",sudoku->solutions);
}
