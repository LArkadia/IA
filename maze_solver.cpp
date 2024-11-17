#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
#include <climits>
using namespace std; 
//Definicion del nuevo dato
typedef long double ld;
//Comparadores con presecion de 10-9
const ld eps = 1e-9;
bool greaterld(ld a, ld b){return a-b > eps;}      
bool lessld(ld a, ld b){return b-a > eps;} 
// Laberinto
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

const int CELL_SIZE = 40; // Tamaño de cada celda en píxeles

// Estructura para el nodo
struct Node {
    int x, y; // Coordenadas y costo acumulado para llegar a este nodo
    ld cost;
    bool operator>(const Node &other) const { return greaterld(cost,other.cost); } // Comparador para la cola de prioridad
    bool operator<(const Node &other) const { return lessld(cost,other.cost); }
};

// Algoritmo Dijkstra
vector<sf::Vector2i> dijkstra(sf::Vector2i start, sf::Vector2i end) {
    int rows = maze.size();  // Número de filas
    int cols = maze[0].size(); // Número de columnas

    // Matriz de costos mínimos desde el nodo inicial
    vector<vector<ld>> dist(rows, vector<ld>(cols, INT_MAX));
    // Matriz para guardar el nodo previo en el camino más corto
    vector<vector<sf::Vector2i>> prev(rows, vector<sf::Vector2i>(cols, {-1, -1}));
    // Cola de prioridad para explorar los nodos
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    // Inicializar el nodo de inicio
    pq.push({start.x, start.y, (ld)0});
    dist[start.x][start.y] = (ld)0;

    // Direcciones posibles para moverse (derecha, abajo, izquierda, arriba)
    vector<sf::Vector2i> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0},{-1,-1},{1,1}};

    // Bucle principal del algoritmo
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        // Si llegamos al nodo final, termina el progrma
        if (sf::Vector2i(current.x, current.y) == end)
            break;

        // Explorar vecinos
        for (auto dir : directions) {
            int nx = current.x + dir.x; // Nueva posición en x
            int ny = current.y + dir.y; // Nueva posición en y

            // Verificar si el vecino está dentro de los límites y es transitable
            if (nx >= 0 && ny >= 0 && nx < rows && ny < cols && maze[nx][ny] == 0) {
                ld cost_x=(nx-current.x)*(nx-current.x);
                ld cost_y=(ny-current.y)*(ny-current.y);
                ld new_cost = current.cost+sqrt(cost_x+cost_y); // Costo acumulado 
                // Si encontramos un camino más corto, actualizamos
                if (new_cost < dist[nx][ny]) {
                    dist[nx][ny] = new_cost;
                    prev[nx][ny] = {current.x, current.y};
                    pq.push({nx, ny, new_cost});
                }
            }
        }
    }

    // Reconstrucción del camino desde el nodo final al inicio
    vector<sf::Vector2i> path;
    for (sf::Vector2i at = end; at != sf::Vector2i{-1, -1}; at = prev[at.x][at.y]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end()); // Se invierte el camino para ir de inicio a fin
    return path;
}

int main() {
    int rows = maze.size();  // Número de filas
    int cols = maze[0].size(); // Número de columnas

    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(cols * CELL_SIZE, rows * CELL_SIZE), "Dijkstra Maze Solver");

    sf::Vector2i start{-1, -1}, end{-1, -1}; // Coordenadas del inicio y fin
    bool selectingStart = true; // Bandera para determinar si estamos seleccionando el inicio

    // Bucle principal de la aplicación
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // Cerrar ventana

            // Detección de clics del mouse para inicio y fin
            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.y / CELL_SIZE; // Coordenada x en celdas
                int y = event.mouseButton.x / CELL_SIZE; // Coordenada y en celdas

                if (x >= 0 && y >= 0 && x < rows && y < cols && maze[x][y] == 0) {
                    if (selectingStart) {
                        start = {x, y}; // Asignar inicio
                        selectingStart = false; // Pasar a seleccionar el fin
                    } else {
                        end = {x, y}; // Asignar fin
                    }
                }
            }
        }

        window.clear(sf::Color::White); // Limpiar la ventana

        // Dibujar el laberinto
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(j * CELL_SIZE, i * CELL_SIZE);

                // Color de las celdas
                if (maze[i][j] == 1)
                    cell.setFillColor(sf::Color::Black); // Muros o barreras
                else
                    cell.setFillColor(sf::Color::White); // Espacio transitable 

                // Colorear inicio y fin
                if (start == sf::Vector2i(i, j))
                    cell.setFillColor(sf::Color::Green); // Inicio
                if (end == sf::Vector2i(i, j))
                    cell.setFillColor(sf::Color::Red); // Fin

                // Dibujar borde negro a cada celda
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::Black);

                window.draw(cell); // Dibujar celda
            }
        }

        // Dibujar el camino más corto si el inicio y fin están definidos
        if (start != sf::Vector2i{-1, -1} && end != sf::Vector2i{-1, -1}) {
            auto path = dijkstra(start, end);
            for (auto &pos : path) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(pos.y * CELL_SIZE, pos.x * CELL_SIZE);
                cell.setFillColor(sf::Color::Cyan); // Color del camino
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::Black);
                window.draw(cell); // Dibujar celda del camino
            }
        }

        window.display(); // Mostrar la ventana
    }

    return 0;
}

