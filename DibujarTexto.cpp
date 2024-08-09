#include <cmath>
//#include <SFML/Graphics.hpp>

//#include "DibujarTexto.hpp" // Cambiado para reflejar la traducci�n
#include "Global.hpp"

// Funci�n para dibujar texto
void dibujar_texto(bool centrar, unsigned short x, unsigned short y, const std::string& texto, sf::RenderWindow& ventana)
{
    short x_caracter = x;
    short y_caracter = y;

    unsigned char ancho_caracter;

    sf::Sprite sprite_caracter;

    sf::Texture textura_fuente;
    textura_fuente.loadFromFile("Resources/Images/Fuente.png"); // Cambiado para reflejar el nombre en espa�ol

    // Hay 96 caracteres en la textura de la fuente.
    ancho_caracter = textura_fuente.getSize().x / 96;

    sprite_caracter.setTexture(textura_fuente);

    if (centrar)
    {
        // Pas� HORAS tratando de hacer que esto funcione.
        // Si conoces una mejor manera de hacerlo, por favor, d�melo.
        x_caracter = static_cast<short>(round(0.5f * (TAMA�O_CELDA * ANCHO_MAPA - ancho_caracter * texto.substr(0, texto.find_first_of('\n')).size())));
        y_caracter = static_cast<short>(round(0.5f * (TAMA�O_CELDA * ALTURA_MAPA - ALTURA_FUENTE * (1 + std::count(texto.begin(), texto.end(), '\n')))));
    }

    for (std::string::const_iterator it = texto.begin(); it != texto.end(); it++)
    {
        if ('\n' == *it)
        {
            if (centrar)
            {
                // A�n no entiendo lo que escrib� aqu�.
                x_caracter = static_cast<short>(round(0.5f * (TAMA�O_CELDA * ANCHO_MAPA - ancho_caracter * texto.substr(1 + it - texto.begin(), texto.find_first_of('\n', 1 + it - texto.begin()) - (1 + it - texto.begin())).size())));
            }
            else
            {
                x_caracter = x;
            }

            y_caracter += ALTURA_FUENTE;

            continue;
        }

        sprite_caracter.setPosition(x_caracter, y_caracter);
        // La textura de la fuente comienza con un car�cter de espacio, que es el car�cter 32.
        sprite_caracter.setTextureRect(sf::IntRect(ancho_caracter * (*it - 32), 0, ancho_caracter, ALTURA_FUENTE));

        x_caracter += ancho_caracter;

        ventana.draw(sprite_caracter); // Aseg�rate de que esto est� en el c�digo para dibujar el texto
    }
}
