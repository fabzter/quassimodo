#include <stdlib.h>
#include <Reglas/Tablero.hpp>
#include <Reglas/Celda.hpp>
#include <Reglas/Barrera.hpp>
#include <Reglas/Jugador.hpp>
#include <Reglas/Juez.hpp>
#include <Reglas/Partida.hpp>
#include <Scripting/Manejador.hpp>
#include <vector>

#include "Player.hpp"

#ifdef DEBUG
void print_vector(std::vector<int> v)
{
    for(int i = 0; i < v.size(); i++)
        std::cout << v.at(i) << ' ';
    std::cout << std::endl;
}
#endif
using namespace Reglas;
using namespace std;

int main(int argc, char** argv)
{
    Tablero t;
    Scripting::Manejador *m = new Scripting::Manejador();
    std::vector<Agente*> agentes;

    agentes.push_back(m->getAgente("../bin/agente.py"));
    agentes.push_back(m->getAgente("../bin/agente.py"));

    std::vector< Jugador* > jugadores;
    jugadores.push_back(new Jugador(0, agentes[0]));
    jugadores.push_back(new Jugador(1, agentes[1]));

    t.setJugadores(jugadores);

    Partida *p = new Partida(&t);

    try{
    p->iniciarPartida();
    }
    catch (boost::python::error_already_set& e)
        {
            PyObject *type, *value, *traceback;
            // Save the error state because PyErr_Print() is going toclear
            // it. That's not what we want.
            PyErr_Fetch(&type, &value, &traceback);
            // But whoops, PyErr_Fetch() just cleared the exceptionflag! If
            // we now call PyErr_Print(), it thinks there's nothingwrong, and
            // doesn't print anything! Immediately restore the exceptionso
            // PyErr_Print() will see it.
            PyErr_Restore(type, value, traceback);
            // Okay, print the traceback to stderr...
            PyErr_Print();
            // then restore (again!) the original exception state.
            PyErr_Restore(type, value, traceback);
        }

    while(p->estaEnCurso())
    {
        cout << t << endl;
        try
        {
            p->siguienteJugada();
        }
        catch(boost::python::error_already_set& e)
        {
            PyObject *type, *value, *traceback;
            // Save the error state because PyErr_Print() is going toclear
            // it. That's not what we want.
            PyErr_Fetch(&type, &value, &traceback);
            // But whoops, PyErr_Fetch() just cleared the exceptionflag! If
            // we now call PyErr_Print(), it thinks there's nothingwrong, and
            // doesn't print anything! Immediately restore the exceptionso
            // PyErr_Print() will see it.
            PyErr_Restore(type, value, traceback);
            // Okay, print the traceback to stderr...
            PyErr_Print();
            // then restore (again!) the original exception state.
            PyErr_Restore(type, value, traceback);
        }
        cin.get();
    }

    return (EXIT_SUCCESS);
}

