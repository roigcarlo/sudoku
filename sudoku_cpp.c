/* Sudoku Breaker v0.2 by Skalgor */
#include <iostream>
#include <list>

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

//Mapped access
short ROW[CELLS] = {0,0,0,0,0,0,0,0,0,
                    1,1,1,1,1,1,1,1,1,
                    2,2,2,2,2,2,2,2,2,
                    3,3,3,3,3,3,3,3,3,
                    4,4,4,4,4,4,4,4,4,
                    5,5,5,5,5,5,5,5,5,
                    6,6,6,6,6,6,6,6,6,
                    7,7,7,7,7,7,7,7,7,
                    8,8,8,8,8,8,8,8,8};

short COL[CELLS] = {0,1,2,3,4,5,6,7,8,
                    0,1,2,3,4,5,6,7,8,
                    0,1,2,3,4,5,6,7,8,
                    0,1,2,3,4,5,6,7,8,
                    0,1,2,3,4,5,6,7,8,
                    0,1,2,3,4,5,6,7,8,
                    0,1,2,3,4,5,6,7,8,
                    0,1,2,3,4,5,6,7,8,
                    0,1,2,3,4,5,6,7,8};

short CHU[CELLS] = {0,0,0,1,1,1,2,2,2,
                    0,0,0,1,1,1,2,2,2,
                    0,0,0,1,1,1,2,2,2,
                    3,3,3,4,4,4,5,5,5,
                    3,3,3,4,4,4,5,5,5,
                    3,3,3,4,4,4,5,5,5,
                    6,6,6,7,7,7,8,8,8,
                    6,6,6,7,7,7,8,8,8,
                    6,6,6,7,7,7,8,8,8};

using namespace std;

class Node {

    public:
        short color;
        short index;

        short posible[9];
        short fix[9];

        Node()
        {
            color = 0;
            index = 0;
        }
};

class Graph {

    public:

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

        Graph()
        {
            solutions       = 0;
            recursion_level = 0;
            depht           = 0;
            elegible_size   = 0;
            elegible_itr    = 0;

            cout << "Allocating memory..." << endl;

            for(int i = 0; i < LENGHT; i++)
            {
                row_block[i] = -1;
                col_block[i] = -1;
                chu_block[i] = -1;
                max_block[i] =  9;
            }

            for(int i = 0; i < CELLS; i++)
            {
                cell[i] = new Node();
                cell[i]->color = 0;
                cell[i]->index = i;
            }
        }

        void read()
        {
            cout << "Reading graph..." << endl;

            int r_color;
            int index;

            for(int i = 0; i < CELLS; i++)
            {
                cin >> r_color;
                cell[i]->color = r_color;
                index = cell[i]->index;

                if (cell[i]->color != 0)
                {
                    BLOC_COLOR(row_block[ROW[index]],r_color-1);
                    BLOC_COLOR(col_block[COL[index]],r_color-1);
                    BLOC_COLOR(chu_block[CHU[index]],r_color-1);
                    max_block[r_color-1]--;
                    depht++;
                }
                else
                {
                    elegible_nodes[elegible_size++] = cell[i];
                }
            }
        }

        void print()
        {
            cout << "Printing graph..." << endl << endl;

            for(int i = 0; i < CELLS; i++)
            {
                if ((i % 9)==0 && i!=0) cout << endl;
                cout << cell[i]->color << " ";
            }
            cout << endl;
        }

    private:
};

void solve(Graph * sudo)
{
    sudo->recursion_level++;

    if(sudo->elegible_itr == sudo->elegible_size || sudo->depht == 81)
    {
        sudo->solutions++;
        //sudo->print();
        return;
    }

    Node * n = sudo->elegible_nodes[sudo->elegible_itr];

    //TODO: Sacar ThisNode
    for(int j = 1; j < 10; j++)
    {
        if( sudo->max_block[j-1] &&
            CHEK_COLOR(sudo->row_block[ROW[n->index]],j-1) &&
            CHEK_COLOR(sudo->col_block[COL[n->index]],j-1) &&
            CHEK_COLOR(sudo->chu_block[CHU[n->index]],j-1) )
        {
            sudo->max_block[j-1]--;
            BLOC_COLOR(sudo->row_block[ROW[n->index]],j-1);
            BLOC_COLOR(sudo->col_block[COL[n->index]],j-1);
            BLOC_COLOR(sudo->chu_block[CHU[n->index]],j-1);

            n->color = j;
            sudo->depht++;
            sudo->elegible_itr++;
            solve(sudo);
            sudo->elegible_itr--;
            sudo->depht--;
            n->color = 0;

            sudo->max_block[j-1]++;
            FREE_COLOR(sudo->row_block[ROW[n->index]],j-1);
            FREE_COLOR(sudo->col_block[COL[n->index]],j-1);
            FREE_COLOR(sudo->chu_block[CHU[n->index]],j-1);
        }
    }
}

int main( )
{
    std::cout << std::endl;
    std::cout << "Inicializando..." << std::endl;

    Graph * sudoku = new Graph();
    sudoku->read();

    solve(sudoku);

    cout << "RC: " << sudoku->recursion_level;
    cout << "SO: " << sudoku->solutions;
}
