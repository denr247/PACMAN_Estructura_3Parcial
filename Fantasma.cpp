#include <array>
#include <cmath>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#include "Global.hpp"
#include "Pacman.hpp"
#include "Fantasma.hpp"
#include "ColisionMapa.hpp"

Fantasma::Fantasma(unsigned char i_id) : id(i_id){}

bool Fantasma::pacman_colision(const Posicion& i_pacman_posicion){
    //ALGORITMO AVANZADO DE COMPROBACION DE COLISIONES.
    if (posicion.x > i_pacman_posicion.x - TAM_CELDA && posicion.x < TAM_CELDA + i_pacman_posicion.x){
        if (posicion.y > i_pacman_posicion.y - TAM_CELDA && posicion.y < TAM_CELDA + i_pacman_posicion.y){
            return 1;
        }
    }
    return 0;
}

float Fantasma::obtener_distancia_objetivo(unsigned char i_direccion){
    short x = posicion.x;
	short y = posicion.y;

	//Imaginaremos filmar al fantasma en una dirección dada y calcularemos la distancia al objetivo.
	switch(i_direccion)
	{
        case 0:
		{
			x += VELOCIDAD_FANTASMA;

			break;
		}
		case 1:
		{
			y -= VELOCIDAD_FANTASMA;

			break;
		}
		case 2:
		{
			x -= VELOCIDAD_FANTASMA;

			break;
		}
		case 3:
		{
			y += VELOCIDAD_FANTASMA;
		}
	}
	return static_cast<float>(sqrt(pow(x - objetivo.x, 2) + pow(y - objetivo.y, 2)));
}

void Fantasma::dibujar(bool i_destello, sf::RenderWindow& i_window)
{
    // Fotograma actual de la animación.
    unsigned char fotograma_cuerpo = static_cast<unsigned char>(floor(temporizador_animacion / static_cast<float>(VELOCIDAD_ANIMACION_FANTASMA)));

    sf::Sprite cuerpo;
    sf::Sprite cara;

    sf::Texture textura;
    //textura.loadFromFile("Resources/Images/Fantasma" + std::to_string(CELL_SIZE) + ".png");

    cuerpo.setTexture(textura);
    cuerpo.setPosition(posicion.x, posicion.y);
    // La animación es básicamente una visualización rápida de imágenes similares.
    // Eso es lo que estamos haciendo aquí.
    cuerpo.setTextureRect(sf::IntRect(TAM_CELDA * fotograma_cuerpo, 0, TAM_CELDA, TAM_CELDA));

    cara.setTexture(textura);
    cara.setPosition(posicion.x, posicion.y);

    // La apariencia de "No tengo miedo".
    if (0 == modo_asustado)
    {
        switch (id)
        {
            case 0:
            {
                // Color rojo
                cuerpo.setColor(sf::Color(255, 0, 0));

                break;
            }
            case 1:
            {
                // Color rosa
                cuerpo.setColor(sf::Color(255, 182, 255));

                break;
            }
            case 2:
            {
                // Color cian (todavía no entiendo por qué lo llamaron azul)
                cuerpo.setColor(sf::Color(0, 255, 255));

                break;
            }
            case 3:
            {
                // Color naranja
                cuerpo.setColor(sf::Color(255, 182, 85));
            }
        }

        cara.setTextureRect(sf::IntRect(TAM_CELDA * direccion, TAM_CELDA, TAM_CELDA, TAM_CELDA));

        i_window.draw(cuerpo);
    }
    // La apariencia de "Bueno, quizá sí tengo miedo".
    else if (1 == modo_asustado)
    {
        cuerpo.setColor(sf::Color(36, 36, 255));
        // La cara permanece igual independientemente de hacia dónde se dirige el fantasma en este momento.
        cara.setTextureRect(sf::IntRect(4 * TAM_CELDA, TAM_CELDA, TAM_CELDA, TAM_CELDA));

        if (1 == i_destello && 0 == fotograma_cuerpo % 2)
        {
            cuerpo.setColor(sf::Color(255, 255, 255));
            cara.setColor(sf::Color(255, 0, 0));
        }
        else
        {
            cuerpo.setColor(sf::Color(36, 36, 255));
            cara.setColor(sf::Color(255, 255, 255));
        }

        i_window.draw(cuerpo);
    }
    // La apariencia de "¡¡¡AAAAH!!! ¡ME VOY A MI CASA!".
    else
    {
        // Solo dibujamos la cara porque Pacman se robó el cuerpo.
        cara.setTextureRect(sf::IntRect(TAM_CELDA * direccion, 2 * TAM_CELDA, TAM_CELDA, TAM_CELDA));
    }

    i_window.draw(cara);

    //--------------------------------------<        Esto es para evitar desbordamientos.         >-
    temporizador_animacion = (1 + temporizador_animacion) % (FANTASMA_FRAMES_ANIMACION * VELOCIDAD_ANIMACION_FANTASMA);
}

