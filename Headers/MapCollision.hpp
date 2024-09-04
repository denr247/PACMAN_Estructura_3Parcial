#pragma once

bool map_collision(bool i_collect_pellets, bool i_use_door, short i_x, short i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);
bool map_collision_pacman(bool i_collect_pellets, bool i_use_door, short i_x, short i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map,int &puntuacion);
