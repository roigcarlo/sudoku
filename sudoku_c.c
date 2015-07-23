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

#define FETCHTIME(A,B) ((B.tv_sec  - A.tv_sec) * 1000000u + B.tv_usec - A.tv_usec) / 1.e6;

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

  G->solutions        = 0;
  G->recursion_level  = 0;
  G->depht            = 0;
  G->elegible_size    = 0;
  G->elegible_itr     = 0;

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

  int & solve_row = sudo->row_block[DAT[0][n->index]];
  int & solve_col = sudo->col_block[DAT[1][n->index]];
  int & solve_chu = sudo->chu_block[DAT[2][n->index]];

  int solve_mrg = (solve_row & solve_col & solve_chu);

  for(j = 0; j < 9; j++) {

    int & MAB = sudo->max_block[j];

    if( MAB && CHEK_COLOR(solve_mrg,j)) {

      MAB--;

      BLOC_COLOR(solve_row,j);
      BLOC_COLOR(solve_col,j);
      BLOC_COLOR(solve_chu,j);

      n->color = j+1;
      sudo->depht++;
      sudo->elegible_itr++;

      Solve(sudo);

      sudo->elegible_itr--;
      sudo->depht--;
      n->color = 0;

      FREE_COLOR(solve_row,j);
      FREE_COLOR(solve_col,j);
      FREE_COLOR(solve_chu,j);

      MAB++;
    }
  }
}

int main( )
{
  struct timeval str, end;
  float dtn;

  printf("Initializing...\n");

  Graph * sudoku = (Graph*)malloc(sizeof(Graph));
  
  InitializeGraph(sudoku);
  ReadGraph(sudoku);

  gettimeofday(&str, NULL);
  Solve(sudoku);
  gettimeofday(&end, NULL);

  dtn = FETCHTIME(str,end);

  printf("RC: %d\n",sudoku->recursion_level);
  printf("SO: %d\n",sudoku->solutions);
  printf("\n");
  printf("Total  Duration: %.10f\n",dtn);
  printf("PerRec Duration: %.10f\n",dtn/sudoku->recursion_level);
  printf("PerSol Duration: %.10f\n",dtn/sudoku->solutions);
}
