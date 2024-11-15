#ifndef VENTANA_IMPORTADO
#define VENTANA_IMPORTADO
//Librerias no estandar
#include "Color.hpp"
#include "Icono_pxl.hpp"
//Librerias estandar
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <math.h>


class Pantalla 
{
private:
    SDL_Window* ventana;
    SDL_Renderer* renderizador;
    SDL_Surface* superficie;
    SDL_Texture* textura;
    bool cerrar;
    bool ttf_opened;
    std::string fuente_predeterminada;
    std::map<std::string,TTF_Font*> fuente;
    std::map<std::string,SDL_Texture*> texturas;

public:
    Pantalla();
    void Iniciar_ttf();
    void crear_ventana(std::string titulo_ventana, uint16_t ancho,uint16_t alto);
    void crear_renderizador(uint32_t banderas);
    void establecer_icono(std::string icono_png);
    void presentar_renderizador();
    bool manejar_eventos();
    bool manejar_eventos(void (*funcion_eventos)(SDL_Event&));
    void crear_textura(std::string nombre,int alto, int ancho);
    void finalizar_textura();
    void dibujar_textura(std::string nombre);
    void limpiar();
    void limpiar(Color &color);
    void dibujar_rectangulo(const uint16_t x,const uint16_t y,const uint16_t ancho,const uint16_t alto,Color &color);
    void dibujar_linea_ang(const uint16_t x,const uint16_t y,const _Float64 angulo ,const uint16_t largo,Color &color);
    void dibujar_linea_pos(const uint16_t x1,const uint16_t y1,const uint16_t x2,const uint16_t y2,Color &color);
    void dibujar_vector(std::vector<int>Y,Color &color);
    void dibujar_vector(std::vector<int>Y,Color &color,int escala);
    void dibujar_mapa(std::map<int,int>Y,Color &color);
    void dibujar_icono(std::vector<std::vector<std::shared_ptr<Color>>> Icono,uint x, uint y);
    void dibujar_circunferencia(const uint16_t x, const uint16_t y, const uint16_t r, Color &color);
    //Funciones ttf
    void Cargar_fuente(std::string nombre,std::string archivo,int tamanno);
    void Cambiar_fuente_predeterminada(std::string nombre);
    void Mostrar_texto(int x, int y,std::string texto, Color &color);
    //Obtener alto y ancho
    void dimensiones(int* ancho,int* alto);
    ~Pantalla();
};
Pantalla::Pantalla(){
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    if (SDL_Init(SDL_INIT_VIDEO) != 0){std::cerr << "Error al iniciar el video" << std::endl;}
    cerrar = false;
}
void Pantalla::Iniciar_ttf(){
    if (TTF_Init() < 0) {
        printf("SDL_ttf no pudo inicializarse! SDL_ttf Error: %s\n", TTF_GetError());
        this->~Pantalla();
        return;
    }
    fuente_predeterminada = "";
    ttf_opened = true;
}
void Pantalla::establecer_icono(std::string icono_png){
    this->superficie = IMG_Load(icono_png.c_str());
    if (!superficie)
    {
        std::cerr << "Error al cargar el icono: " << IMG_GetError() << std::endl;
        this->~Pantalla();
        return;
    }
    SDL_SetWindowIcon(this->ventana,this->superficie);
}
void Pantalla::crear_renderizador(uint32_t banderas){
    this->renderizador = SDL_CreateRenderer(this->ventana,-1,banderas);
    if (!this->renderizador)
    {
        std::cerr << "Error al crear el renderizador: " << SDL_GetError() << std::endl;
        this->~Pantalla();
        return;
    }
    
}
bool Pantalla::manejar_eventos(void (*funcion_eventos)(SDL_Event& evento)){
    SDL_Event evento_actual;
    while (SDL_PollEvent(&evento_actual))
    {
        if (evento_actual.type == SDL_QUIT)
        {   
            this->cerrar = true;
        }
        funcion_eventos(evento_actual);
    }
    return !cerrar;
}
bool Pantalla::manejar_eventos(){
    SDL_Event evento_actual;
    while (SDL_PollEvent(&evento_actual))
    {
        if (evento_actual.type == SDL_QUIT)
        {
            this->cerrar = true;
        }
    }
    return !cerrar;
}
void Pantalla::crear_textura(std::string nombre,int alto, int ancho){
    this->texturas[nombre] = SDL_CreateTexture(this->renderizador,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,ancho,alto);
    if (this->texturas[nombre] == nullptr)
    {
        std::cerr << "Error al crear la textura, SDL: " << SDL_GetError() << std::endl;
    }
    SDL_SetRenderTarget(renderizador,this->texturas[nombre]);
}
void Pantalla::finalizar_textura(){
    SDL_SetRenderTarget(this->renderizador,nullptr);
}
void Pantalla::dibujar_textura(std::string nombre){
    SDL_RenderCopy(this->renderizador,this->texturas[nombre],nullptr,nullptr);
}
void Pantalla::crear_ventana(std::string titulo_ventana, uint16_t ancho,uint16_t alto){
    ventana = SDL_CreateWindow(titulo_ventana.c_str(),SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,ancho,alto,SDL_WINDOW_SHOWN);
    if (!ventana){
        std::cerr << "Error al crear la ventana, SDL: " << SDL_GetError() << std::endl;
        this->~Pantalla();
        return;
    }
}

