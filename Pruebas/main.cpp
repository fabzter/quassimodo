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
    Scripting::Manejador *m = new Scripting::Manejador(t);
    std::vector<Agente*> agentes;

    //agentes.push_back(m->getAgente("../bin/agenteCamina2.py"));
    agentes.push_back(m->getAgente("../bin/agenteCamina.py"));
    agentes.push_back(m->getAgente("../bin/agenteCamina.py"));
    //agentes.push_back(m->getAgente("../bin/agenteBarreras.py"));

    std::vector< Jugador* > jugadores;
    jugadores.push_back(new Jugador(0, agentes[0]));
    jugadores.push_back(new Jugador(1, agentes[1]));

    t.setJugadores(jugadores);

    Barrera b1;
    Barrera b2;
    Barrera b3;
    Barrera b4;
    Barrera b5;
    Barrera b6;
    Barrera b7;

    b1.colocar(1, 4, ESTE);
    b2.colocar(3, 4, ESTE);
    b3.colocar(5, 4, ESTE);
    b4.colocar(7, 4, ESTE);
    t.setBarrera(0, b1);
    t.setBarrera(0, b2);
    t.setBarrera(0, b3);
    t.setBarrera(0, b4);

    Partida *p = new Partida(&t);

    cout << t << endl;
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
        try
        {
            p->siguienteJugada();
        }
        catch(boost::python::error_already_set& e)
        {
            PyObject *type, *value, *traceback;
            // Save the error state because PyErr_Print() is going to clear
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
        cout << t << endl;
        cin.get();
    }

    return (EXIT_SUCCESS);
}

