#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
#include <climits>
using namespace std; 
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

// Algoritmo BFS
vector<sf::Vector2i> bfs(sf::Vector2i start, sf::Vector2i end) {
    int rows = maze.size();  // Número de filas
    int cols = maze[0].size(); // Número de columnas
    
    // Matriz de visitados
    vector<vector<int>> visited(rows, vector<int>(cols,0));
    // Matriz para guardar el nodo previo en el camino más corto
    vector<vector<sf::Vector2i>> prev(rows, vector<sf::Vector2i>(cols, {-1, -1}));
    // Cola para explorar los nodos
    queue<vector<int>>q;
    // Inicializar el nodo de inicio
    q.push({start.x, start.y});
    // Direcciones posibles para moverse (derecha, abajo, izquierda, arriba)
    vector<sf::Vector2i> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    prev[start.x][start.y]={-1,-1};
    visited[start.x][start.y]=1;
    while(q.size()){
        auto current=q.front();
        q.pop();
        for(auto dir:directions){
            int nx = current[0] + dir.x; // Nueva posición en x
            int ny = current[1] + dir.y; // Nueva posición en y
            if (nx >= 0 && ny >= 0 && nx < rows && ny < cols && maze[nx][ny] == 0){
                if(!visited[nx][ny]){
                    q.push({nx,ny});
                    visited[nx][ny]=1;
                    prev[nx][ny]={current[0],current[1]};
                }
            }
        }
    }
    // Reconstrucción del camino desde el nodo final al inicio
    vector<sf::Vector2i> path;
    for (sf::Vector2i at = end; at != sf::Vector2i{-1, -1}; at = prev[at.x][at.y]) {
        path.push_back(at);
    }
    cout<<endl;
    reverse(path.begin(), path.end()); // Se invierte el camino para ir de inicio a fin
    return path;
}

int main() {
    int rows = maze.size();  // Número de filas
    int cols = maze[0].size(); // Número de columnas

    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(cols * CELL_SIZE, rows * CELL_SIZE), "Dijkstra Maze Solver");

    sf::Vector2i start{-1, -1}, end{-1, -1}; // Coordenadas del inicio y fin
    int  selecting = 0; // Bandera 0 inicio 1 fin

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
                    if (selecting==0) {
                        start = {x, y}; // Asignar inicio
                        end={-1,-1};
                        selecting^=1; // Pasar a seleccionar el fin
                    } else {
                        end = {x, y}; // Asignar fin
                        selecting^=1;// Pasar a seleccionar inicio, Si continua
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
            auto path = bfs(start, end);
            for (auto &pos : path) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(pos.y * CELL_SIZE, pos.x * CELL_SIZE);
                cout<<pos.x<<" "<<pos.y<<endl;
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