void Pantalla::presentar_renderizador(){
    SDL_RenderPresent(this->renderizador);
}
void Pantalla::dibujar_rectangulo(const uint16_t x,const uint16_t y,const uint16_t ancho,const uint16_t alto,Color &color){
    SDL_Rect rectangulo{x:x,y:y,w:ancho,h:alto};
    SDL_SetRenderDrawColor(this->renderizador,color.R(),color.G(),color.B(),color.A());
    SDL_RenderFillRect(this->renderizador,&rectangulo);
}
void Pantalla::dibujar_linea_ang(const uint16_t x,const uint16_t y,const _Float64 angulo ,const uint16_t largo,Color &color){
    const uint16_t y2 = y + (largo * sin(angulo));
    const uint16_t x2 = x + (largo * cos(angulo));
    SDL_SetRenderDrawColor(this->renderizador,color.R(),color.G(),color.B(),color.A());
    SDL_RenderDrawLine(this->renderizador,x,y,x2,y2);
}

void Pantalla::dibujar_linea_pos(const uint16_t x1,const uint16_t y1,const uint16_t x2,const uint16_t y2,Color &color){
    SDL_SetRenderDrawColor(this->renderizador,color.R(),color.G(),color.B(),color.A());
    SDL_RenderDrawLine(this->renderizador,x1,y1,x2,y2);
}
void Pantalla::dibujar_vector(std::vector<int>Y,Color &color){
        SDL_SetRenderDrawColor(this->renderizador,color.R(),color.G(),color.B(),color.A());
        SDL_Point puntos[Y.size()];
        for (int i = 0; i < Y.size(); i++)
        {
            puntos[i].x = i;
            puntos[i].y = Y[i];
        }
        SDL_RenderDrawLines(this->renderizador,puntos,Y.size());

}
void Pantalla::dibujar_vector(std::vector<int>Y,Color &color,int escala){
        SDL_SetRenderDrawColor(this->renderizador,color.R(),color.G(),color.B(),color.A());
        SDL_Point puntos[Y.size()];
        for (int i = 0; i < Y.size(); i++)
        {
            puntos[i].x = i*escala;
            puntos[i].y = Y[i];
        }
        SDL_RenderDrawLines(this->renderizador,puntos,Y.size());

}
void Pantalla::dibujar_mapa(std::map<int,int>Y,Color &color){


}
void Pantalla::limpiar(){
    SDL_SetRenderDrawBlendMode(this->renderizador, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(this->renderizador,0,0,0,255);
    SDL_RenderClear(this->renderizador);
}
void Pantalla::limpiar(Color &color){
    SDL_SetRenderDrawColor(this->renderizador,color.R(),color.G(),color.B(),color.A());
    SDL_RenderClear(this->renderizador);
}
void Pantalla::dibujar_icono(std::vector<std::vector<std::shared_ptr<Color>>> Icono,uint x, uint y){
    uint numero_linea = 0;
    uint numero_columna = 0;

    for (auto linea: Icono)
    {
        for (auto color: linea)
        {
            if(color != nullptr){
            SDL_SetRenderDrawColor(this->renderizador,color.get()->R(),color.get()->G(),color.get()->B(),color.get()->A());
            SDL_RenderDrawPoint(this->renderizador,x+numero_columna,y+numero_linea);
            }
            numero_columna++;
        }
        numero_linea++;
        numero_columna = 0;
    }
    
}
void Pantalla::dibujar_circunferencia(const uint16_t x, const uint16_t y, const uint16_t r, Color &color) {
    if(r == 0){return;}
    SDL_SetRenderDrawColor(this->renderizador, color.R(), color.G(), color.B(), color.A());
    for (float i = 0; i <= 45; i+= 0.05) {
        int h = static_cast<int>(round(r * sin(i * M_PI / 180.0)));
        int w = static_cast<int>(round(r * cos(i * M_PI / 180.0)));

        SDL_RenderDrawPoint(this->renderizador, x + w, y + h);
        SDL_RenderDrawPoint(this->renderizador, x - w, y + h);
        SDL_RenderDrawPoint(this->renderizador, x + w, y - h);
        SDL_RenderDrawPoint(this->renderizador, x - w, y - h);
        SDL_RenderDrawPoint(this->renderizador, x + h, y + w);
        SDL_RenderDrawPoint(this->renderizador, x - h, y + w);
        SDL_RenderDrawPoint(this->renderizador, x + h, y - w);
        SDL_RenderDrawPoint(this->renderizador, x - h, y - w);
    }
}

//Funciones de ttf
void Pantalla::Cargar_fuente(std::string nombre,std::string archivo,int tamanno){
    this->fuente[nombre] = TTF_OpenFont(archivo.c_str(),tamanno);
    if (this->fuente[nombre] == NULL)
    {
        std::cerr << "Error al cargar la fuente " << nombre << " | Error: " << TTF_GetError() << std::endl;
        this->~Pantalla();
        return;
    }
    if (this->fuente_predeterminada == "")
    {
        this->fuente_predeterminada = nombre;
    }
    
}
void Pantalla::Cambiar_fuente_predeterminada(std::string nombre){
    if (this->fuente[nombre] == nullptr){
        std::cerr << "Fuente predeterminada no cargada!!" << std::endl;
        this->~Pantalla();
        return;
    } else{
        this->fuente_predeterminada = nombre;
    }
}
void Pantalla::Mostrar_texto(int x,int y,std::string texto, Color &color){
    this->superficie = TTF_RenderText_Solid(this->fuente[this->fuente_predeterminada],texto.c_str(),SDL_Color{color.R(),color.G(),color.B(),color.A()});
    if (superficie == nullptr)
    {
    std::cerr << "Error al crear la superficie de texto!, Error: " << TTF_GetError() << std::endl ;
    this->~Pantalla();
    return;
    }
    this->textura = SDL_CreateTextureFromSurface(this->renderizador,this->superficie);
    if (this->textura == nullptr){
        std::cerr << "Error al crear la textura del texto, Error: " << SDL_GetError() << std::endl;
        this->~Pantalla();
        return;
    }
    int ancho_texto, alto_texto;
    SDL_QueryTexture(this->textura,NULL,NULL,&ancho_texto,&alto_texto);
    SDL_Rect  Area_texto = SDL_Rect{x,y,ancho_texto,alto_texto};
    SDL_RenderCopy(this->renderizador,this->textura,NULL,&Area_texto);
}


Pantalla::~Pantalla(){
    if (this->renderizador != nullptr){SDL_DestroyRenderer(this->renderizador);}
    if (this->superficie != nullptr){SDL_FreeSurface(this->superficie);}
    if (this->ventana != nullptr){SDL_DestroyWindow(this->ventana);}
    if (this->textura != nullptr){SDL_DestroyTexture(this->textura);}

    SDL_Quit();
    if (ttf_opened == true){TTF_Quit();}
    
}

void Pantalla::dimensiones(int* ancho,int* alto){
    return SDL_GetWindowSize(this->ventana,ancho,alto);
}










#endif