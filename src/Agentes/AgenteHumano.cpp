
#include "AgenteHumano.hpp"

Agentes::AgenteHumano::AgenteHumano()
{
}

Agentes::AgenteHumano::AgenteHumano(const AgenteHumano& orig)
{
}

Agentes::AgenteHumano::~AgenteHumano()
{
}

void Agentes::AgenteHumano::iniciar(int id)
{
    this->id = id;
}

Reglas::Jugada Agentes::AgenteHumano::siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante)
{
    return this->jugada;
}

void Agentes::AgenteHumano::terminar()
{

}
void Agentes::AgenteHumano::SetJugada( Reglas::Jugada &j){

    this->jugada=j;
}

Reglas::Agente* Agentes::FabricaHumano::operator ()()
{
    return new AgenteHumano();
}