#ifndef ICONO_IMPORTADO
#define ICONO_IMPORTADO
// librerias no estandar
#include "Color.hpp"
// librerias estandar
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdint.h>

#define Vector std::vector
#define String std::string
#define Shared_ptr std::shared_ptr

std::vector<std::string> separar_por_espacios(std::string &string_entrada);

class Icono_pxl
{
private:
    Vector<Vector<Shared_ptr<Color>>> cuadricula;
    // Vector<Color*> colores;
    uint8_t resolucion;

public:
    Icono_pxl(uint8_t resolucion);
    Icono_pxl(String icono_txt);
    void ver_punteros();
    Vector<Vector<Shared_ptr<Color>>> pixel_art();
    ~Icono_pxl();
};

Icono_pxl::Icono_pxl(uint8_t resolucion) : resolucion(resolucion)
{
    cuadricula.resize(resolucion, Vector<Shared_ptr<Color>>(resolucion));
}
Icono_pxl::Icono_pxl(String icono_txt)
{
    std::ifstream archivo_icono(icono_txt);
    if (!archivo_icono.is_open())
    {
        std::cerr << "Error al abrir el archivo: " << icono_txt << std::endl;
        return;
    }
    String linea;
    uint8_t etapa = 0;
    uint8_t numero_linea = 0;
    std::map<char, Shared_ptr<Color>> simbolo_color;
    while (std::getline(archivo_icono, linea))
    {
        if (etapa == 0)
        {
            // guardar la resolucion
            this->resolucion = std::stoi(linea);
            this->cuadricula.resize(this->resolucion, Vector<Shared_ptr<Color>>(this->resolucion));
            etapa = 1;
        }
        if (etapa == 1 && linea != "\n" && linea != "")
        {
            // cargar los colores que representa cada caracter
            Vector<String> datos_color = separar_por_espacios(linea);
            if (datos_color.size() >= 5)
            {
                simbolo_color[datos_color[0][0]] = std::make_shared<Color>(std::stoi(datos_color[1]), std::stoi(datos_color[2]), std::stoi(datos_color[3]), std::stoi(datos_color[4]));
            }
            else if (datos_color.size() == 4)
            {
                simbolo_color[datos_color[0][0]] = std::make_shared<Color>(std::stoi(datos_color[1]), std::stoi(datos_color[2]), std::stoi(datos_color[3]));
            }
            else if (datos_color.size() == 1)
            {
                simbolo_color[datos_color[0][0]] = nullptr;
            }
            else
            {
                std::cerr << "Color mal asignado!! " << icono_txt << std::endl;
                simbolo_color[datos_color[0][0]] = nullptr;
            }
        }
        else if (linea == "\n" || linea == "")
        {
            etapa++;
        }
        if (etapa == 2 && linea != "")
        {
            // cargar los colores a la cuadricula
            if (numero_linea < this->resolucion)
            {
                for (int columna = 0; columna < this->resolucion; columna++)
                {
                    this->cuadricula.at(numero_linea).at(columna) = simbolo_color[linea[columna]];
                }
            }
            numero_linea++;
        }
    }
    archivo_icono.close();
}

Vector<Vector<Shared_ptr<Color>>> Icono_pxl::pixel_art()
{
    return cuadricula;
}
void Icono_pxl::ver_punteros()
{
    for (int i = 0; i < this->resolucion; i++)
    {
        for (int o = 0; o < this->resolucion; o++)
        {
            std::cout << this->cuadricula.at(i).at(o) << "|";
        }
        std::cout << "\n";
    }
}
Icono_pxl::~Icono_pxl() {}

std::vector<std::string> separar_por_espacios(std::string &string_entrada)
{
    std::vector<std::string> salida;
    std::istringstream see(string_entrada);
    std::string palabra;
    while (see >> palabra)
    {
        salida.push_back(palabra);
    }
    return salida;
}

#endif