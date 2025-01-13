#include <vector>
#include <algorithm>
#include <cmath>

namespace comportamientos {
// Variable global para el objetivo
int objetivo_x = 19, objetivo_y = 19;

struct celda{
    int I;
    int R;
    int D;
};

// Función para calcular la distancia Manhattan
int calcular_distancia(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}
// Funcion auxiliar para revisar que la celda este libre
bool esta_libre(int x, int y, const std::vector<std::vector<char>>& mapa) {
    return (x >= 0 && y >= 0 && x < mapa.size() && y < mapa[0].size() && mapa[y][x] != 'X');
}
// Funcion auxiliar revision de visitadas
int obtener_visitas(int x, int y, const std::vector<std::vector<int>>& memoria) {
    if (y >= 0 && y < memoria.size() && x >= 0 && x < memoria[0].size()) {
        return memoria[y][x];
    }
    return 0;
}
// Funcion auxiliar comparacion
bool comparar_celdas(const celda& a, const celda& b) {
    if (a.R != b.R) {
        return a.R < b.R; // Primero R=menor número de visitas
    }
    return a.D < b.D; // Dentro del mismo R, ordenar por distancia ascendente
}

// Comportamiento del alfil: movimiento diagonal
void comportamiento_alfil(int* x, int* y, std::vector<std::vector<char>>* mapa, std::vector<std::vector<int>>* memoria) {
    // Coordenadas actuales de la reina
    int mejor_x = *x;
    int mejor_y = *y;
    if (*x == objetivo_x && *y == objetivo_y) {
    return;
    }
    
    // Dirección de movimiento: 8 posibles (horizontal, vertical, diagonal)
    // Índices:
    // 0: Derecha (→)
    // 1: Izquierda (←)
    // 2: Abajo (↓)
    // 3: Arriba (↑)
    // 4: Diagonal Inferior Derecha (↘)
    // 5: Diagonal Inferior Izquierda (↙)
    // 6: Diagonal Superior Izquierda (↖)
    // 7: Diagonal Superior Derecha (↗)
    int dx[] = {1, -1,1, -1, -1, 1};
    int dy[] = {0, 0,1, 1, -1, -1};
    
    std::vector<celda> celdas;

    // Evaluar todas las posibles direcciones de movimiento
    for (int i = 0; i < 6; ++i) {
        int nx = *x + dx[i];
        int ny = *y + dy[i];
        
        // Verificar que la nueva posición esté dentro de los límites del mapa y que no esté bloqueada ('X')
        if (!esta_libre(nx, ny, *mapa)) {
            continue; // Posición bloqueada o fuera de límites
        }
        
        // Si el movimiento es diagonal, verificar las celdas adyacentes
        bool es_diagonal = (dx[i] != 0) && (dy[i] != 0);
        if (es_diagonal) {
            // Determinar las posiciones adyacentes horizontales y verticales
            int adj_x1 = *x + dx[i];
            int adj_y1 = *y; // Movimiento horizontal
            int adj_x2 = *x;
            int adj_y2 = *y + dy[i]; // Movimiento vertical
            
            bool adj1_libre = esta_libre(adj_x1, adj_y1, *mapa);
            bool adj2_libre = esta_libre(adj_x2, adj_y2, *mapa);
            
            // Si ambas celdas adyacentes están bloqueadas, no se puede mover diagonalmente
            if (!adj1_libre && !adj2_libre) {
                continue;
            }
        }
        
        // Calcular la distancia desde la nueva posición al objetivo
        int distancia = calcular_distancia(nx, ny, objetivo_x, objetivo_y);
        
        // Obtener el número de veces que la casilla ya ha sido visitada
        int visitas = obtener_visitas(nx, ny, *memoria);
        
        // Agregar la celda al vector
        celdas.push_back({i, visitas, distancia});
    }
    
    // Ordenar las celdas primero por R (menor número de visitas), luego por D (distancia ascendente)
    std::sort(celdas.begin(), celdas.end(), comparar_celdas);
    
    // Si hay al menos una celda disponible, mover a la posición más óptima
    if (!celdas.empty()) {
        int mejor_movimiento = celdas[0].I; // Índice del mejor movimiento
        mejor_x = *x + dx[mejor_movimiento];
        mejor_y = *y + dy[mejor_movimiento];
        
        // Actualizar las coordenadas de la reina
        *x = mejor_x;
        *y = mejor_y;
        
        // Incrementar el contador de visitas en la nueva posición
        if (mejor_y >= 0 && mejor_y < memoria->size() && mejor_x >= 0 && mejor_x < (*memoria)[0].size()) {
            (*memoria)[mejor_y][mejor_x] += 1; // Incrementa el contador de visitas
        }
    }
    else {
        // No hay movimientos disponibles que no hayan sido visitados
    }
}

// Comportamiento de la torre: movimiento en línea recta
void comportamiento_torre(int* x, int* y, std::vector<std::vector<char>>* mapa, std::vector<std::vector<int>>* memoria) {
    // Coordenadas actuales de la reina
    int mejor_x = *x;
    int mejor_y = *y;
    if (*x == objetivo_x && *y == objetivo_y) {
    return;
    }
    
    // Dirección de movimiento: 8 posibles (horizontal, vertical, diagonal)
    // Índices:
    // 0: Derecha (→)
    // 1: Izquierda (←)
    // 2: Abajo (↓)
    // 3: Arriba (↑)
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    
    std::vector<celda> celdas;

    // Evaluar todas las posibles direcciones de movimiento
    for (int i = 0; i < 4; ++i) {
        int nx = *x + dx[i];
        int ny = *y + dy[i];
        
        // Verificar que la nueva posición esté dentro de los límites del mapa y que no esté bloqueada ('X')
        if (!esta_libre(nx, ny, *mapa)) {
            continue; // Posición bloqueada o fuera de límites
        }

        
        // Calcular la distancia desde la nueva posición al objetivo
        int distancia = calcular_distancia(nx, ny, objetivo_x, objetivo_y);
        
        // Obtener el número de veces que la casilla ya ha sido visitada
        int visitas = obtener_visitas(nx, ny, *memoria);
        
        // Agregar la celda al vector
        celdas.push_back({i, visitas, distancia});
    }
    
    // Ordenar las celdas primero por R (menor número de visitas), luego por D (distancia ascendente)
    std::sort(celdas.begin(), celdas.end(), comparar_celdas);
    
    // Si hay al menos una celda disponible, mover a la posición más óptima
    if (!celdas.empty()) {
        int mejor_movimiento = celdas[0].I; // Índice del mejor movimiento
        mejor_x = *x + dx[mejor_movimiento];
        mejor_y = *y + dy[mejor_movimiento];
        
        // Actualizar las coordenadas de la reina
        *x = mejor_x;
        *y = mejor_y;
        
        // Incrementar el contador de visitas en la nueva posición
        if (mejor_y >= 0 && mejor_y < memoria->size() && mejor_x >= 0 && mejor_x < (*memoria)[0].size()) {
            (*memoria)[mejor_y][mejor_x] += 1; // Incrementa el contador de visitas
        }
    }
    else {
        // No hay movimientos disponibles que no hayan sido visitados
    }
}


// Comportamiento de la reina: movimiento en línea recta o diagonal

void comportamiento_reina(int* x, int* y, std::vector<std::vector<char>>* mapa, std::vector<std::vector<int>>* memoria) {
    // Coordenadas actuales de la reina
    int mejor_x = *x;
    int mejor_y = *y;
    if (*x == objetivo_x && *y == objetivo_y) {
    return;
    }
    
    // Dirección de movimiento: 8 posibles (horizontal, vertical, diagonal)
    // Índices:
    // 0: Derecha (→)
    // 1: Izquierda (←)
    // 2: Abajo (↓)
    // 3: Arriba (↑)
    // 4: Diagonal Inferior Derecha (↘)
    // 5: Diagonal Inferior Izquierda (↙)
    // 6: Diagonal Superior Izquierda (↖)
    // 7: Diagonal Superior Derecha (↗)
    int dx[] = {1, -1, 0, 0, 1, -1, -1, 1};
    int dy[] = {0, 0, 1, -1, 1, 1, -1, -1};
    
    std::vector<celda> celdas;

    // Evaluar todas las posibles direcciones de movimiento
    for (int i = 0; i < 8; ++i) {
        int nx = *x + dx[i];
        int ny = *y + dy[i];
        
        // Verificar que la nueva posición esté dentro de los límites del mapa y que no esté bloqueada ('X')
        if (!esta_libre(nx, ny, *mapa)) {
            continue; // Posición bloqueada o fuera de límites
        }
        
        // Si el movimiento es diagonal, verificar las celdas adyacentes
        bool es_diagonal = (dx[i] != 0) && (dy[i] != 0);
        if (es_diagonal) {
            // Determinar las posiciones adyacentes horizontales y verticales
            int adj_x1 = *x + dx[i];
            int adj_y1 = *y; // Movimiento horizontal
            int adj_x2 = *x;
            int adj_y2 = *y + dy[i]; // Movimiento vertical
            
            bool adj1_libre = esta_libre(adj_x1, adj_y1, *mapa);
            bool adj2_libre = esta_libre(adj_x2, adj_y2, *mapa);
            
            // Si ambas celdas adyacentes están bloqueadas, no se puede mover diagonalmente
            if (!adj1_libre && !adj2_libre) {
                continue;
            }
        }
        
        // Calcular la distancia desde la nueva posición al objetivo
        int distancia = calcular_distancia(nx, ny, objetivo_x, objetivo_y);
        
        // Obtener el número de veces que la casilla ya ha sido visitada
        int visitas = obtener_visitas(nx, ny, *memoria);
        
        // Agregar la celda al vector
        celdas.push_back({i, visitas, distancia});
    }
    
    // Ordenar las celdas primero por R (menor número de visitas), luego por D (distancia ascendente)
    std::sort(celdas.begin(), celdas.end(), comparar_celdas);
    
    // Si hay al menos una celda disponible, mover a la posición más óptima
    if (!celdas.empty()) {
        int mejor_movimiento = celdas[0].I; // Índice del mejor movimiento
        mejor_x = *x + dx[mejor_movimiento];
        mejor_y = *y + dy[mejor_movimiento];
        
        // Actualizar las coordenadas de la reina
        *x = mejor_x;
        *y = mejor_y;
        
        // Incrementar el contador de visitas en la nueva posición
        if (mejor_y >= 0 && mejor_y < memoria->size() && mejor_x >= 0 && mejor_x < (*memoria)[0].size()) {
            (*memoria)[mejor_y][mejor_x] += 1; // Incrementa el contador de visitas
        }
    }
    else {
        // No hay movimientos disponibles que no hayan sido visitados
    }
}



};
