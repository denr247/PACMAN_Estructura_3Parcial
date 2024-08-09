#pragma once
using namespace std;

array<sarray<Cell, MAP_HEIGHT>, MAP_WIDTH> convertir_boceto(const array<string, MAP_HEIGHT>& i_mapa_boceto, array<Posicion, 4>& i_fantasma_posicion, Pacman& i_pacman);
