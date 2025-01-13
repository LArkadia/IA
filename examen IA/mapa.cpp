#include <cstdlib>
#include <iterator>
#include <ostream>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <functional>

namespace funciones {

// Función para cargar el mapa desde un archivo
std::vector<std::vector<char>> Cargar_mapa(const std::string& nombre_archivo) {
    std::fstream archivo(nombre_archivo, std::ios::in | std::ios::binary);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + nombre_archivo);
    }

    std::vector<std::vector<char>> mapa;
    std::string linea;
    while (std::getline(archivo, linea)) {
        mapa.emplace_back(linea.begin(), linea.end());
    }
    archivo.close();
    return mapa;
}

}

namespace clases {

// Estructura para representar una posición
struct pos {
    int x;
    int y;
};

// Clase agente
class agente {
private:
    uint id;  // ID del agente
    pos posicion;  // Posición del agente
    std::vector<std::vector<char>> *mapa;
    std::vector<std::vector<int>> *memoria;
    std::function<void(int*, int*, std::vector<std::vector<char>>*,std::vector<std::vector<int>>*)> comportamiento;  // Comportamiento

public:
    // Constructor del agente
    agente(int x, int y, std::function<void(int*, int*, std::vector<std::vector<char>>*,std::vector<std::vector<int>>*)> comportamiento,std::vector<std::vector<char>>* mapa);

    // Método para ejecutar el comportamiento
    pos ejecutar_comportamiento();
};

// Implementación del constructor
agente::agente(int x, int y, std::function<void(int*, int*, std::vector<std::vector<char>>*,std::vector<std::vector<int>> *)> comportamiento,std::vector<std::vector<char>>* mapa)
    :posicion({x, y}), comportamiento(comportamiento),mapa(mapa) {
        int alto = mapa->size();
        int ancho = mapa->at(0).size();
        auto mem = new std::vector<std::vector<int>>;
        mem->resize(alto);
        memoria = new std::vector<std::vector<int>>(alto, std::vector<int>(ancho, 0));        
}

// Método para ejecutar el comportamiento
pos agente::ejecutar_comportamiento() {
    comportamiento(&posicion.x, &posicion.y, mapa, memoria);
    return this->posicion;
}

}