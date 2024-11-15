#include "graficos/Color.hpp"
#include "graficos/Pantalla.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <map>
#include <string>


void eventos(SDL_Event& evento_actual);
int main(){

    Pantalla Ventana;
    std::map<std::string,Color*> colores;
    Ventana.crear_ventana("test window", 600, 500);
    Ventana.crear_renderizador(SDL_RENDERER_ACCELERATED);
    Ventana.Iniciar_ttf();
    Ventana.Cargar_fuente("courier","graficos/texto/COURIERTXT.ttf", 12);
    
    colores["Verde"] = new Color(0,155,0);
    colores["Azul"] = new Color(30,80,155);
    colores["Rojo"] = new Color(155,0,0);
    colores["Rosa"] = new Color(155,100,100);


    while (Ventana.manejar_eventos(eventos)) {
    


        Ventana.presentar_renderizador();
        Ventana.Mostrar_texto(10, 10,"Hola mundo", *colores["Azul"]);
        Ventana.Mostrar_texto(10,26, "Esta es una prueba",*colores["Verde"]);


    }




}
void eventos(SDL_Event& evento_actual){
    switch (evento_actual.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        switch (evento_actual.button.button)
        {
        case SDL_BUTTON_LEFT:

        break;
        
        default:
            break;
        }
        break;
    case SDL_KEYDOWN:
        switch (evento_actual.key.type) {
        
        }
    default:
        break;
    }
}
