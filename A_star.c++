#include "algoritmos/A_star.hpp"
#include "SDL_Visor/graphics/SDL_Visor.hpp"

const int WIDTH = 800;
const int HEIGHT = 800;

void eventos(SDL_Event& eventos);

int main(int argc, char const *argv[])
{
    vsr::Screen window("Algoritmo A*",WIDTH,HEIGHT,SDL_RENDERER_ACCELERATED);
    window.Init_TTF("courier","SDL_Visor/fonts/COURIERTXT.ttf",10);
    vsr::Color Blanco(255,255,255,255);


    while (window.Handle_events())
    {
        window.Clean_screen();
        window.Show_text(10,10,"Hola mundo",Blanco);
        window.Present_renderer();
        SDL_Delay(200);
    }
    


    return 0;
}

void eventos(SDL_Event& evento){

}


