#include "algoritmos/A_star.hpp"
#include "graficos/Pantalla.hpp"
#include "graficos/Cuadricula.hpp"
#include <cinttypes>
#include <SDL2/SDL_render.h>

const int WIDTH = 800;
const int HEIGHT = 800;

void eventos(SDL_Event& eventos);

int root_x,root_y;
int target_x,target_y;
uint8_t status = 0;
float escala = float(WIDTH/8)/7;

int main(int argc, char const *argv[])
{
    Pantalla Ventana;
    Ventana.crear_ventana("Algoritmo A*",WIDTH,HEIGHT);    
    Ventana.crear_renderizador(SDL_RENDERER_ACCELERATED);
    Ventana.Iniciar_ttf();
    Ventana.Cargar_fuente("courier","graficos/texto/COURIERTXT.ttf",14);
    Color Blanco(255,255,255);
    Color Negro(0,0,0);
    auto* icono_HAR = new Icono_pxl("Iconos/HAR.txt");
    auto* icono_ROOT = new Icono_pxl("Iconos/ROOT.txt");
    auto* icono_BLOCK = new Icono_pxl("Iconos/BLOCK.txt");

    Cuadricula Mapa("map.txt");

    Ventana.limpiar(Blanco);
    Mapa.Mostrar(&Ventana,icono_BLOCK,escala);
    char buffer[100];
    sprintf(buffer,"escala %.1f\0",escala);
    Ventana.Mostrar_texto(0,400,String(buffer),Negro);
    
    while (Ventana.manejar_eventos(eventos))
    {
        SDL_Delay(200);
        sprintf(buffer,"Status: %d\0",status);
        Ventana.Mostrar_texto(120,400,String(buffer),Negro);
        if (status == 2)
        {
            sprintf(buffer,"%d %d T: %d %d\0",root_x,root_y,target_x,target_y);
            Ventana.Mostrar_texto(10,420,String(buffer),Negro);
            status++;
        }
        Ventana.presentar_renderizador();

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


