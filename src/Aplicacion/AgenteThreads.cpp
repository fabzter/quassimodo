#include "AgenteThreads.hpp"

AgenteThreads::AgenteThreads(Reglas::Agente *agente)
{
    this->agente = agente;
    this->esta_thread_corriendo = this->esta_lista_jugada = false;
    this->jugada.setPosicion(-1, -1);
}

AgenteThreads::AgenteThreads(const AgenteThreads& orig)
{
}

AgenteThreads::~AgenteThreads()
{
}

void AgenteThreads::pedirJugada(const Reglas::Tablero &tablero,
                                            Reglas::AyudanteDeAgente &ayudante)
{
    this->jugada = this->agente->siguienteJugada(tablero, ayudante);
}

void AgenteThreads::run(const Reglas::Tablero &tablero,
                                            Reglas::AyudanteDeAgente &ayudante)
{
    if(this->esta_thread_corriendo)
        return;
    this->esta_thread_corriendo = true;
    
    this->thread = boost::thread(&AgenteThreads::pedirJugada, this, 
            tablero, ayudante );
}

bool AgenteThreads::estaListaJugada()
{
    if( this->jugada.getPosicion().at(0) != -1 )
    {
        this->esta_lista_jugada = true;
        this->esta_thread_corriendo = false;
        return true;
    }
    return false;
}

void AgenteThreads::iniciar(int id)
{
    this->agente->iniciar(id);
}

Reglas::Jugada AgenteThreads::siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante)
{
    Reglas::Jugada j = this->jugada;
    this->jugada.setPosicion(-1, -1);
    this->thread.join();
    return j;
}

void AgenteThreads::terminar()
{
    this->agente->terminar();
}