void Fantasma::reset(const Posicion& i_inicio, const Posicion& i_salida_inicio)
{
    modo_movimiento = 0;
    // Todos pueden usar la puerta excepto el fantasma rojo.
    // Porque odio al fantasma rojo.
    usar_puerta = 0 < id;

    direccion = 0;
    modo_asustado = 0;
    temporizador_velocidad_asustado = 0;

    temporizador_animacion = 0;

    inicio = i_inicio;
    salida_inicio = i_salida_inicio;
    objetivo = i_salida_inicio;
}

void Fantasma::set_posicion(short i_x, short i_y)
{
    posicion = {i_x, i_y};
}

void Fantasma::cambiar_modo()
{
    modo_movimiento = 1 - modo_movimiento;
}

void Fantasma::actualizar(unsigned char i_nivel, array<array<Celda, ANCHO_MAPA>, ALTURA_MAPA>& i_mapa, Fantasma& i_fantasma_0, Pacman& i_pacman)
{
    // ¿Puede el fantasma moverse?
    bool mover = false;

    // Si es mayor que 1, significa que el fantasma ha llegado a la intersección.
    // No consideramos el regreso como una dirección disponible.
    unsigned char direcciones_disponibles = 0;
    unsigned char velocidad = VELOCIDAD_FANTASMA;

    array<bool, 4> paredes{};

    // Aquí el fantasma empieza y deja de estar asustado.
    if (modo_asustado == 0 && i_pacman.get_temporizador_energizador() == DURACION_ENERGIZADOR / pow(2, i_nivel))
    {
        temporizador_velocidad_asustado = VELOCIDAD_ASUSTADO_FANTASMA;
        modo_asustado = 1;
    }
    else if (i_pacman.get_temporizador_energizador() == 0 && modo_asustado == 1)
    {
        modo_asustado = 0;
    }

    // Usamos el operador módulo en caso de que el fantasma salga del mapa.
    if (2 == modo_asustado && 0 == posicion.x % VELOCIDAD_HUIDA_FANTASMA && 0 == posicion.y % VELOCIDAD_HUIDA_FANTASMA)
    {
        velocidad = VELOCIDAD_HUIDA_FANTASMA;
    }

    void actualizar_objetivo(unsigned char i_direccion_pacman, const Posicion& i_posicion_fantasma_0, const Posicion& i_posicion_pacman);


    // Verificación de colisiones con las paredes
    paredes[0] = colision_mapa(0, usar_puerta, velocidad + posicion.x, posicion.y, i_mapa);
    paredes[1] = colision_mapa(0, usar_puerta, posicion.x, posicion.y - velocidad, i_mapa);
    paredes[2] = colision_mapa(0, usar_puerta, posicion.x - velocidad, posicion.y, i_mapa);
    paredes[3] = colision_mapa(0, usar_puerta, posicion.x, velocidad + posicion.y, i_mapa);

    if (1 != modo_asustado)
    {
        // Usamos 4 porque usar un número entre 0 y 3 hará que el fantasma se mueva en una dirección que no puede.
        unsigned char direccion_optima = 4;

        // El fantasma puede moverse.
        mover = true;

        for (unsigned char a = 0; a < 4; a++)
        {
            // Los fantasmas no pueden dar marcha atrás (a menos que realmente tengan que hacerlo)
            if (a == (2 + direccion) % 4)
            {
                continue;
            }
            else if (0 == paredes[a])
            {
                if (4 == direccion_optima)
                {
                    direccion_optima = a;
                }

                direcciones_disponibles++;

                if (obtener_distancia_objetivo(a) < obtener_distancia_objetivo(direccion_optima))
                {
                    // La dirección óptima es la que está más cerca del objetivo.
                    direccion_optima = a;
                }
            }
        }

        if (1 < direcciones_disponibles)
        {
            // Cuando el fantasma está en la intersección, elige la dirección óptima.
            direccion = direccion_optima;
        }
        else
        {
            if (4 == direccion_optima)
            {
                // La parte de "a menos que sea necesario".
                direccion = (2 + direccion) % 4;
            }
            else
            {
                direccion = direccion_optima;
            }
        }
    }
    else
    {
        // Usamos rand() porque consideramos que estamos usando aleatoriedad aquí, y no es necesario usar toda una biblioteca para ello.
        unsigned char direccion_aleatoria = rand() % 4;

        if (0 == temporizador_velocidad_asustado)
        {
            // El fantasma puede moverse después de un cierto número de fotogramas.
            mover = true;
            temporizador_velocidad_asustado = VELOCIDAD_ASUSTADO_FANTASMA;

            for (unsigned char a = 0; a < 4; a++)
            {
                // No pueden dar marcha atrás incluso si están asustados.
                if (a == (2 + direccion) % 4)
                {
                    continue;
                }
                else if (0 == paredes[a])
                {
                    direcciones_disponibles++;
                }
            }

            if (0 < direcciones_disponibles)
            {
                while (1 == paredes[direccion_aleatoria] || direccion_aleatoria == (2 + direccion) % 4)
                {
                    // Seguimos eligiendo una dirección aleatoria hasta que podamos usarla.
                    direccion_aleatoria = rand() % 4;
                }

                direccion = direccion_aleatoria;
            }
            else
            {
                // Si no hay otro camino, da marcha atrás.
                direccion = (2 + direccion) % 4;
            }
        }
        else
        {
            temporizador_velocidad_asustado--;
        }
    }

    // Si el fantasma puede moverse, lo movemos.
    if (mover)
    {
        switch (direccion)
        {
            case 0: // derecha
                posicion.x += velocidad;
                break;
            case 1: // arriba
                posicion.y -= velocidad;
                break;
            case 2: // izquierda
                posicion.x -= velocidad;
                break;
            case 3: // abajo
                posicion.y += velocidad;
                break;
        }

        // Warp (teletransportación).
        // Cuando el fantasma sale del mapa, lo movemos al otro lado.
        if (-TAM_CELDA >= posicion.x)
        {
            posicion.x = TAM_CELDA * ANCHO_MAPA - velocidad;
        }
        else if (posicion.x >= TAM_CELDA * ANCHO_MAPA)
        {
            posicion.x = velocidad - TAM_CELDA;
        }
    }

    if (pacman_colision(i_pacman.get_posicion()))
    {
        if (0 == modo_asustado) // Cuando el fantasma no está asustado y colisiona con Pacman, matamos a Pacman.
        {
            i_pacman.establecer_muerto(true);
        }
        else // De lo contrario, el fantasma empieza a huir a su casa.
        {
            usar_puerta = true;
            modo_asustado = 2;
            objetivo = inicio;
        }
    }
}


