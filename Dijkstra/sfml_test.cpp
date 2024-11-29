#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Crear una ventana de 800x600 píxeles
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test Window");

    // Bucle principal
    while (window.isOpen()) {
        // Manejo de eventos
        sf::Event event;
        while (window.pollEvent(event)) {
            // Cerrar la ventana
            if (event.type == sf::Event::Closed) {
                std::cout << "Ventana cerrada." << std::endl;
                window.close();
            }
            // Detectar clics del mouse
            if (event.type == sf::Event::MouseButtonPressed) {
                std::cout << "Mouse presionado en ("
                          << event.mouseButton.x << ", "
                          << event.mouseButton.y << ")" << std::endl;
            }
        }

        // Limpiar la ventana con color azul
        window.clear(sf::Color::Blue);

        // Mostrar el contenido en la pantalla
        window.display();
    }

    return 0;
}
