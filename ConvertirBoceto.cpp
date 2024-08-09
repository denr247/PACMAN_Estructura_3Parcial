#include <array>
#include <string>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/Pacman.hpp"
#include "Headers/ConvertirBoceto.hpp" // Cambiado para reflejar la traducci�n

// Funci�n para convertir el boceto en un mapa
std::array<std::array<Celda, ALTURA_DEL_MAPA>, ANCHO_DEL_MAPA> convertir_boceto(const std::array<std::string, ALTURA_DEL_MAPA>& boceto_mapa, std::array<Posicion, 4>& posiciones_fantasma, Pacman& pacman)
{
    // Inicializaci�n del mapa de salida
    std::array<std::array<Celda, ALTURA_DEL_MAPA>, ANCHO_DEL_MAPA> mapa_salida{};

    for (unsigned char a = 0; a < ALTURA_DEL_MAPA; a++)
    {
        for (unsigned char b = 0; b < ANCHO_DEL_MAPA; b++)
        {
            // Por defecto, cada celda est� vac�a.
            mapa_salida[b][a] = Celda::Vac�o;

            switch (boceto_mapa[a][b])
            {
                //#pared #obst�culo #no_puedes_pasarme
                case '#':
                {
                    mapa_salida[b][a] = Celda::Pared;
                    break;
                }
                case '=':
                {
                    mapa_salida[b][a] = Celda::Puerta;
                    break;
                }
                case '.':
                {
                    mapa_salida[b][a] = Celda::Pellet;
                    break;
                }
                // Fantasma rojo
                case '0':
                {
                    posiciones_fantasma[0].x = TAMA�O_DE_CELDA * b;
                    posiciones_fantasma[0].y = TAMA�O_DE_CELDA * a;
                    break;
                }
                // Fantasma rosa
                case '1':
                {
                    posiciones_fantasma[1].x = TAMA�O_DE_CELDA * b;
                    posiciones_fantasma[1].y = TAMA�O_DE_CELDA * a;
                    break;
                }
                // Fantasma azul (cian)
                case '2':
                {
                    posiciones_fantasma[2].x = TAMA�O_DE_CELDA * b;
                    posiciones_fantasma[2].y = TAMA�O_DE_CELDA * a;
                    break;
                }
                // Fantasma naranja
                case '3':
                {
                    posiciones_fantasma[3].x = TAMA�O_DE_CELDA * b;
                    posiciones_fantasma[3].y = TAMA�O_DE_CELDA * a;
                    break;
                }
                // �Pacman!
                case 'P':
                {
                    pacman.establecer_posicion(TAMA�O_DE_CELDA * b, TAMA�O_DE_CELDA * a);
                    break;
                }
                // Esto parece una cara sorprendida.
                case 'o':
                {
                    mapa_salida[b][a] = Celda::Energizador;
                    break;
                }
            }
        }
    }

    return mapa_salida;
}
