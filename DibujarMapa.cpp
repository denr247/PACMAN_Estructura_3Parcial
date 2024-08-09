#include <array>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/DibujarMapa.hpp" // Cambiado para reflejar la traducción

// Función para dibujar el mapa
void dibujar_mapa(const std::array<std::array<Celda, ALTURA_DEL_MAPA>, ANCHO_DEL_MAPA>& mapa, sf::RenderWindow& ventana)
{
    sf::Sprite sprite;

    sf::Texture textura;
    textura.loadFromFile("Resources/Images/Mapa" + std::to_string(TAMAÑO_DE_CELDA) + ".png"); // Cambiado para reflejar el nombre en español

    sprite.setTexture(textura);

    for (unsigned char x = 0; x < ANCHO_DEL_MAPA; x++)
    {
        for (unsigned char y = 0; y < ALTURA_DEL_MAPA; y++)
        {
            sprite.setPosition(static_cast<float>(TAMAÑO_DE_CELDA * x), static_cast<float>(TAMAÑO_DE_CELDA * y));

            // Recortamos solo lo que necesitamos de la textura.
            switch (mapa[x][y])
            {
                case Celda::Puerta:
                {
                    sprite.setTextureRect(sf::IntRect(2 * TAMAÑO_DE_CELDA, TAMAÑO_DE_CELDA, TAMAÑO_DE_CELDA, TAMAÑO_DE_CELDA));
                    ventana.draw(sprite);
                    break;
                }
                case Celda::Energizador:
                {
                    sprite.setTextureRect(sf::IntRect(TAMAÑO_DE_CELDA, TAMAÑO_DE_CELDA, TAMAÑO_DE_CELDA, TAMAÑO_DE_CELDA));
                    ventana.draw(sprite);
                    break;
                }
                case Celda::Pellet:
                {
                    sprite.setTextureRect(sf::IntRect(0, TAMAÑO_DE_CELDA, TAMAÑO_DE_CELDA, TAMAÑO_DE_CELDA));
                    ventana.draw(sprite);
                    break;
                }
                case Celda::Pared:
                {
                    bool abajo = false;
                    bool izquierda = false;
                    bool derecha = false;
                    bool arriba = false;

                    if (y < ALTURA_DEL_MAPA - 1)
                    {
                        if (Celda::Pared == mapa[x][y + 1])
                        {
                            abajo = true;
                        }
                    }

                    // Como tenemos túneles de teletransporte, necesitamos dibujarlos como si estuvieran conectados.
                    if (x > 0)
                    {
                        if (Celda::Pared == mapa[x - 1][y])
                        {
                            izquierda = true;
                        }
                    }
                    else
                    {
                        izquierda = true;
                    }

                    if (x < ANCHO_DEL_MAPA - 1)
                    {
                        if (Celda::Pared == mapa[x + 1][y])
                        {
                            derecha = true;
                        }
                    }
                    else
                    {
                        derecha = true;
                    }

                    if (y > 0)
                    {
                        if (Celda::Pared == mapa[x][y - 1])
                        {
                            arriba = true;
                        }
                    }

                    //--------------------------------------------<         ¡PROPIEDAD DISTRIBUTIVA!         >----------------------------
                    sprite.setTextureRect(sf::IntRect(TAMAÑO_DE_CELDA * (abajo + 2 * (izquierda + 2 * (derecha + 2 * arriba))), 0, TAMAÑO_DE_CELDA, TAMAÑO_DE_CELDA));
                    ventana.draw(sprite);
                    break;
                }
            }
        }
    }
}
