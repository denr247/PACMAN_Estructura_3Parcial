#pragma once

class Pacman
{
    bool animacion_pacmanMuere;
    // ¿Estoy muerto?
    bool esta_muerto;

    unsigned char direccion;

    // ¡Más temporizadores!
    unsigned short temporizador_animacion;
    unsigned short temporizador_energizador;

    // Ubicación actual de esta criatura, comúnmente conocida como Pacman.
    Posicion posicion;
public:
    Pacman();

    bool get_animacion_pacmanMuere();
    bool get_esta_muerto();

    unsigned char get_direccion();

    unsigned short get_temporizador_energizador();

    void dibujar(bool i_victoria, sf::RenderWindow& i_ventana);
    void reiniciar();
    void establecer_temporizador_animacion(unsigned short i_temporizador_animacion);
    void establecer_muerto(bool i_muerto);
    void establecer_posicion(short i_x, short i_y);
    void actualizar(unsigned char i_nivel, std::array<std::array<Celda, MAP_HEIGHT>, MAP_WIDTH>& i_mapa);

    Posicion obtener_posicion();
};
