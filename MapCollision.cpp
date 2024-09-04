#include <array>
#include <cmath>
#include <iostream>
#include "Headers/Global.hpp"
#include "Headers/MapCollision.hpp"
//#include "Headers/Puntuacion.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


sf::SoundBuffer pelletBuffer;
sf::Sound pelletSound;
bool pelletSoundPlaying = false;

//COLISIONES PARA LOS FANTASMAS

bool map_collision(bool i_collect_pellets, bool i_use_door, short i_x, short i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
	bool output = 0;
	//Getting the exact position.
	float cell_x = i_x / static_cast<float>(CELL_SIZE);
	float cell_y = i_y / static_cast<float>(CELL_SIZE);

	//A ghost/Pacman can intersect 4 cells at most.
	for (unsigned char a = 0; a < 4; a++)
	{
		short x = 0;
		short y = 0;

		switch (a)
		{
			case 0: //Top left cell
			{
				x = static_cast<short>(floor(cell_x));
				y = static_cast<short>(floor(cell_y));

				break;
			}
			case 1: //Top right cell
			{
				x = static_cast<short>(ceil(cell_x));
				y = static_cast<short>(floor(cell_y));

				break;
			}
			case 2: //Bottom left cell
			{
				x = static_cast<short>(floor(cell_x));
				y = static_cast<short>(ceil(cell_y));

				break;
			}
			case 3: //Bottom right cell
			{
				x = static_cast<short>(ceil(cell_x));
				y = static_cast<short>(ceil(cell_y));
			}
		}

		//Making sure that the position is inside the map.
		if (0 <= x && 0 <= y && MAP_HEIGHT > y && MAP_WIDTH > x)
		{
			if (0 == i_collect_pellets) //Here we only care about the walls.
			{
				if (Cell::Wall == i_map[x][y])
				{
					output = 1;
				}
				else if (0 == i_use_door && Cell::Door == i_map[x][y])
				{
					output = 1;
				}
			}
			else //Here we only care about the collectables.
			{
				if (Cell::Energizer == i_map[x][y])
				{
					output = 1;

					i_map[x][y] = Cell::Empty;
				}
				else if (Cell::Pellet == i_map[x][y]){
                    if (!pelletSoundPlaying) {
                        pelletBuffer.loadFromFile("audio/eat2.wav");
                        pelletSound.setBuffer(pelletBuffer);
                        pelletSound.play();
                        pelletSoundPlaying = true;
                    }

                    i_map[x][y] = Cell::Empty;


				}
				if (pelletSoundPlaying && pelletSound.getStatus() == sf::Sound::Stopped) {
                    pelletSoundPlaying = false;
                }
			}
		}
	}

	return output;
}

// PARA LAS COLISIONES DEL PACMAN
bool map_collision_pacman(bool i_collect_pellets, bool i_use_door, short i_x, short i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, int &puntuacion)
{
    bool output = false;
    float cell_x = i_x / static_cast<float>(CELL_SIZE);
    float cell_y = i_y / static_cast<float>(CELL_SIZE);

    for (unsigned char a = 0; a < 4; a++)
    {
        short x = 0;
        short y = 0;

        switch (a)
        {
            case 0: // Top left cell
                x = static_cast<short>(floor(cell_x));
                y = static_cast<short>(floor(cell_y));
                break;
            case 1: // Top right cell
                x = static_cast<short>(ceil(cell_x));
                y = static_cast<short>(floor(cell_y));
                break;
            case 2: // Bottom left cell
                x = static_cast<short>(floor(cell_x));
                y = static_cast<short>(ceil(cell_y));
                break;
            case 3: // Bottom right cell
                x = static_cast<short>(ceil(cell_x));
                y = static_cast<short>(ceil(cell_y));
                break;
        }

        // Check if coordinates are within map bounds
        if (0 <= x && 0 <= y && MAP_HEIGHT > y && MAP_WIDTH > x)
        {
            if (0 == i_collect_pellets)
            {
                if (Cell::Wall == i_map[x][y] || (0 == i_use_door && Cell::Door == i_map[x][y]))
                {
                    output = true;
                }
            }
            else
            {
                if (Cell::Energizer == i_map[x][y])
                {
                    output = true;
                    puntuacion += 10;  // Incrementa la puntuación por energizador, me puede servir para las cerezas

                    i_map[x][y] = Cell::Empty;
                }
                else if (Cell::Pellet == i_map[x][y])
                {
                    puntuacion += 1;  // Incrementa la puntuación por comer comidita
                    if (!pelletSoundPlaying) {
                        pelletBuffer.loadFromFile("audio/eat2.wav");
                        pelletSound.setBuffer(pelletBuffer);
                        pelletSound.play();
                        pelletSoundPlaying = true;
                    }
                    i_map[x][y] = Cell::Empty;
                }
                else if(Cell::Fruit == i_map[x][y])
                {
                    sf::Text text;
                    puntuacion += 50;
                    text.setString("50");
                    // Borra la fruta actual del mapa
                    i_map[x][y] = Cell::Empty;

                    // Encuentra una nueva posición aleatoria para la fruta
                    bool placed = false;
                    while (!placed)
                    {
                        int new_x = rand() % MAP_WIDTH;
                        int new_y = rand() % MAP_HEIGHT;

                        // Asegúrate de que la nueva posición esté dentro del mapa y que sea una celda vacía
                        if (new_x >= 0 && new_x < MAP_WIDTH && new_y >= 0 && new_y < MAP_HEIGHT && i_map[new_x][new_y] == Cell::Empty)
                        {
                            i_map[new_x][new_y] = Cell::Fruit;
                            placed = true;
                        }
                    }
                }
            }
        }
    }

    // Update pellet sound status
    if (pelletSoundPlaying && pelletSound.getStatus() == sf::Sound::Stopped) {
        pelletSoundPlaying = false;
    }

    return output;
}
