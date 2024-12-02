bool isValidCell(int x, int y) {
    return x >= 0 && y >= 0 && x <n && y <m && maze[x][y] == 0;
}
std::vector<std::pair<int, int>>AStar(const std::pair<int, int>& start, const std::pair<int, int>& goal) {
    auto heuristic = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return abs(a.first - b.first) + abs(a.second - b.second);
    };
    n = maze.size();  // Número de filas
    m= maze[0].size();
    std::priority_queue<std::pair<int, std::pair<int, int>>, 
                        std::vector<std::pair<int, std::pair<int, int>>>, 
                        std::greater<>> pq;
    std::map<std::pair<int, int>, int> gScore;
    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;
    gScore[start] = 0;
    pq.push({heuristic(start, goal), start});
    cameFrom[start] = {-1, -1};
    const std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    while (!pq.empty()) {
        auto current = pq.top().second;
        
        pq.pop();
        if (current == goal) {
            std::vector<std::pair<int, int>> path;
            for (auto at = goal; at != std::pair<int, int>{-1, -1}; at = cameFrom[at]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (const auto& dir : directions) {
            std::pair<int, int> neighbor = {current.first + dir.first, current.second + dir.second};
            if (isValidCell(neighbor.first, neighbor.second)) {
                int tentativeGScore = gScore[current] + 1;
                if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                    gScore[neighbor] = tentativeGScore;
                    pq.push({tentativeGScore + heuristic(neighbor, goal), neighbor});
                    cameFrom[neighbor] = current;
                }
            }
        }
    }
    return {};
}
