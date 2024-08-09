
#include <array>
#include <cmath>

#include "Headers/Global.hpp"
#include "Headers/ColisionConElMapa.hpp" // Cambiado para reflejar la traducción

// Función para verificar colisiones con el mapa
bool colision_con_el_mapa(bool recoger_pellets, bool usar_puerta, short x, short y, std::array<std::array<Celda, ALTURA_DEL_MAPA>, ANCHO_DEL_MAPA>& mapa)
{
    bool resultado = false;

    // Obtener la posición exacta.
    float celda_x = x / static_cast<float>(TAMAÑO_DE_CELDA);
    float celda_y = y / static_cast<float>(TAMAÑO_DE_CELDA);

    // Un fantasma/Pacman puede intersectar a lo sumo 4 celdas.
    for (unsigned char i = 0; i < 4; i++)
    {
        short coord_x = 0;
        short coord_y = 0;

        switch (i)
        {
            case 0: // Celda superior izquierda
            {
                coord_x = static_cast<short>(floor(celda_x));
                coord_y = static_cast<short>(floor(celda_y));
                break;
            }
            case 1: // Celda superior derecha
            {
                coord_x = static_cast<short>(ceil(celda_x));
                coord_y = static_cast<short>(floor(celda_y));
                break;
            }
            case 2: // Celda inferior izquierda
            {
                coord_x = static_cast<short>(floor(celda_x));
                coord_y = static_cast<short>(ceil(celda_y));
                break;
            }
            case 3: // Celda inferior derecha
            {
                coord_x = static_cast<short>(ceil(celda_x));
                coord_y = static_cast<short>(ceil(celda_y));
                break;
            }
        }

        // Asegurarse de que la posición está dentro del mapa.
        if (0 <= coord_x && 0 <= coord_y && ALTURA_DEL_MAPA > coord_y && ANCHO_DEL_MAPA > coord_x)
        {
            if (!recoger_pellets) // Solo nos preocupamos por las paredes.
            {
                if (Celda::Pared == mapa[coord_x][coord_y])
                {
                    resultado = true; // Hay una pared.
                }
                else if (!usar_puerta && Celda::Puerta == mapa[coord_x][coord_y])
                {
                    resultado = true; // Hay una puerta y no se debe usar.
                }
            }
            else // Solo nos preocupamos por los coleccionables.
            {
                if (Celda::Energizador == mapa[coord_x][coord_y])
                {
                    resultado = true; // Hay un energizador.
                    mapa[coord_x][coord_y] = Celda::Vacío; // Eliminar el energizador.
                }
                else if (Celda::Pellet == mapa[coord_x][coord_y])
                {
                    mapa[coord_x][coord_y] = Celda::Vacío; // Eliminar el pellet.
                }
            }
        }
    }

    return resultado;
}

