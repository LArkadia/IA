#include <cinttypes>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define Vector std::vector
#define String std::string

struct Cord
{
    uint16_t x;
    uint16_t y;
};
class Pathfinder
{
private:
    class A_star_cell
    {
    private:
        uint heuristic,accumulated,weight,ID;
        A_star_cell* parent;
    public:
        A_star_cell(Cord position, Cord start, Cord target);        
        ~A_star_cell();
    };
    Vector<Vector<bool>> grid_mask;
    Vector<Vector<A_star_cell*>> grid;
    Cord start,target;
    uint8_t start_counter;
public:
    Pathfinder(String map_file_path);
    Vector<Vector<bool>> Get_grid_mask();
    void Set_start(Cord start);
    void Set_target(Cord target);
    void Step();
    ~Pathfinder();
};
