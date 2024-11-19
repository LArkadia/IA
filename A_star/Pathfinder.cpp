#include "Pathfinder.hpp"
Pathfinder::Pathfinder(String map_file_path) {
    start_counter = false;

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
        this->target = target;
        if (start_counter < 2){
            start_counter++;
        }
    }

    void Pathfinder::Step(){
        if (start_counter < 2){
            std::cerr << "Error: Can't start, missing start or target" << std::endl;
            return;
        }
        
    }

Pathfinder::~Pathfinder(){

}

Pathfinder::A_star_cell::A_star_cell(Cord position, Cord start, Cord target){
    
}
    
Pathfinder::A_star_cell::~A_star_cell(){

}
