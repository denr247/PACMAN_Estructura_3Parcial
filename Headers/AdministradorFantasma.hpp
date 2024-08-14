#pragma once
unsigned namespace std;

class AdministradorFantasma {
    //LOS FANTTASMAS CAMBIARAN DE DIFERENTES MODOS ANTES DE PERSEGUIR AL PACMAN.
    // SEGUIMIENTO DE ONDAS
    unsigned char onda_actual;

    //MALDITA SEA, REALMENTE USE MUCHOS TEMPORIZADORES
    unsigned short temporizador_onda;

    array<Fantasma, 4> fantasmas;

public:

    AdministradorFantasma();

    void dibujar(bool i_destello, sf::RenderWindow& i_window);
    void reset(unsigned char i_nivel, const array<Posicion, 4>& i_fantasma_posicion);
    void actualizar(unsigned char i_nivel, array<array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_mapa, Pacman& i_pacman);
};
