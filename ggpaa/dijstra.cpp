vector<sf::Vector2i> dijkstra(sf::Vector2i start, sf::Vector2i end) {
    int rows = maze.size();  // Número de filas
    int cols = maze[0].size(); // Número de columnas

    // Matriz de costos mínimos desde el nodo inicial
    vector<vector<int>> dist(rows, vector<int>(cols, INT_MAX));
    // Matriz para guardar el nodo previo en el camino más corto
    vector<vector<sf::Vector2i>> prev(rows, vector<sf::Vector2i>(cols, {-1, -1}));
    // Cola de prioridad para explorar los nodos
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    // Inicializar el nodo de inicio
    pq.push({start.x, start.y, 0});
    dist[start.x][start.y] = 0;

    // Direcciones posibles para moverse (derecha, abajo, izquierda, arriba)
    vector<sf::Vector2i> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    // Bucle principal del algoritmo
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        // Si llegamos al nodo final, terminamos
        if (sf::Vector2i(current.x, current.y) == end)
            break;

        // Explorar los vecinos
        for (auto dir : directions) {
            int nx = current.x + dir.x; // Nueva posición en x
            int ny = current.y + dir.y; // Nueva posición en y

            // Verificar si el vecino está dentro de los límites y es transitable
            if (nx >= 0 && ny >= 0 && nx < rows && ny < cols && maze[nx][ny] == 0) {
                int new_cost = current.cost + 1; // Costo acumulado
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
    reverse(path.begin(), path.end()); // Invertimos el camino para que vaya del inicio al final
    return path;
}