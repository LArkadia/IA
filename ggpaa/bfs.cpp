#include "bfs.hpp"

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
    reverse(path.begin(), path.end()); // Se invierte el camino para ir de inicio a fin
    return path;
}