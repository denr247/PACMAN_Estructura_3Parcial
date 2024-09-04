#include <array>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
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


enum class GameState {
    Menu,
    Playing,
    GameOver,
    LevelComplete
};

int main()
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("audio/intro.ogg")) {
        std::cout << "Error" << std::endl;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.setVolume(50);
    sound.play();

    sf::SoundBuffer deathBuffer;
    if (!deathBuffer.loadFromFile("audio/death.ogg")) {
        std::cout << "Error loading death sound" << std::endl;
    }
    sf::Sound deathSound;
    deathSound.setBuffer(deathBuffer);
    deathSound.setVolume(50);

    Nivel nivel;
    GhostManager ghost_manager;
    Pacman pacman;
    GameState gameState = GameState::Menu;
    std::string playerName = "";
    bool respawning = false;
    bool play_death_sound = false;
    unsigned lag = 0;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    std::chrono::time_point<std::chrono::steady_clock> respawn_start_time;
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
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::TextEntered && gameState == GameState::Menu)
                {
                    if (event.text.unicode < 128)
                    {
                        if (event.text.unicode == '\b' && !playerName.empty())
                            playerName.pop_back();
                        else if (event.text.unicode != '\b')
                            playerName += static_cast<char>(event.text.unicode);
                    }
                }
                else if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (gameState == GameState::Menu && !playerName.empty())
                        {
                            gameState = GameState::Playing;
                        }
                        else if (gameState == GameState::GameOver || gameState == GameState::LevelComplete)
                        {
                            nivel.reiniciarNivel();
                            pacman.reiniciar_vidas();
                            pacman.reset();
                            map = convert_sketch(nivel.getMapaActual(), ghost_positions, pacman);
                            ghost_manager.reset(nivel.getNivelActual(), ghost_positions);
                            gameState = GameState::Playing;
                        }
                    }
                }
            }

            if (gameState == GameState::Playing)
            {
                if (pacman.get_dead())
                {
                    if (!respawning)
                    {
                        if (!play_death_sound)
                        {
                            deathSound.play();
                            play_death_sound = true;
                        }
                        respawn_start_time = std::chrono::steady_clock::now();
                        respawning = true;
                    }

                    auto now = std::chrono::steady_clock::now();
                    std::chrono::duration<float> elapsed = now - respawn_start_time;

                    if (elapsed.count() >= 3.0f)
                    {
                        pacman.perder_vida();
                        if (pacman.get_vidas() > 0)
                        {
                            ghost_manager.reset(nivel.getNivelActual(), ghost_positions);
                            pacman.reset();
                            pacman.set_dead(false);
                            respawning = false;
                            play_death_sound = false;
                        }
                        else
                        {

                            gameState = GameState::GameOver;
                            respawning = false;
                        }
                    }
                }
                else
                {
                    pacman.update(nivel.getNivelActual(), map);
                    ghost_manager.update(nivel.getNivelActual(), map, pacman);

                    bool level_complete = true;
                    for (const auto& column : map)
                    {
                        for (const auto& cell : column)
                        {
                            if (cell == Cell::Pellet)
                            {
                                level_complete = false;
                                break;
                            }
                        }
                        if (!level_complete) break;
                    }

                    if (level_complete)
                    {
                        gameState = GameState::LevelComplete;
                    }
                }
            }

            if (gameState == GameState::LevelComplete)
            {
                nivel.avanzarNivel();
                pacman.reset();
                map = convert_sketch(nivel.getMapaActual(), ghost_positions, pacman);
                ghost_manager.reset(nivel.getNivelActual(), ghost_positions);
                gameState = GameState::Playing;
            }

            if (FRAME_DURATION > lag)
            {
                window.clear();

                switch (gameState)
                {
                    case GameState::Menu:
                        draw_text(1, 0, 0, "Ingresa tu nombre: " + playerName + "\nPresiona Enter para empezar", window);
                        break;

                    case GameState::Playing:
                        draw_map(map, window);
                        ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);
                        draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Nivel: " + std::to_string(nivel.getNivelActual()) + " | Puntacion: " + std::to_string(pacman.get_puntuacion()) + " | Vidas: " + std::to_string(pacman.get_vidas()) + " | Jugador: " + playerName, window);
                        pacman.draw(false, window);
                        break;

                    case GameState::GameOver:
                        draw_text(1, 0, 0, "Juego terminado, " + playerName + "\nPresiona Enter para volver empezar", window);
                        break;

                    case GameState::LevelComplete:
                        draw_text(1, 0, 0, "Nivel Completado!\nPresiona Enter para avanzar", window);
                        break;
                }

                window.display();
            }
        }
    }

    return 0;
}
