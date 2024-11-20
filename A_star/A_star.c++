#include "Pathfinder.hpp"
#include "../SDL_Visor/SDL_Visor.hpp"

const int WIDTH = 800;
const int HEIGHT = 800;

void eventos(SDL_Event& eventos);
std::map<String,vsr::Color*> Load_colors();
Vector<vsr::Icon*> Load_arrow_Icons(vsr::Screen* window);
void create_map_texture(String texture_name,uint16_t cell_widht,Vector<Vector<bool>> map_grid,vsr::Screen* window,vsr::Icon blocked_icon,vsr::Color &grid_color);
double heuristic(Cord target, Cord current);
Cord start,target;
uint8_t status = 0;
float escala = float(WIDTH/8);
Vector<Vector<bool>> mask;
int main(int argc, char const *argv[])
{
    //Create window
    vsr::Screen window("Algoritmo A*",WIDTH,HEIGHT,SDL_RENDERER_ACCELERATED);
    window.Set_events_handler(eventos);
    window.Init_TTF("sans","../SDL_Visor/fonts/NotoSans/NotoSans-Thin.ttf",10);
    window.Init_IMG();
    //Load colors
    auto colors = Load_colors();
    //Load Icons
    auto arrow_icons = Load_arrow_Icons(&window);
    vsr::Icon block_icon("../Iconos/BLOCK.png",window.Get_renderer());
    vsr::Icon start_icon("../Iconos/START.png",window.Get_renderer());
    vsr::Icon target_icon("../Iconos/TARGET.png",window.Get_renderer());
    //Load map_mask
    auto grid = Pathfinder("map.txt",heuristic);
    mask = grid.Get_grid_mask();
    escala = escala / mask.size();
    //Create background texture
    uint16_t cell_size = 32;
    create_map_texture("background", cell_size, grid.Get_grid_mask(), &window, block_icon,*colors["Black"]);
    Vector<Vector<uint8_t>> textures;
    bool path_found = false;
    int x,y;
    uint16_t cell_width = WIDTH/mask.size();
    uint16_t cell_height = HEIGHT/ mask[0].size();
    SDL_Rect area;
    while (window.Handle_events())
    {   
        window.Clean_screen(*colors["White"]);
        window.Draw_saved_texture("background");
        switch (status)
        {
        case 2:
                grid.Set_start(start);
                grid.Set_target(target); 
                status++;
            break;
        case 3:
                
                if (!path_found)
                {   
                    path_found = grid.Step();
                    if (path_found)
                    {
                        status++;
                    }
                    
                }
                area = {start.x*cell_width,start.y*cell_height,cell_width,cell_height};
                window.Draw_texture(start_icon.Get_texture(),&area);
                area = {target.x*cell_width,target.y*cell_height,cell_width,cell_height};
                window.Draw_texture(target_icon.Get_texture(),&area);
                textures = grid.Get_textures();
                x = 0;
                for (auto &&column : textures)
                {
                    y = 0;
                    for (auto &&cell : column)
                    {
                        if (cell != 255)
                        {
                            area = {x*cell_width,y*cell_height,cell_width,cell_height};
                            window.Draw_texture(arrow_icons[cell]->Get_texture(),&area);
                        }
                        
                        y++;
                    }
                    x++;
                }
                
            break;
        default:

                Vector<Cord> path = grid.Get_path();

                for (auto &&path_cell : path){

                    area = {path_cell.x *cell_width,path_cell.y*cell_height,cell_width,cell_height};
                    window.Draw_filled_rectangle(path_cell.x *cell_width,path_cell.y*cell_height,cell_width,cell_height,*colors["Green"]);   
                }
                area = {start.x*cell_width,start.y*cell_height,cell_width,cell_height};
                window.Draw_texture(start_icon.Get_texture(),&area);
                area = {target.x*cell_width,target.y*cell_height,cell_width,cell_height};
                window.Draw_texture(target_icon.Get_texture(),&area);
                
                x = 0;
                for (auto &&column : textures)
                {
                    y = 0;
                    for (auto &&cell : column)
                    {
                        if (cell != 255)
                        {
                            SDL_Rect area = {x*cell_width,y*cell_height,cell_width,cell_height};
                            window.Draw_texture(arrow_icons[cell]->Get_texture(),&area);
                        }
                        
                        y++;
                    }
                    x++;
                }
            break;
        }
        
        
        window.Present_renderer();
        
        SDL_Delay(300);
    }
    


    return 0;
}

