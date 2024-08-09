#pragma once
using namespace std;

class Fantasma {

    //MODO PERSECUCION DEL FANTASMA
    bool modo_movimiento;
    //VARIABLE PARA USAR LA PUERTA
    bool usar_puerta;

    //DIRECCION ACTUAL
    unsigned char direccion;
    //0 - NO TENGO MIEDO
    //1 - BUENO, QUIZA SI
    //2 - ¡¡¡AAAAAAAH!!! ¡ME VOY A MI CASA!
    unsigned char modo_asustado;
    //TEMPORIZADOR PARA EL MOVIMIENTO DEL FANTASMA
    unsigned char temporizador_velocidad_asustado;
    //ROJO
    //ROSA
    //AZUL
    //NARANJA
    unsigned char id;

    unsigned short temporizador_animacion;

    //El FANTASMA VA AQUI CUADO ESCAPE
    Posicion inicio;
    //NO PUEDES QUEDARTE EN, TIENES QUE SALIR
    Posicion salida_inicio;
    //POSICION ACTUAL
    Posicion posicion;
    //OBJETIVO ACTUAL
    Posicion objetivo;

public:

    Fantasma(unsigned char i_id);

    bool pacman_colision(const Posicion& i_pacman_posicion);

    float obtener_distancia_objetivo(unsigned char i_direccion);

    void dibujar(bool i_flash, sf::RenderWindow& i_ventana);
    void reset(const Posicion& i_inicio, const Posicion& i_salida_inicio);
    void set_posicion(short i_x, short i_y);
    void cambiar_modo();
    void actualizar(unsigned char i_nivel, array<array<Celda, ALTURA_MAPA>, ANCHO_MAPA>& i_mapa, Fantasma& i_fantasma_0, Pacman& i_pacman);
    void actualizar_objetivo(unsigned char i_pacman_direccion, const Posicion& i_fantasma_0_posicion, const Posicion& i_pacman_posicion);

    Posicion get_posicion();
};
