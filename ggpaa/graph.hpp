#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> maze = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0}
};
int n,m;
const int CELL_SIZE = 40; // Tamaño de cada celda en píxeles
struct Node {
    int x, y, cost; // Coordenadas y costo acumulado para llegar a este nodo
    bool operator>(const Node &other) const { return cost > other.cost; } // Comparador para la cola de prioridad
};