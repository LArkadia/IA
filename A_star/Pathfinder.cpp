#include <cinttypes>
#include <vector>
#include <string>

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
    
   
    



    Vector<Vector<bool>> grid;

    Cord start,target;
public:
    Pathfinder(String map_file_path);
    void Set_start(uint16_t x,uint16_t y);
    void Set_target(uint16_t x,uint16_t y);
    void Step();
    ~Pathfinder();
};

Pathfinder::Pathfinder(String map_file_path){

}

Pathfinder::~Pathfinder(){

}

Pathfinder::A_star_cell::A_star_cell(Cord position, Cord start, Cord target){
    
}
    
Pathfinder::A_star_cell::~A_star_cell(){

}
