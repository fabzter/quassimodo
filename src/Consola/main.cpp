
#include <stdlib.h>
#include <Reglas/Tablero.hpp>
#include <Reglas/Celda.hpp>
#include <Reglas/Barrera.hpp>
#include <Reglas/Jugador.hpp>
#include <Reglas/Juez.hpp>
#include <Reglas/AyudanteDeAgente.hpp>
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

    // Agent script paths come from argv (with sensible defaults), instead of the
    // original hardcoded relative paths. Usage: consola [agente1.py] [agente2.py]
    std::string ruta1 = (argc > 1) ? argv[1] : "bin/agenteCamina.py";
    std::string ruta2 = (argc > 2) ? argv[2] : "bin/agenteCamina2.py";
    try
    {
        agentes.push_back(m->getAgente(ruta1));
        agentes.push_back(m->getAgente(ruta2));
    }
    catch(Scripting::ScriptMalo &e)
    {
        cout << "Error cargando agente: " << e.what() << endl;
        return EXIT_FAILURE;
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
           break;
        }
        catch(ReglasRotas &e)
        {
           cout << "Se rompieron la reglas!: \n" << e.what() << endl;
           break;
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
