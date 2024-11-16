#include "algoritmos/A_star.hpp"
#include "graficos/Pantalla.hpp"
#include <cinttypes>
#include <SDL2/SDL_render.h>

const int WIDTH = 800;
const int HEIGHT = 800;

void eventos(SDL_Event& eventos);

int main(int argc, char const *argv[])
{
    Pantalla Ventana;
    Ventana.crear_ventana("Algoritmo A*",WIDTH,HEIGHT);    
    Ventana.crear_renderizador(SDL_RENDERER_ACCELERATED);
    Ventana.Iniciar_ttf();
    Ventana.Cargar_fuente("courier","graficos/texto/COURIERTXT.ttf",10);
    auto* icono_HAR = new Icono_pxl("Iconos/HAR.txt");
    auto* icono_ROOT = new Icono_pxl("Iconos/ROOT.txt");
    Ventana.dibujar_icono((icono_HAR)->pixel_art(),8,8);
    Ventana.dibujar_icono((icono_ROOT)->pixel_art(),0,0);
    while (Ventana.manejar_eventos(eventos))
    {
        SDL_Delay(200);
        Ventana.presentar_renderizador();

    }
    



    return 0;
}

void eventos(SDL_Event& evento){

}


