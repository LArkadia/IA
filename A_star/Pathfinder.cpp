#include "Pathfinder.hpp"

Cord Pathfinder::Get_lower_open(){
    if (open.empty()) {
        return { UINT16_MAX, UINT16_MAX }; // Invalid coordinate
    }

    // Initialize with the first element
    Cord lowest_cord = open.front();
    uint lowest_weight = grid[lowest_cord.x][lowest_cord.y]->Get_weight();

    // Find the node with the lowest weight
    for (Cord& c : open) {
        uint current_weight = grid[c.x][c.y]->Get_weight();
        if (current_weight < lowest_weight) {
            lowest_weight = current_weight;
            lowest_cord = c;
        }
    }
    return lowest_cord;
}
//void Pathfinder::Check_neightbours(Cord current){}

void Pathfinder::Check_neightbours(Cord current) {
    static const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    static const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int dir = 0; dir < 8; ++dir) {
        int nx = current.x + dx[dir];
        int ny = current.y + dy[dir];

        // Boundary checks
        if (nx < 0 || ny < 0 || nx >= grid_mask.size() || ny >= grid_mask[0].size()) {
            continue;
        }

        Cord neighbour = { static_cast<uint16_t>(nx), static_cast<uint16_t>(ny) };

        // Skip if not passable or already closed
        if (!grid_mask[nx][ny] || std::find(closed.begin(), closed.end(), neighbour) != closed.end())  {
            continue;
        }

        uint tentative_g = grid[current.x][current.y]->Get_accumulated() + movement_function(neighbour,current); // Assuming uniform cost

        // If neighbor is not in open list, add it
        if (std::find(open.begin(), open.end(), neighbour) == open.end()) {
            grid[nx][ny] = new A_star_cell(current, neighbour, target, tentative_g, heuristic_function);
            grid[nx][ny]->Set_parent(current);
            open.push_back(neighbour);
        } else {
            // Update if a better path is found
            if (tentative_g < grid[nx][ny]->Get_accumulated()) {
                grid[nx][ny]->Update(tentative_g,tentative_g + grid[nx][ny]->Get_heuristic(),current);
            }
        }
    }
}

uint16_t Pathfinder::A_star_cell::last_id = 0;
Pathfinder::Pathfinder(String map_file_path)
{
    start_counter = false;
    path_found = false;
    std::ifstream infile(map_file_path);
    if (!infile.is_open()) {
        std::cerr << "Error: Can't open file: " << map_file_path << std::endl;
        return;
    }

    String line;
    // Obtener dimensiones
    if (std::getline(infile, line)) {
        std::istringstream iss(line);
        int rows, cols; 
        if (!(iss >> rows >> cols)) {
            std::cerr << "Error: Wrong format for the header in file: " << map_file_path << std::endl;
            infile.close();
            return;
        }

        grid_mask.resize(cols, Vector<bool>(rows, false));
        grid.resize(cols, Vector<A_star_cell*>(rows, nullptr));

        // Load Map
        int current_row = 0;
        while (std::getline(infile, line) && current_row < rows) {
            if (line.length() < static_cast<size_t>(cols)) {
                std::cerr << "Warning: line " << current_row + 2 << " is incomplete. Filling with '#' " << std::endl;
            }
            for (int col = 0; col < cols; ++col) {
                if (col < static_cast<int>(line.length())) {
                    char ch = line[col];
                    if (ch != '#') {
                        grid_mask[col][current_row] = true; 
                    }
                } else {
                    grid_mask[col][current_row] = false;
                }
            }
            current_row++;
        }
        // Validate size
        if (current_row < rows) {
            std::cerr << "Warning: Map dimensions are wrong. Filling with '#'." << std::endl;
        }
    } else {
        std::cerr << "Error: void file: " << map_file_path << std::endl;
    }
    infile.close();
    start_counter = 0;
}

