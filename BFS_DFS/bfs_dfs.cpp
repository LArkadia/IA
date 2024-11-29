#include </usr/include/SFML/Graphics.hpp>
#include <queue>
#include <stack>
#include <vector>
#include <iostream>

const int GRID_SIZE = 20;
const int CELL_SIZE = 30;

// Representación del laberinto inicial
std::vector<std::string> maze = {
    "_#___#_#________________",
    "_#_#____________________",
    "___#####_______________#",
    "#_######______________#_",
    "___#___#______________#_",
    "___#_#__#_____________#_",
    "#__#_#_#_____________#__",
    "#____#__#__#_________##_",
    "________________________",
    "________###_____________",
    "_____#__________________",
    "_____#__#___###_________",
    "___________#_________#__",
    "#___####___#___#_______#",
    "___________#__#_______#_",
    "#_########__##__######__",
    "#_______________________#",
    "___#_#_###_____________#",
    "___#________#___________",
    "_________#__#___________"
};

struct Node {
    int x, y;
};

// Direcciones posibles: derecha, izquierda, abajo, arriba
std::vector<Node> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// Función para validar si una celda es válida y no visitada
bool isValid(int x, int y, const std::vector<std::vector<bool>>& visited) {
    return x >= 0 && y >= 0 && x < GRID_SIZE && y < GRID_SIZE && 
           maze[y][x] == '_' && !visited[y][x];
}

// Función BFS
std::vector<Node> bfs(Node start, Node goal) {
    std::queue<std::vector<Node>> q;
    std::vector<std::vector<bool>> visited(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
    q.push({start});
    visited[start.y][start.x] = true;

    while (!q.empty()) {
        auto path = q.front(); q.pop();
        Node current = path.back();

        if (current.x == goal.x && current.y == goal.y) {
            return path;
        }

        for (auto dir : directions) {
            int nx = current.x + dir.x;
            int ny = current.y + dir.y;

            if (isValid(nx, ny, visited)) {
                visited[ny][nx] = true;
                auto newPath = path;
                newPath.push_back({nx, ny});
                q.push(newPath);
            }
        }
    }
    return {};
}

// Función DFS
std::vector<Node> dfs(Node start, Node goal) {
    std::stack<std::vector<Node>> s;
    std::vector<std::vector<bool>> visited(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
    s.push({start});
    visited[start.y][start.x] = true;

    while (!s.empty()) {
        auto path = s.top(); s.pop();
        Node current = path.back();

        if (current.x == goal.x && current.y == goal.y) {
            return path;
        }

        for (auto dir : directions) {
            int nx = current.x + dir.x;
            int ny = current.y + dir.y;

            if (isValid(nx, ny, visited)) {
                visited[ny][nx] = true;
                auto newPath = path;
                newPath.push_back({nx, ny});
                s.push(newPath);
            }
        }
    }
    return {};
}

int main() {
    sf::RenderWindow window(sf::VideoMode(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE), "Maze Solver");
    Node start{-1, -1}, goal{-1, -1};
    bool startSet = false, goalSet = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x / CELL_SIZE;
                int y = event.mouseButton.y / CELL_SIZE;

                if (!startSet && maze[y][x] == '_') {
                    start = {x, y};
                    startSet = true;
                } else if (!goalSet && maze[y][x] == '_') {
                    goal = {x, y};
                    goalSet = true;
                }
            }
        }

        window.clear(sf::Color::White);

        // Dibujar laberinto
        for (int y = 0; y < GRID_SIZE; ++y) {
            for (int x = 0; x < GRID_SIZE; ++x) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);

                if (maze[y][x] == '#') {
                    cell.setFillColor(sf::Color::Black);
                } else {
                    cell.setFillColor(sf::Color::White);
                }

                if (startSet && x == start.x && y == start.y) {
                    cell.setFillColor(sf::Color::Green);
                }
                if (goalSet && x == goal.x && y == goal.y) {
                    cell.setFillColor(sf::Color::Red);
                }
                window.draw(cell);
            }
        }

        // Dibujar caminos BFS y DFS
        if (startSet && goalSet) {
            auto pathBFS = bfs(start, goal);
            auto pathDFS = dfs(start, goal);

            for (const auto& node : pathBFS) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(node.x * CELL_SIZE, node.y * CELL_SIZE);
                cell.setFillColor(sf::Color::Red);
                window.draw(cell);
            }

            for (const auto& node : pathDFS) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(node.x * CELL_SIZE, node.y * CELL_SIZE);
                if (std::find(pathBFS.begin(), pathBFS.end(), node) != pathBFS.end()) {
                    cell.setFillColor(sf::Color::Magenta);
                } else {
                    cell.setFillColor(sf::Color::Blue);
                }
                window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}
