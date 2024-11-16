#ifndef CUADRICULA_IMPORTADO
#define CUADRICULA_IMPORTADO
#include "Icono_pxl.hpp"
#include "Pantalla.hpp"


class Cuadricula
{
private:
    int alto;
    int ancho;
    Vector<Vector<bool>> mapa; 
public:
    Cuadricula(String file_path);
    void Mostrar(Pantalla *Ventana,Icono_pxl *Icono_bloqueado);
    void Mostrar(Pantalla *Ventana,Icono_pxl *Icono_bloqueado,int escala);
    void Mostrar(int x,int y,Pantalla *Ventana,Icono_pxl *Icono_bloqueado);
    void Mostrar(int x,int y,Pantalla *Ventana,Icono_pxl *Icono_bloqueado,int escala);
    ~Cuadricula();
};

Cuadricula::Cuadricula(String file_path){
    std::ifstream archivo(file_path);
    if (!archivo.is_open())
    {
        std::cerr << "Error abriendo el archivo: " << file_path << std::endl;
        return;
    }
    String linea;
    Vector<String>datos;
    uint8_t etapa;
    uint16_t numero_de_linea = 0;
    while (std::getline(archivo,linea))
    {
        switch (etapa)
        {
        case 0:
            datos = separar_por_espacios(linea);
            this->alto = std::stoi(datos.at(0));
            this->ancho = std::stoi(datos.at(1));
            this->mapa.resize(this->alto,Vector<bool>(this->ancho));
            etapa ++;
            break;
        case 1:
            if (linea != "")
            {
                    for (int columna = 0; columna < this->ancho; columna++)
                    {
                    this->mapa.at(columna).at(numero_de_linea) = (linea[columna] == '#') ;
                    }
                    numero_de_linea++;
            }
            break;
        default:
            break;
        }
    }
}
void Cuadricula::Mostrar(Pantalla *Ventana,Icono_pxl *Icono_bloqueado){
    Mostrar(0,0,Ventana,Icono_bloqueado);
}
void Cuadricula::Mostrar(int x,int y,Pantalla *Ventana,Icono_pxl *Icono_bloqueado){
    for (int i = 0; i < alto; i++)
    {
        for (int u = 0; u < ancho; u++)
        {
            if (mapa[i][u])
            {
                Ventana->dibujar_icono(Icono_bloqueado->pixel_art(),i*8,u*8);
            }
        }
        
    }
    
}

void Cuadricula::Mostrar(Pantalla *Ventana,Icono_pxl *Icono_bloqueado,int escala){
    Mostrar(0,0,Ventana,Icono_bloqueado,escala);
    
}
void Cuadricula::Mostrar(int x, int y,Pantalla *Ventana,Icono_pxl *Icono_bloqueado,int escala){
    for (int i = 0; i < alto; i++)
    {
        for (int u = 0; u < ancho; u++)
        {
            if (mapa[i][u])
            {
                Ventana->dibujar_icono(Icono_bloqueado->escalar_icono(escala),x + i*8*escala,y + u*8*escala);
            }
        }
        
    }
    
}


Cuadricula::~Cuadricula()
{
}









#endif