Pathfinder::Pathfinder(String map_file_path, double (*heuristic)(Cord target, Cord current),double (*movement)(Cord target, Cord current)):Pathfinder(map_file_path){
    heuristic_function = heuristic;
    movement_function = movement;
}

Vector<Vector<bool>> Pathfinder::Get_grid_mask(){
        return grid_mask;
    }

    void Pathfinder::Set_start(Cord start){
        this->start = start;
        if (start_counter < 2){
            start_counter++;
        }
    }

    void Pathfinder::Set_target(Cord end){
        this->target = end;
        if (start_counter < 2){
            start_counter++;
        }
    }

bool Pathfinder::Step() {
//std::cout << "DF 1" <<std::endl;
    if (start_counter < 2) {
        std::cerr << "Error: Can't start, missing start or target" << std::endl;
        return false;
    }
//std::cout << "DF 2" <<std::endl;
    // Initialize the start node if the open list is empty
    if (open.empty()) {
//std::cout << "DF 2.0" <<std::endl;
        grid[start.x][start.y] = new A_star_cell(start, start, target, 0, heuristic_function);
//std::cout << "DF 2.1" <<std::endl;
        open.push_back(start);
    }
//std::cout << "DF 3" <<std::endl;
    // No path exists if the open list is empty after initialization
    if (open.empty()) {
        std::cerr << "Error: No path found." << std::endl;
        return false;
    }
//std::cout << "DF 4" <<std::endl;
    // Get the node with the lowest weight (f = g + h) from the open list
    Cord current = Get_lower_open();
//std::cout << "DF 5" <<std::endl;
    // Check if the target is reached
//    std::cout << current.x << " " << current.y  << "target " << target.x << " " << target.y<< std::endl;

    if (current.x == target.x && current.y == target.y) {
        //std::cout << "Path found!" << std::endl;
        path_found = true;
        // Optional: Reconstruct path here by backtracking using parent nodes
        return true;
    }
//std::cout << "DF 6" <<std::endl;
    // Remove current node from the open list
//std::cout << "Removed from open " << current.x << " " << current.y << std::endl;
    open.erase(std::remove(open.begin(), open.end(), current), open.end());
//std::cout << "DF 7" <<std::endl;
    // Add current node to the closed list
    closed.push_back(current);
//std::cout << "DF 8" <<std::endl;
    // Explore neighbors
    static const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    static const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
//std::cout << "DF 9" <<std::endl;
    for (int dir = 0; dir < 8; ++dir) {
        int nx = current.x + dx[dir];
        int ny = current.y + dy[dir];
//std::cout << "DF 10" <<std::endl;
        // Check boundaries
        if (nx < 0 || ny < 0 || nx >= grid_mask.size() || ny >= grid_mask[0].size()) {
            continue;
        }
//std::cout << "DF 11" <<std::endl;
        Cord neighbour = { static_cast<uint16_t>(nx), static_cast<uint16_t>(ny) };

        // Skip if the cell is not passable or already in the closed list
        if (!grid_mask[nx][ny] || std::find(closed.begin(), closed.end(), neighbour) != closed.end()) {
            continue;
        }
//std::cout << "DF 12" <<std::endl;
        double tentative_g = grid[current.x][current.y]->Get_accumulated() + Distance(current,neighbour); 
//std::cout << "DF 13" <<std::endl;
        // If the neighbor is not in the open list, create a new A_star_cell
        if (std::find(open.begin(), open.end(), neighbour) == open.end()) {
            grid[nx][ny] = new A_star_cell(current, neighbour, target, tentative_g, heuristic_function);
            open.push_back(neighbour);
        } else {
            // If the neighbor is already in the open list, check if the new path is better
//std::cout << "Tentative" << tentative_g << " Current: " << grid[nx][ny]->Get_accumulated() << std::endl;
            if (tentative_g <= grid[nx][ny]->Get_accumulated()) {
                grid[nx][ny]->Update(tentative_g,tentative_g + grid[nx][ny]->Get_heuristic(),current);
            }
        }
    }
    return false;
}