void eventos(SDL_Event& evento){
switch (evento.type)
{
case SDL_MOUSEBUTTONDOWN:
    switch (evento.button.button)
    {
    case SDL_BUTTON_LEFT:
        std::cout << floor(evento.button.x/(escala*8)) << " " << floor(evento.button.y/(escala*8)) << std::endl;
        switch (status)
        {
        case 0:
            start.x = evento.button.x/(escala*8);
            start.y = evento.button.y/(escala*8);
            if (mask[start.x][start.y])
            {
                status++;
            }
            
            
            break;
        case 1:
            target.x = evento.button.x/(escala*8);
            target.y = evento.button.y/(escala*8);
            if (mask[target.x][target.y])
            {
                status++;
            }
        break;
        default:
            break;
        }
        break;
    
    default:
        break;
    }
    break;

default:
    break;
}
}

std::map<String,vsr::Color*> Load_colors(){
    std::map<String, vsr::Color*> colors = {
        {"White", new vsr::Color{255, 255, 255, 255}},
        {"Black", new vsr::Color(0, 0, 0, 255)},
        {"Green", new vsr::Color(124, 252, 0, 255)},
        {"Orange",new vsr::Color(255, 128, 0, 255)},
        {"Red", new vsr::Color(237, 64, 64, 255)}
    };
    return colors;
}

Vector<vsr::Icon *> Load_arrow_Icons(vsr::Screen* window)
{
    Vector<vsr::Icon *> icons;
    icons.push_back(new vsr::Icon("../Iconos/ULA.png",window->Get_renderer()));
    icons.push_back(new vsr::Icon("../Iconos/UA.png",window->Get_renderer()));
    icons.push_back(new vsr::Icon("../Iconos/URA.png",window->Get_renderer()));
    icons.push_back(new vsr::Icon("../Iconos/RA.png",window->Get_renderer()));
    icons.push_back(new vsr::Icon("../Iconos/DRA.png",window->Get_renderer()));
    icons.push_back(new vsr::Icon("../Iconos/DA.png",window->Get_renderer()));
    icons.push_back(new vsr::Icon("../Iconos/DLA.png",window->Get_renderer()));
    icons.push_back(new vsr::Icon("../Iconos/LA.png",window->Get_renderer()));
    return icons;
}

void create_map_texture(String texture_name, uint16_t cell_size, Vector<Vector<bool>> map_grid, vsr::Screen *window, vsr::Icon blocked_icon, vsr::Color &grid_color){
    uint16_t width = cell_size * map_grid.size();
    uint16_t height = cell_size * map_grid[0].size();
    window->Create_texture(texture_name, width+1, height+1);
    window->Clean_screen();
    // Dibujar líneas verticales
    for (uint16_t i = 0; i <= map_grid.size(); i++){
        window->Draw_line_pos(i * cell_size, 0, i * cell_size, height, grid_color);
    }
    // Dibujar líneas horizontales
    for (uint16_t i = 0; i <= map_grid[0].size(); i++){
        window->Draw_line_pos(0, i * cell_size, width, i * cell_size, grid_color);
    }
    for (uint16_t u = 0; u < map_grid[0].size(); u++)
    {
        for (uint16_t i = 0; i < map_grid.size(); i++)
        {
            if(!map_grid[u][i]){
                SDL_Rect block_area = {u*cell_size,i*cell_size,cell_size,cell_size};
                window->Draw_texture(blocked_icon.Get_texture(),&block_area);
            }
        }
        
    }
    
    window->End_texture();
}

double heuristic(Cord target, Cord current){
    return std::max(std::abs(target.x - current.x), std::abs(target.y - current.y));
}