void Fantasma::actualizar_objetivo(unsigned char i_direccion_pacman, const Posicion& i_posicion_fantasma_0, const Posicion& i_posicion_pacman)
{
    if (1 == usar_puerta) // Si el fantasma puede usar la puerta.
    {
        if (posicion == objetivo)
        {
            if (salida_inicio == objetivo) // Si el fantasma ha llegado a la salida.
            {
                usar_puerta = 0; // Ya no puede usar la puerta.
            }
            else if (inicio == objetivo) // Si el fantasma ha llegado a su casa.
            {
                modo_asustado = 0; // Deja de estar asustado.
                objetivo = salida_inicio; // Y empieza a salir de la casa.
            }
        }
    }
    else
    {
        if (0 == modo_movimiento) // El modo disperso
        {
            // Cada fantasma va a la esquina que se le asigna.
            switch (id)
            {
                case 0:
                {
                    objetivo = {TAM_CELDA * (ANCHO_MAPA - 1), 0};
                    break;
                }
                case 1:
                {
                    objetivo = {0, 0};
                    break;
                }
                case 2:
                {
                    objetivo = {TAM_CELDA * (ANCHO_MAPA - 1), TAM_CELDA * (ALTURA_MAPA - 1)};
                    break;
                }
                case 3:
                {
                    objetivo = {0, TAM_CELDA * (ALTURA_MAPA - 1)};
                }
            }
        }
        else // El modo persecución
        {
            switch (id)
            {
                case 0: // El fantasma rojo perseguirá a Pacman.
                {
                    objetivo = i_posicion_pacman;
                    break;
                }
                case 1: // El fantasma rosa perseguirá la 4ª celda delante de Pacman.
                {
                    objetivo = i_posicion_pacman;

                    switch (i_direccion_pacman)
                    {
                        case 0: // derecha
                        {
                            objetivo.x += TAM_CELDA * FANTASMA_1_PERSEGUIR;
                            break;
                        }
                        case 1: // arriba
                        {
                            objetivo.y -= TAM_CELDA * FANTASMA_1_PERSEGUIR;
                            break;
                        }
                        case 2: // izquierda
                        {
                            objetivo.x -= TAM_CELDA * FANTASMA_1_PERSEGUIR;
                            break;
                        }
                        case 3: // abajo
                        {
                            objetivo.y += TAM_CELDA * FANTASMA_1_PERSEGUIR;
                        }
                    }
                    break;
                }
                case 2: // El fantasma azul.
                {
                    objetivo = i_posicion_pacman;

                    // Obtener la segunda celda delante de Pacman.
                    switch (i_direccion_pacman)
                    {
                        case 0: // derecha
                        {
                            objetivo.x += TAM_CELDA * FANTASMA_2_PERSEGUIR;
                            break;
                        }
                        case 1: // arriba
                        {
                            objetivo.y -= TAM_CELDA * FANTASMA_2_PERSEGUIR;
                            break;
                        }
                        case 2: // izquierda
                        {
                            objetivo.x -= TAM_CELDA * FANTASMA_2_PERSEGUIR;
                            break;
                        }
                        case 3: // abajo
                        {
                            objetivo.y += TAM_CELDA * FANTASMA_2_PERSEGUIR;
                        }
                    }

                    // Enviamos un vector del fantasma rojo a la segunda celda delante de Pacman.
                    // Luego lo duplicamos.
                    objetivo.x += objetivo.x - i_posicion_fantasma_0.x;
                    objetivo.y += objetivo.y - i_posicion_fantasma_0.y;

                    break;
                }
                case 3: // El fantasma naranja perseguirá a Pacman hasta que se acerque a él. Luego cambiará al modo disperso.
                {
                    // Usando el teorema de Pitágoras nuevamente.
                    if (TAM_CELDA * FANTASMA_3_PERSEGUIR <= sqrt(pow(posicion.x - i_posicion_pacman.x, 2) + pow(posicion.y - i_posicion_pacman.y, 2)))
                    {
                        objetivo = i_posicion_pacman;
                    }
                    else
                    {
                        objetivo = {0, TAM_CELDA * (ALTURA_MAPA - 1)};
                    }
                }
            }
        }
    }
}

Posicion Fantasma::get_posicion()
{
    return posicion;
}


