#include <array>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Global.hpp"
#include "Pacman.hpp"
#include "Headers/MapCollision.hpp"

namespace {
    sf::SoundBuffer buffer;
    sf::Sound sound;

    sf::SoundBuffer death_buffer;
    sf::Sound death_sound;

    sf::SoundBuffer pellet_buffer;
    sf::Sound pellet_sound;
}

Pacman::Pacman() :
    animation_over(0),
    dead(0),
    direction(0),
    energizer_timer(0),
    posicion_inicial({(MAP_WIDTH * CELL_SIZE) / 2 - CELL_SIZE / 2 - CELL_SIZE, (MAP_HEIGHT * CELL_SIZE) * 5 / 7}),
    position(posicion_inicial),
    puntuacion(0),
    vidas(3)
{
    // Carga el archivo de sonido
    if (!buffer.loadFromFile("audio/scared.wav")) {
        std::cout << "Error loading sound file!" << std::endl;
    }else{
    sound.setBuffer(buffer);
    sound.setVolume(20);
    }
     if (!death_buffer.loadFromFile("audio/death.ogg")) {
        std::cout << "Error loading death sound file!" << std::endl;
    } else {
        death_sound.setBuffer(death_buffer);
    }

    if (!pellet_buffer.loadFromFile("audio/eat.wav")) {
        std::cout << "Error loading death sound file!" << std::endl;
    } else {
        pellet_sound.setBuffer(pellet_buffer);

    }
}

bool Pacman::get_animation_over()
{
    return animation_over;
}

bool Pacman::get_dead()
{
    return dead;
}

unsigned char Pacman::get_direction()
{
    return direction;
}

unsigned short Pacman::get_energizer_timer()
{
    return energizer_timer;
}

void Pacman::draw(bool i_victory, sf::RenderWindow& i_window)
{
    unsigned char frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));

    sf::Sprite sprite;
    sf::Texture texture;

    sprite.setPosition(position.x, position.y);

    if (1 == dead || 1 == i_victory)
    {
        if (animation_timer < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
        {
            animation_timer++;
            texture.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");
            sprite.setTexture(texture);
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));
            i_window.draw(sprite);
        }
        else
        {
            // Solo puedes morir una vez
            animation_over = 1;
        }
    }
    else
    {
        texture.loadFromFile("Resources/Images/Pacman" + std::to_string(CELL_SIZE) + ".png");
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));
        i_window.draw(sprite);
        animation_timer = (1 + animation_timer) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
    }
}

void Pacman::reset()
{
    animation_over = 0;
    dead = 0;
    direction = 0;
    animation_timer = 0;
    energizer_timer = 0;
    position = posicion_inicial;  // Restablecer a la posición inicial
}

void Pacman::set_animation_timer(unsigned short i_animation_timer)
{
    animation_timer = i_animation_timer;
}

void Pacman::set_dead(bool i_dead) {
    dead = i_dead;

    if (dead) {
        animation_timer = 0;
        death_sound.play();
        if (death_sound.getStatus() != sf::Sound::Playing) {
            std::cerr << "Error reproduciendo el sonido de muerte" << std::endl;
        }
    }
}

void Pacman::set_position(short i_x, short i_y)
{
    position = {i_x, i_y};
}

void Pacman::update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
    std::array<bool, 4> walls{};
    walls[0] = map_collision_pacman(0, 0, PACMAN_SPEED + position.x, position.y, i_map, puntuacion);
    walls[1] = map_collision_pacman(0, 0, position.x, position.y - PACMAN_SPEED, i_map, puntuacion);
    walls[2] = map_collision_pacman(0, 0, position.x - PACMAN_SPEED, position.y, i_map, puntuacion);
    walls[3] = map_collision_pacman(0, 0, position.x, PACMAN_SPEED + position.y, i_map, puntuacion);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && 0 == walls[0])
    {
        direction = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && 0 == walls[1])
    {
        direction = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && 0 == walls[2])
    {
        direction = 2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && 0 == walls[3])
    {
        direction = 3;
    }

    if (0 == walls[direction])
    {
        switch (direction)
        {
            case 0: position.x += PACMAN_SPEED; break;
            case 1: position.y -= PACMAN_SPEED; break;
            case 2: position.x -= PACMAN_SPEED; break;
            case 3: position.y += PACMAN_SPEED; break;
        }
    }

    if (-CELL_SIZE >= position.x)
    {
        position.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED;
    }
    else if (CELL_SIZE * MAP_WIDTH <= position.x)
    {
        position.x = PACMAN_SPEED - CELL_SIZE;
    }

    if (map_collision_pacman(1, 0, position.x, position.y, i_map, puntuacion))
    {
        energizer_timer = static_cast<unsigned short>(ENERGIZER_DURATION / pow(2, i_level));
        sound.play();
        if (sound.getStatus() != sf::Sound::Playing) {
            std::cerr << "Error reproduciendo el sonido" << std::endl;
        }
    }
    else
    {
        energizer_timer = std::max(0, energizer_timer - 1);
    }

    std::cout << "Puntuación actual: " << puntuacion << std::endl;  // Agrega esta línea para verificar la puntuación
}



Position Pacman::get_position()
{
    return position;
}

int Pacman::get_puntuacion()const
{
    return puntuacion;
}


void Pacman::set_puntuacion(int nuevaPuntuacion){
    puntuacion=nuevaPuntuacion;
}

void Pacman::set_vidas(int nuevas_vidas) {
    vidas = nuevas_vidas;
}

int Pacman::get_vidas() const {
    return vidas;
}

void Pacman::perder_vida() {
    if (vidas > 0) {
        vidas--;
        if (vidas == 0) {
            puntuacion = 0; // Reiniciar la puntuación a 0 cuando se pierden todas las vidas
        }
    }
}

void Pacman::reiniciar_vidas() {
    vidas = 3;
}

void Pacman::resetPosition() {
    position = posicion_inicial;  // Restablece la posición a la posición inicial
}

Position Pacman::getInitialPosition() const {
    return posicion_inicial;
}
