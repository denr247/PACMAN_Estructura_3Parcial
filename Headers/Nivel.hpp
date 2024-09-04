#pragma once

#include <array>
#include <vector>
#include <string>
#include "Global.hpp"

class Nivel {
private:
    // Definición de mapas como una matriz de mapas
    std::array<std::array<std::string, MAP_HEIGHT>, MAP_WIDTH> mapas;
    int nivelActual;

public:
    Nivel();

    // Método para obtener el mapa actual en formato de texto
    std::array<std::string, MAP_HEIGHT> getMapaActual() const;

    // Método para avanzar al siguiente nivel
    void avanzarNivel();

    // Método para reiniciar al primer nivel cuando el Pacman muere
    void reiniciarNivel();

    // Método para obtener el nivel actual
    int getNivelActual() const;
};
