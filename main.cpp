#include <array>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Global.hpp"
#include "Headers/DrawText.hpp"
#include "Pacman.hpp"
#include "Headers/Ghost.hpp"
#include "Headers/GhostManager.hpp"
#include "Headers/ConvertSketch.hpp"
#include "Headers/DrawMap.hpp"
#include "Headers/MapCollision.hpp"
#include "Headers/Nivel.hpp"

enum GameState { Menu, Playing, GameOver };

int main()
{
    GameState gameState = Menu;
    std::string playerName;

    // Cargar y reproducir sonido de introducción
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("audio/intro.ogg")) {
        std::cout << "Error" << std::endl;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.setVolume(50);
    sound.play();

    // Cargar sonido de muerte
    sf::SoundBuffer deathBuffer;
    if (!deathBuffer.loadFromFile("audio/death.ogg")) {
        std::cout << "Error loading death sound" << std::endl;
    }
    sf::Sound deathSound;
    deathSound.setBuffer(deathBuffer);
    deathSound.setVolume(50);

    // Instanciar la clase Nivel
    Nivel nivel;
    GhostManager ghost_manager;
    Pacman pacman;
    bool game_won = false;
    bool game_over = false;
    bool respawning = false; // Variable para controlar el estado de respawn
    bool play_death_sound = false; // Variable para controlar la reproducción del sonido de muerte
    unsigned lag = 0;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    std::chrono::time_point<std::chrono::steady_clock> respawn_start_time; // Tiempo de inicio del respawn
    sf::Event event;
    std::array<Position, 4> ghost_positions;
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

    auto mapaActual = nivel.getMapaActual();
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map = convert_sketch(mapaActual, ghost_positions, pacman);
    ghost_manager.reset(nivel.getNivelActual(), ghost_positions);

    previous_time = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
        lag += delta_time;
        previous_time += std::chrono::microseconds(delta_time);

        while (FRAME_DURATION <= lag)
        {
            lag -= FRAME_DURATION;

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (gameState == Menu) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == '\b') {  // Si se presiona la tecla de retroceso
                            if (!playerName.empty()) {
                                playerName.pop_back();
                            }
                        }
                        else if (event.text.unicode == '\r') {  // Si se presiona Enter
                            gameState = Playing;
                        }
                        else if (event.text.unicode < 128) {  // Para caracteres ASCII
                            playerName += static_cast<char>(event.text.unicode);
                        }
                    }
                }
            }

            if (gameState == Playing) {
                if (pacman.get_dead()) {
                    if (!respawning) {
                        if (!play_death_sound) {
                            deathSound.play();
                            play_death_sound = true;
                        }
                        respawn_start_time = std::chrono::steady_clock::now();
                        respawning = true;
                    }

                    auto now = std::chrono::steady_clock::now();
                    std::chrono::duration<float> elapsed = now - respawn_start_time;

                    if (elapsed.count() >= 3.0f) {
                        pacman.perder_vida();
                        if (pacman.get_vidas() > 0) {
                            ghost_manager.reset(nivel.getNivelActual(), ghost_positions);  // Reiniciar solo los fantasmas
                            pacman.reset();  // Reiniciar Pac-Man (incluye posición)
                            pacman.set_dead(false);  // Establecer Pac-Man como no muerto
                            respawning = false;
                            play_death_sound = false;
                            game_won = false;
                            game_over = false;
                        } else {
                            gameState = GameOver;  // Cambiar el estado a Game Over
                            respawning = false;
                        }
                    }
                }
                else // Si Pac-Man no está muerto, actualizar el juego normalmente
                {
                    pacman.update(nivel.getNivelActual(), map);
                    ghost_manager.update(nivel.getNivelActual(), map, pacman);

                    game_won = true;
                    for (const auto& column : map)
                    {
                        for (const auto& cell : column)
                        {
                            if (cell == Cell::Pellet)
                            {
                                game_won = false;
                                break;
                            }
                        }
                        if (!game_won) break;
                    }
                }

                if (gameState == GameOver)
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                    {
                        nivel.reiniciarNivel(); // Dependiendo de cómo manejes los niveles
                        pacman.reiniciar_vidas(); // Restablece las vidas a 3
                        pacman.reset();
                        map = convert_sketch(nivel.getMapaActual(), ghost_positions, pacman); // Reiniciar el mapa completo solo cuando el juego termina
                        ghost_manager.reset(nivel.getNivelActual(), ghost_positions);
                        game_won = false;
                        gameState = Menu;  // Regresar al menú
                    }
                }
                else
                {
                    if (game_won)
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                        {
                            nivel.avanzarNivel();
                            map = convert_sketch(nivel.getMapaActual(), ghost_positions, pacman);
                            ghost_manager.reset(nivel.getNivelActual(), ghost_positions);
                            pacman.reset();
                            pacman.set_dead(false); // Asegúrate de marcar Pac-Man como no muerto
                            game_won = false;
                        }
                    }
                }
            }

            if (FRAME_DURATION > lag)
            {
                window.clear();

                if (gameState == Menu) {
                    draw_text(true, 0, 0, "Bienvenido a Pac Man!\nIngresa tu nombre: " + playerName, window);
                }
                else if (gameState == Playing) {
                    if (!game_won && !pacman.get_dead() && !game_over)
                    {
                        draw_map(map, window);
                        ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);
                        draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Jugador: " + playerName + "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tNivel: " + std::to_string(nivel.getNivelActual()) + "\t\t\t\t\t\t\t\t\t\t\t\t\t\tScore: " + std::to_string(pacman.get_puntuacion()) + "\t\t\t\t\t\t\t\t\t\t\t\t\t\tVidas: " + std::to_string(pacman.get_vidas()), window);
                        pacman.draw(game_won, window);
                    }


                    if (game_won || pacman.get_dead() || gameState == GameOver)
                    {
                        if (game_won)
                        {
                            draw_text(true, 0, 0, "Next level!\n Press Enter to continue", window);
                        }
                        else if (gameState == GameOver)
                        {
                            draw_text(true, 0, 0, "Game over\n Press Enter to restart", window);
                        }
                        else
                        {
                            draw_text(true, 0, 0, "Pac-Man died", window);
                        }
                    }
                }

                window.display();
            }
        }
    }
}
