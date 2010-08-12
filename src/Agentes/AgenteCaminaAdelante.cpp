#include "AgenteCaminaAdelante.hpp"
#include <iostream>

Agentes::AgenteCaminaAdelante::AgenteCaminaAdelante()
{
}

Agentes::AgenteCaminaAdelante::AgenteCaminaAdelante(const AgenteCaminaAdelante& orig)
{
}

Agentes::AgenteCaminaAdelante::~AgenteCaminaAdelante()
{
}

void Agentes::AgenteCaminaAdelante::iniciar(int id)
{
    this->id = id;
    this->esta_iniciado = false;
}

Reglas::Jugada
Agentes::AgenteCaminaAdelante::siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante)
{
    if(!this->esta_iniciado)
    {
        const std::vector<Reglas::Celda>& metas = tablero.getMetas(this->id);
        if(metas.at(0).getPosicion().at(1) == 8)
        {
            this->direccion = Reglas::NORTE;
        }
        else if (metas.at(0).getPosicion().at(1) == 0)
        {
            this->direccion = Reglas::SUR;
        }
        else
        {
            std::cout << "WTF!" << std::endl;
            this->direccion = Reglas::SUR;
        }
        this->esta_iniciado = true;
    }

    Reglas::Jugada j;
    j.setTipoDeJugada(Reglas::MOVIMIENTO);

    const Reglas::Celda& celdaActual = tablero.getCelda(this->id);

    if(celdaActual.getHijo(this->direccion).estaLibre())
    {
        j.setPosicion(celdaActual.getHijo(this->direccion).getPosicion());
    }
    else
    {
        j.setPosicion(celdaActual.getHijo(this->direccion).getHijo(this->direccion).getPosicion());
    }

    return j;
}

void Agentes::AgenteCaminaAdelante::terminar()
{

}

Reglas::Agente* Agentes::FabricaCaminaAdelante::operator ()()
{
    return new AgenteCaminaAdelante();
}