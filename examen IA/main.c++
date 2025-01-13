#include "SDL_Visor/SDL_Visor.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cstdlib>
#include <ctime>
#include "mapa.cpp"
#include "comportamientos.cpp"

Vector<vsr::Color> colores = {
     vsr::Color(200, 200, 200, 250), // Gris claro
     vsr::Color(200, 100, 250, 250), // Lila
     vsr::Color(255, 0, 0, 250),     // Rojo
     vsr::Color(0, 255, 0, 250),     // Verde
     vsr::Color(0, 0, 255, 250),     // Azul
     vsr::Color(255, 255, 0, 250),   // Amarillo
     vsr::Color(255, 165, 0, 250),   // Naranja
     vsr::Color(128, 0, 128, 250),   // Morado
     vsr::Color(0, 255, 255, 250),   // Cian
     vsr::Color(255, 192, 203, 250), // Rosa
    vsr::Color(139, 69, 19, 250),   // Marrón
    vsr::Color(105, 105, 105, 250), // Gris oscuro
    vsr::Color(255, 20, 147, 250),  // Rosa fuerte
    vsr::Color(0, 128, 0, 250),     // Verde oscuro
    vsr::Color(70, 130, 180, 250),  // Azul acero
    vsr::Color(0, 0, 0, 250),       // Negro
};

struct entidad{
    clases::agente agente;
    int numero_color;
};

void Crear_textura_mapa(vsr::Screen *window, std::string nombre_textura, std::vector<std::vector<char>> mapa);

//-------------------------------------------------------MAIN------------------------------------------------------------
int dim = 700;
int cell_size = dim / 20;
int main() {
srand(time(0));
vsr::Screen window("Agentes inteligentes",dim,dim,SDL_RENDERER_ACCELERATED);
auto mapa = funciones::Cargar_mapa("map");
int x = rand()%20;
int y = rand()%10;
entidad A = {
    clases::agente(x,y,comportamientos::comportamiento_reina,&mapa),
    3
};
x = rand()%20;
y = rand()%20;
entidad B = {
    clases::agente(x,y,comportamientos::comportamiento_torre,&mapa),
    4
};

Crear_textura_mapa(&window, "bg", mapa);

clases::pos pos_A,pos_B;

while (window.Handle_events()) {
    window.Clean_screen();
    window.Draw_saved_texture("bg",nullptr);
    pos_A = A.agente.ejecutar_comportamiento();
    pos_B = B.agente.ejecutar_comportamiento();
    window.Draw_filled_rectangle(pos_A.x*cell_size, pos_A.y*cell_size+cell_size/2-5, 10, 10, colores[A.numero_color]);
    window.Draw_filled_rectangle(pos_B.x*cell_size+cell_size/2-5, pos_B.y*cell_size+cell_size/2-5, 10, 10, colores[B.numero_color]);
    window.Present_renderer();
    SDL_Delay(300);
}


return 0;
}
//---------------------------------------------------------FIN DEL MAIN----------------------------------------------------


void Crear_textura_mapa(vsr::Screen *window, std::string nombre_textura, std::vector<std::vector<char>> mapa){
    window->Create_texture(nombre_textura, 200, 200);
    
    int x = 0;
    int y = 0;

    for (auto line : mapa) {
        for (auto character : line) {
            if (character != 'X') {
                window->Draw_filled_rectangle(x*10, y*10, 10, 10, colores[0]);
            }else{
                window->Draw_filled_rectangle(x*10, y*10, 10, 10, colores[15]);
            }
            x++;
        }
        x = 0;
        y++;
    }
    window->End_texture();


}
