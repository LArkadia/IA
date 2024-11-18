#include "algoritmos/A_star.hpp"
#include "SDL_Visor/graphics/SDL_Visor.hpp"

const int WIDTH = 800;
const int HEIGHT = 800;

void eventos(SDL_Event& eventos);

int root_x,root_y;
int target_x,target_y;
uint8_t status = 0;
float escala = float(WIDTH/8)/7;

int main(int argc, char const *argv[])
{
    vsr::Screen window("Algoritmo A*",WIDTH,HEIGHT,SDL_RENDERER_ACCELERATED);
    window.Init_TTF("courier","SDL_Visor/fonts/COURIERTXT.ttf",10);
    vsr::Color Blanco(255,255,255,255);


    while (window.Handle_events())
    {
        window.Clean_screen();
        window.Draw_rectangle(10,10,10,10,Blanco);
        window.Show_text(10,10,"Si Puto",Blanco);
        window.Present_renderer();
        SDL_Delay(200);
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
        switch (status)
        {
        case 0:
            root_x = evento.button.x/(escala*8);
            root_y = evento.button.y/(escala*8);
            status++;
            break;
        case 1:
            target_x = evento.button.x/(escala*8);
            target_y = evento.button.y/(escala*8);
            status++;
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


