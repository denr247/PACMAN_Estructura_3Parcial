#pragma once

class Pacman
{
	bool animation_over;
	bool dead;
	unsigned char direction;
	unsigned short animation_timer;
	unsigned short energizer_timer;
	Position position;
	Position posicion;  // La posici�n actual de Pac-Man
    Position posicion_inicial;  // La posici�n inicial de Pac-Man
	int puntuacion;
	int vidas;

public:
	Pacman();
	bool get_animation_over();
	bool get_dead();
	unsigned char get_direction();
	unsigned short get_energizer_timer();
	void draw(bool i_victory, sf::RenderWindow& i_window);
	void reset();
	void set_animation_timer(unsigned short i_animation_timer);
	void set_dead(bool i_dead);
	void set_position(short i_x, short i_y);
	void update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);
    int get_puntuacion()const;
	void set_puntuacion(int);
	Position get_position();
	void set_vidas(int nuevas_vidas);
    int get_vidas() const;
    void perder_vida();
    void reiniciar_vidas();
    void resetPosition(); // M�todo para restablecer a la posici�n inicial
    Position getInitialPosition() const; // M�todo para obtener la posici�n inicial
};
