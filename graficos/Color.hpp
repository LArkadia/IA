#ifndef COLOR_IMPORTADO
#define COLOR_IMPORTADO
#include <stdint.h>

class Color
{
private:
    uint8_t rojo;
    uint8_t verde;
    uint8_t azul;
    uint8_t transparencia;
public:
    Color(uint8_t rojo,uint8_t verde,uint8_t azul,uint8_t transparencia);
    Color(uint8_t rojo,uint8_t verde,uint8_t azul);
    uint8_t R();
    uint8_t G();
    uint8_t B();
    uint8_t A();
    ~Color();
};

Color::Color(uint8_t rojo,uint8_t verde,uint8_t azul,uint8_t transparencia){
    this->rojo = rojo;this->verde = verde;this->azul = azul;this->transparencia = transparencia;
}
Color::Color(uint8_t rojo,uint8_t verde,uint8_t azul){
    this->rojo = rojo;this->verde = verde;this->azul = azul;
}
uint8_t Color::R(){return this->rojo;}
uint8_t Color::G(){return this->verde;}
uint8_t Color::B(){return this->azul;}
uint8_t Color::A(){return this->transparencia;}

Color::~Color(){}






#endif