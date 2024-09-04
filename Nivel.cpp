#include "Headers/Nivel.hpp"

Nivel::Nivel() : nivelActual(0) {

    mapas[0]={
        //mapa letras espe
        "#################################################",
		"#o......................o......................#o#",
		"#.########.#.#######.#######.########.########...#",
		"#.########.#.#######.#.....#.########.########.#.#",
		"#.########.#.#######.#.#.#.#.########.########...#",
		"#.###......#.####....#.#.#.#.########..........#.#",
		"#.###o####.#.#######.#.#.#.#.########.###$####...#",
		"#o###......#.#######.#.###.#.########.###......#.#",
		"#.########.#.#######....0....########.########...##",
		"..########......####.###=###.########.########.#..",
		"#.########.#.#######.##123##.##.......########...##",
		"#.###......#.#######.#######.##.#####.###......#.#",
		"#.###.####.#.#######...#.#...##.#.....###o####...#",
		"#.###......#.#######.#.#.#.#.##.#.###..........#.#",
		"#.########.#.#######.#.#.#.#.##.#.#.#.########.#.#",
		"#.########...#######....P....##.#.#.#.########...#",
		"#o########.#.#######.#######.##.#.#.#.########.#.#",
		"#......................o.........................#",
		"#.########.#.#######.#######.#################.#.#",
		"#.......................o........................#",
		"##################################################"

	};
    mapas[1] = {
        //mapa nivel 2, hay que mejorar.
        "##################################################",
		"#.......................o........................#",
		"#.#.######.#.# #############.#.######.########.#.#",
		"#.#........#.# ......#.....#.#........########.#.#",
		"#.#.######.#.##.####.#.#.#.#.#.######.########...#",
		"#.###......#.##.#....#.#.#.#.## ## ##..........#.#",
		"#....o####.#.##.####.#.#.#.#.## ## ##.###.####...#",
		"#.###......#.........#.###.#.##    ##.###......#.#",
		"#.#...####.#.####.##....0....########.####.###...##",
		" .#.######......#.##.###=###....#####.####.....#...",
		"#.#.######.#.####.##.##123##.##.......####.###...##",
		"#.#.#......#.####.##.#######.##.#####..#.......#.#",
		"#.#...####.#...........#.#...##.#.....###o####...#",
		"#.###......#.##.####.#.#.#.#.##.#.###.###......#.#",
		"#.....####.#.##....#.#.#.#.#.##.#.#.#.########...#",
		"#.###.####...#####.#....P....##.#.#.#.########.#.#",
		"#.###.####.#.#####.#.#######.##.#.#.#.########...#",
		"#.........................#....................#.#",
		"#.########.#.#######.#######.#################.#.#",
		"#.......................o.......................#",
		"##################################################"

	};
    mapas[2]={
        " ################################################ ",
		" #.......................o......................# ",
		"##.########## #############.#.######.########.#.#",
		"  .#........#.# ......#.....#.#........########..#",
		"##.#.######.#.##.####.#.#.#.#.#.######.########.##",
		" #.#.#.#..#.#.##.#....#.#.#.#.## ## ##..........# ",
		" #....o#.##.#.##.####.#.#.#.#.## ## ##.###.####.# ",
		" #.###....#.#.........#.###.#.##    ##.###......# ",
		"##.#...####.#.####.##....0....########.####.###.### ",
		"  .#.#....#......#.##.###=###....#####.####.....  ",
		"##.#.######.#.####.##.##123##.##.......####.###.### ",
		" #.#.#......#.####.##.#######.##.#####.........# ",
		" ###...####.#...........#.#...##.#.....#.#o####.# ",
		" #.###......#.##.####.#.#.#.#.##.#.###.#........# ",
		" #.....#.##.#.##....#.#.#.#.#.##.#.#.#.#.#.####.# ",
		" #.###.#.##...#####.#....P....##.#.#.#.#.#.####.# ",
		" #.###.#.##.#.#####.#.#######.##.#.#.#.###..#.#.# ",
		" #.........................#....................# ",
		" #.########.#.#######.#######.#################.# ",
		" #.......................o......................# ",
		" ################################################ "
    };

}

// M�todo para obtener el mapa del nivel actual
std::array<std::string, MAP_HEIGHT> Nivel::getMapaActual() const {
    return mapas[nivelActual];
}

// M�todo para avanzar al siguiente nivel
void Nivel::avanzarNivel() {
    if (nivelActual < mapas.size() - 1) {
        nivelActual++;
    } else {
        // Si ya estamos en el �ltimo nivel, podr�amos reiniciar al primer nivel o mantener el �ltimo nivel.
         nivelActual = 0; // Reiniciar al primer nivel (opcional)
    }
}

// M�todo para reiniciar al primer nivel
void Nivel::reiniciarNivel() {
    nivelActual = 0;
}

// M�todo para obtener el n�mero del nivel actual
int Nivel::getNivelActual() const {
    return nivelActual + 1; // Devuelve el n�mero de nivel actual (1-indexed)
}
