#include <array>
#include <cmath>
//#include <SFML/Graphics.hpp>
#include "Global.hpp"
#include "Pacman.hpp"
//#include "ColisionConElMapa.hpp"

/*
    Constructor
*/

Pacman::Pacman() :
    animacion_pacmanMuere(0),
    esta_muerto(0),
    direccion(0),
    temporizador_energizador(0),
    posicion({0, 0}){}

bool Pacman::get_animacion_pacmanMuere()
{
	return animacion_pacmanMuere;
}

bool Pacman::get_esta_muerto()
{
	return esta_muerto;
}

unsigned char Pacman::get_direccion()
{
	return direccion;
}

unsigned short Pacman::get_temporizador_energizador()
{
	return temporizador_energizador;
}

void Pacman::dibujar(bool i_victoria, sf::RenderWindow& i_ventana)
{
	unsigned char fotograma = static_cast<unsigned char>(floor(temporizador_animacion / static_cast<float>(PACMAN_ANIMATION_SPEED)));

	sf::Sprite sprite;

	sf::Texture textura;

	sprite.setPosition(posicion.x, posicion.y);

	if (1 == esta_muerto || 1 == i_victoria)
	{
		if (temporizador_animacion < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
		{
			temporizador_animacion++;

			textura.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");

			sprite.setTexture(textura);
			sprite.setTextureRect(sf::IntRect(CELL_SIZE * fotograma, 0, CELL_SIZE, CELL_SIZE));

			i_ventana.draw(sprite);
		}
		else
		{
			// Solo puedes morir una vez.
            //aqui se modifica las vidas 
			animacion_pacmanMuere = 1;
		}
	}
	else
	{
		textura.loadFromFile("Resources/Images/Pacman" + std::to_string(CELL_SIZE) + ".png");

		sprite.setTexture(textura);
		sprite.setTextureRect(sf::IntRect(CELL_SIZE * fotograma, CELL_SIZE * direccion, CELL_SIZE, CELL_SIZE));

		i_ventana.draw(sprite);

		temporizador_animacion = (1 + temporizador_animacion) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
	}
}

void Pacman::reiniciar()
{
    /*
        Aqui iniciamos todo en 0 por los valores tipo bool
    */
	animacion_pacmanMuere = 0;
	esta_muerto = 0;
	direccion = 0;
	temporizador_animacion = 0;
	temporizador_energizador = 0;
}

void Pacman::establecer_temporizador_animacion(unsigned short i_temporizador_animacion)
{
	temporizador_animacion = i_temporizador_animacion;
}

void Pacman::establecer_muerto(bool i_muerto)
{
	esta_muerto = i_muerto;

	if (1 == esta_muerto)
	{
		// Asegurándose de que la animación comience desde el principio.
		temporizador_animacion = 0;
	}
}

void Pacman::establecer_posicion(short i_x, short i_y)
{
	posicion = {i_x, i_y};
}

void Pacman::actualizar(unsigned char i_nivel, std::array<std::array<Celda, MAP_HEIGHT>, MAP_WIDTH>& i_mapa)
{
	std::array<bool, 4> paredes{};
	paredes[0] = colision_mapa(0, 0, PACMAN_SPEED + posicion.x, posicion.y, i_mapa);
	paredes[1] = colision_mapa(0, 0, posicion.x, posicion.y - PACMAN_SPEED, i_mapa);
	paredes[2] = colision_mapa(0, 0, posicion.x - PACMAN_SPEED, posicion.y, i_mapa);
	paredes[3] = colision_mapa(0, 0, posicion.x, PACMAN_SPEED + posicion.y, i_mapa);

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (0 == paredes[0]) // No puedes girar en esta dirección si hay una pared allí.
		{
			direccion = 0;
		}
	}

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (0 == paredes[1])
		{
			direccion = 1;
		}
	}

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (0 == paredes[2])
		{
			direccion = 2;
		}
	}

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (0 == paredes[3])
		{
			direccion = 3;
		}
	}

	if (0 == paredes[direccion])
	{
		switch (direccion)
		{
			case 0:
			{
				posicion.x += PACMAN_SPEED;

				break;
			}
			case 1:
			{
				posicion.y -= PACMAN_SPEED;

				break;
			}
			case 2:
			{
				posicion.x -= PACMAN_SPEED;

				break;
			}
			case 3:
			{
				posicion.y += PACMAN_SPEED;
			}
		}
	}

	if (-CELL_SIZE >= posicion.x)
	{
		posicion.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED;
	}
	else if (CELL_SIZE * MAP_WIDTH <= posicion.x)
	{
		posicion.x = PACMAN_SPEED - CELL_SIZE;
	}

	if (1 == colision_mapa(1, 0, posicion.x, posicion.y, i_mapa)) // Cuando Pacman come un energizador...
	{
		// ¡Se energiza!
		temporizador_energizador = static_cast<unsigned short>(ENERGIZER_DURATION / pow(2, i_nivel));
	}
	else
	{
		temporizador_energizador = std::max(0, temporizador_energizador - 1);
	}
}

Posicion Pacman::obtener_posicion()
{
	return posicion;
}
