#ifndef ADMINISTRADORDE FANTASMAS_HPP
#define ADMINISTRADORDE FANTASMAS_HPP

#include <array>
#include "Global.hpp"
#include "Pacman.hpp"
#include "Fantasma.hpp"

class AdministradorDeFantasmas
{
public:
    AdministradorDeFantasmas(); // Constructor

    void dibujar(bool parpadeo, sf::RenderWindow& ventana); // Dibuja los fantasmas
    void reiniciar(unsigned char nivel, const std::array<Posicion, 4>& posiciones_fantasmas); // Reinicia los fantasmas
    void actualizar(unsigned char nivel, std::array<std::array<Celda, ALTURA_DEL_MAPA>, ANCHO_DEL_MAPA>& mapa, Pacman& pacman); // Actualiza los fantasmas

private:
    unsigned char onda_actual; // Onda actual
    unsigned short temporizador_onda; // Temporizador para la onda
    std::array<Fantasma, 4> fantasmas; // Arreglo de fantasmas
};

#endif // ADMINISTRADORDE FANTASMAS_HPP

