#include <cinttypes>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

#define Vector std::vector
#define String std::string

struct Cord
{
    uint16_t x;
    uint16_t y;
    bool operator==(const Cord& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Cord& other) const {
        return x != other.x || y != other.y;
    }
};

class Pathfinder
{
private:
    class A_star_cell
    {
    private:
        static uint16_t last_id;
        uint ID;
        double heuristic,accumulated,weight;
        Cord parent,position;
        uint8_t texture;
        uint8_t GetTextureIndex(int dx, int dy);
    public:
        A_star_cell(Cord parent,Cord position, Cord target,uint acummulated,double (*heuristic)(Cord target,Cord current));     
        void Set_parent(Cord new_parent);   
        void Update(uint new_accumulated,uint new_weigth,Cord new_parent);
        double Get_accumulated();
        double Get_heuristic();
        double Get_weight();
        Cord Get_parent();
        uint8_t Get_texture();
        ~A_star_cell();
    };
    Vector<Vector<bool>> grid_mask;
    Vector<Vector<A_star_cell*>> grid;
    Vector<Cord> open;
    Vector<Cord> closed;
    bool path_found;
    double (*heuristic_function)(Cord target,Cord current); 
    Cord start,target;
    uint8_t start_counter;
    //Functions
    Cord Get_lower_open();
    void Check_neightbours(Cord current);
public:
    Pathfinder(String map_file_path);
    Pathfinder(String map_file_path,double (*heuristic)(Cord target,Cord current));
    Vector<Vector<bool>> Get_grid_mask();
    void Set_start(Cord start);
    void Set_target(Cord target);
    bool Step();
    Vector<Vector<uint8_t>> Get_textures();
    Vector<Cord> Get_path();
    ~Pathfinder();
};
double Distance(const Cord& point1, const Cord& point2);
