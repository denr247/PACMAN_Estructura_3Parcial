#pragma once

constexpr unsigned char TAM_CELDA = 16;
constexpr unsigned char ALTURA_FUENTE = 16;

/* contar todo desde 0, por lo que este es en realidad el segundo fantasma.
El sitio web usaba celdas más pequeñas, así que estoy configurando valores más pequeños.
Qué celda frente a Pacman apuntará el fantasma rosa.*/
constexpr unsigned char FANTASMA_1_PERSEGUIR = 2;
//Qué celda frente a Pacman usará el fantasma azul para calcular el objetivo.
constexpr unsigned char FANTASMA_2_PERSEGUIR = 1;
//Si la distancia entre el fantasma naranja y Pacman es menor que este valor, el fantasma naranja entrará en modo dispersión.
constexpr unsigned char FANTASMA_3_PERSEGUIR = 4;
//Cuántos fotogramas hay en la animación del cuerpo del fantasma.
constexpr unsigned char FANTASMA_FRAMES_ANIMACION = 6;
constexpr unsigned char VELOCIDAD_ANIMACION_FANTASMA = 4;
//La velocidad del fantasma después de que Pacman lo toca mientras está energizado, despues que come el poder amarillo
constexpr unsigned char VELOCIDAD_HUIDA_FANTASMA = 4;

//cuanto mayor sea el valor, más lento será el fantasma.
constexpr unsigned char VELOCIDAD_ASUSTADO_FANTASMA = 3; //se aplica cuando el pacman come su poder
constexpr unsigned char VELOCIDAD_FANTASMA = 1;
constexpr unsigned char ALTURA_MAPA = 21;
constexpr unsigned char ANCHO_MAPA = 21;
constexpr unsigned char FRAMES_ANIMACION_PACMAN = 6;
constexpr unsigned char VELOCIDAD_ANIMACION_PACMAN = 4;
constexpr unsigned char FRAMES_MUERTE_PACMAN = 12;
constexpr unsigned char VELOCIDAD_PACMAN = 2;
constexpr unsigned char REDIMENSIONAR_PANTALLA = 2;
constexpr unsigned short DURACION_PERSEGUIR = 1024;
constexpr unsigned short DURACION_ENERGIZADOR = 512;
constexpr unsigned short DURACION_FRAME = 16667;
constexpr unsigned short INICIO_DESTELLO_FANTASMA = 64;
constexpr unsigned short DURACION_DISPERSION_LARGA = 512;
constexpr unsigned short DURACION_DISPERSION_CORTA = 256;


/*
    Constantes para crear el mapa 
*/
enum Celda
{
	Puerta,
	Vacío,
	Energizador,
	Pastilla,
	Pared
};

/*
    Dimensiones para el mapa
*/
struct Posicion
{
	short x;
	short y;
	bool operator==(const Posicion& i_posicion)
	{
		return this->x == i_posicion.x && this->y == i_posicion.y;
	}
};
