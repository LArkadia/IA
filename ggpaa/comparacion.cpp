#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace std;
#include "graph.hpp"
#include "dfs.cpp"
#include "bfs.cpp"
#include "dijstra.cpp"
#include "a_start.cpp"

void drawMaze(sf::RenderWindow &window,int rows,int cols,sf::Vector2i start,sf::Vector2i end){
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
}
int main() {
    int rows = maze.size();  // Número de filas
    int cols = maze[0].size(); // Número de columnas

    // Crear ventanas
    sf::RenderWindow window(sf::VideoMode(cols * CELL_SIZE, rows * CELL_SIZE), "Maze Solver");

    sf::Vector2i start{-1, -1}, end{-1, -1}; // Coordenadas del inicio y fin
    int  selecting = 0; // Bandera 0 inicio 1 fin
    // Bucle principal de la aplicación
    sf::RenderWindow dfsWindow(sf::VideoMode(cols * CELL_SIZE, rows * CELL_SIZE), "DFS Maze Solver");
    sf::RenderWindow bfsWindow(sf::VideoMode(cols * CELL_SIZE, rows * CELL_SIZE), "BFS Maze Solver");
    sf::RenderWindow dijstraWindow(sf::VideoMode(cols * CELL_SIZE, rows * CELL_SIZE), "Dijstra GGpa Maze Solver");
    sf::RenderWindow a_startWindow(sf::VideoMode(cols * CELL_SIZE, rows * CELL_SIZE), "A_Start GGpa Maze Solver");
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
        bfsWindow.clear(sf::Color::White); // Limpiar la ventana
        dfsWindow.clear(sf::Color::White); // Limpiar la ventana
        dijstraWindow.clear(sf::Color::White); // Limpiar la ventana
        a_startWindow.clear(sf::Color::White); // Limpiar la ventana
        // Dibujar el laberinto
        drawMaze(window,rows,cols,start,end);
        drawMaze(bfsWindow,rows,cols,start,end);
        drawMaze(dfsWindow,rows,cols,start,end);
        drawMaze(dijstraWindow,rows,cols,start,end);
        drawMaze(a_startWindow,rows,cols,start,end);

        
        // Dibujar el camino más corto si el inicio y fin están definidos
        if (start != sf::Vector2i{-1, -1} && end != sf::Vector2i{-1, -1} && selecting==0) {
            auto path=dfs_aux(start,end);
            for (auto &pos : path) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(pos.y * CELL_SIZE, pos.x * CELL_SIZE);
                cell.setFillColor(sf::Color::Cyan); // Color del camino
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::Black);
                dfsWindow.draw(cell);
            }
            dfsWindow.display();
            path=bfs(start,end);
            for (auto &pos : path) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(pos.y * CELL_SIZE, pos.x * CELL_SIZE);
                cell.setFillColor(sf::Color::Cyan); // Color del camino
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::Black);
                bfsWindow.draw(cell);
            }
            bfsWindow.display();
            path=dijkstra(start,end);
            for (auto &pos : path) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(pos.y * CELL_SIZE, pos.x * CELL_SIZE);
                cell.setFillColor(sf::Color::Cyan); // Color del camino
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::Black);
                dijstraWindow.draw(cell);
            }
            dijstraWindow.display();
            pair<int,int>startP={start.x,start.y};
            pair<int,int>endP={end.x,end.y};
            auto path_a=AStar(startP,endP);
            for (auto &pos : path_a) {
                cout<<pos.first<<" "<<endl;
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(pos.second * CELL_SIZE, pos.first * CELL_SIZE);
                cell.setFillColor(sf::Color::Cyan); // Color del camino
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::Black);
                a_startWindow.draw(cell);
            }
            a_startWindow.display();
        }

        window.display(); // Mostrar la ventana
    }

    return 0;
}