Vector<Vector<uint8_t>> Pathfinder::Get_textures() {
    // Initialize a 2D vector of uint8_t with the same size as the grid
    Vector<Vector<uint8_t>> textures(grid.size(), Vector<uint8_t>(grid[0].size(), 0));

    // Iterate through the grid to populate the textures
    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid[x].size(); ++y) {
            if (grid[x][y] != nullptr) {
                // Assign the texture value of the corresponding A_star_cell
                textures[x][y] = grid[x][y]->Get_texture();
            } else {
                // Assign a default value (e.g., 0 for uninitialized cells)
                textures[x][y] = 255;
            }
        }
    }

    return textures;
}

Vector<Cord> Pathfinder::Get_path()
{
    auto current = target;
    Vector<Cord>path;
    if (path_found)
    {
        while (current != start)
        {
            path.push_back(current);
            current = grid[current.x][current.y]->Get_parent();
        }
        path.push_back(current);
    }
    return path;
}

Pathfinder::~Pathfinder(){

}


uint8_t Pathfinder::A_star_cell::GetTextureIndex(int dx, int dy)
{
    if (dx == -1 && dy == -1)   return 0; // UP LEFT ARROW
    if (dx == 0  && dy == -1)   return 1; // UP ARROW
    if (dx == 1  && dy == -1)   return 2; // UP RIGHT ARROW
    if (dx == 1  && dy == 0)    return 3; // RIGHT ARROW
    if (dx == 1  && dy == 1)    return 4; // DOWN RIGHT ARROW
    if (dx == 0  && dy == 1)    return 5; // DOWN ARROW
    if (dx == -1 && dy == 1)    return 6; // DOWN LEFT ARROW
    if (dx == -1 && dy == 0)    return 7; // LEFT ARROW
                                return 0; // Default case;
}

Pathfinder::A_star_cell::A_star_cell(Cord parent, Cord position, Cord target, uint accumulated, double (*heuristic)(Cord target, Cord current))
{
    if (heuristic == nullptr)
    {
        std::cerr << "Error: heuristic is nullptr" << std::endl;
    }
    
//std::cout << "DF A1" <<std::endl;
    this->heuristic = heuristic(target, position);
//std::cout << "DF A1.1" <<std::endl;
    this->accumulated = accumulated;
    this->parent = parent;
    this->position = position;
//std::cout << "DF A2" <<std::endl;
    weight = accumulated + this->heuristic;
    
    ID = last_id;
    last_id++;

    // Calculate direction from current node to parent node
    int dx = parent.x - position.x;
    int dy = parent.y - position.y;
//std::cout << "DF A3" <<std::endl;
    // Map direction to texture index
    texture = GetTextureIndex(dx,dy);
//std::cout << "DF A4" <<std::endl;
}

void Pathfinder::A_star_cell::Set_parent(Cord new_parent){
    parent = new_parent;
}

void Pathfinder::A_star_cell::Update(uint new_accumulated, uint new_weigth, Cord new_parent){
    accumulated = new_accumulated;
    weight = new_weigth;
    parent = new_parent;
    int dx = parent.x - position.x;
    int dy = parent.y - position.y;
    texture = GetTextureIndex(dx,dy);
}

double Pathfinder::A_star_cell::Get_accumulated()
{
    return accumulated;
}

double Pathfinder::A_star_cell::Get_heuristic()
{
    return heuristic;
}

double Pathfinder::A_star_cell::Get_weight()
{
    return weight;
}

Cord Pathfinder::A_star_cell::Get_parent()
{
    return Cord(parent);
}

uint8_t Pathfinder::A_star_cell::Get_texture()
{
    return texture;
}

Pathfinder::A_star_cell::~A_star_cell()
{
}
double Distance(const Cord& point1, const Cord& point2) {
    // Use the distance formula: sqrt((x2 - x1)^2 + (y2 - y1)^2)
    return std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2));
}