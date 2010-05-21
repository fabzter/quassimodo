#include <stdlib.h>
#include <Reglas/Tablero.hpp>
#include <Reglas/Celda.hpp>
#include <Reglas/Barrera.hpp>
#include <Reglas/Jugador.hpp>
#include <Reglas/Juez.hpp>
#include "Partida.hpp"
#include <Scripting/Manejador.hpp>
#include <Scripting/Excepciones.hpp>
#include <vector>

using namespace Reglas;
using namespace std;

int main(int argc, char** argv)
{
    Tablero t;
    Scripting::Manejador *m = new Scripting::Manejador(t);
    std::vector<Agente*> agentes;

    try
    {
        agentes.push_back(m->getAgente("../../bin/agenteBusca.py"));
        agentes.push_back(m->getAgente("../../bin/agenteBarreras2.py"));
    }
    catch(Scripting::ScriptMalo &e)
    {
        agentes.push_back(m->getAgente("../bin/agenteBusca.py"));
        agentes.push_back(m->getAgente("../bin/agenteBarreras2.py"));
    }

    std::vector< Jugador* > jugadores;
    jugadores.push_back(new Jugador(0, agentes[0]));
    jugadores.push_back(new Jugador(1, agentes[1]));

    t.setJugadores(jugadores);

    Partida *p = new Partida(&t);

    cout << t << endl;
    try
    {
        p->iniciarPartida();
    }
    catch (Scripting::ScriptMalo &e)
    {
        cout << "Error en un Script: \n" << e.what() << endl;
        throw;
    }

    while(p->estaEnCurso())
    {
        try
        {
            p->siguienteJugada();
        }
        catch(Scripting::ScriptMalo &e)
        {
           cout << "Error en un Script: \n" << e.what() << endl;
           throw;
        }
        catch(ReglasRotas &e)
        {
           cout << "Se rompieron la reglas!: \n" << e.what() << endl;
        }
        cout << t << endl;
        cin.get();
    }
    if(p->hayGanador())
    {
        cout << "Hay un ganador!" << endl;
    }

    return (EXIT_SUCCESS);
}
