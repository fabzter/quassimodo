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

void probarConversionesDeJugada();
using namespace Reglas;
using namespace std;

int main(int argc, char** argv)
{
    probarConversionesDeJugada();
    Tablero t;
    Scripting::Manejador *m = new Scripting::Manejador(t);
    std::vector<Agente*> agentes;

    agentes.push_back(m->getAgente("../bin/agenteCamina2.py"));
    //agentes.push_back(m->getAgente("../bin/agenteCamina2.py"));
    //agentes.push_back(m->getAgente("../bin/agenteCamina.py"));
    agentes.push_back(m->getAgente("../bin/agenteBarreras.py"));
    //agentes.push_back(m->getAgente("../bin/agenteBarreras2.py"));

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

    b1.colocar(0, 1, ESTE);
    b2.colocar(2, 1, ESTE);
    b3.colocar(4, 1, ESTE);
    b4.colocar(7, 4, ESTE);
    t.setBarrera(0, b1);
    t.setBarrera(0, b2);
    t.setBarrera(0, b3);
    //t.setBarrera(0, b4);

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

void probarConversionesDeJugada()
{
    cout << "Pruebas de Conversión de Jugadas" << endl;
    Barrera b;
    b.colocar(1, 5, ESTE);

    cout << "Conversion de Barrera a Jugada: ";
    Jugada j(b);
    if(j.getPosicion()[0] == b.getPosicion()[0] &&
       j.getPosicion()[1] == b.getPosicion()[1] &&
       j.getDireccion() == b.getDireccion())
    {
        cout << "Funciona." << endl;
    }
    else
    {
        cout << "No funca" << endl;
    }

    cout << "Conversion de Celda a Jugada: ";
    Celda c;
    Jugada j1(c);
    if(j1.getPosicion()[0] == c.getPosicion()[0] &&
       j1.getPosicion()[1] == c.getPosicion()[1])
    {
        cout << "Funciona." << endl;
    }
    else
    {
        cout << "No funca" << endl;
    }

    cout << "Conversion de Jugada a Barrera: ";
    Barrera b1(j);
    if(j.getPosicion()[0] == b1.getPosicion()[0] &&
       j.getPosicion()[1] == b1.getPosicion()[1] &&
       j.getDireccion() == b1.getDireccion())
    {
        cout << "Funciona." << endl;
    }
    else
    {
        cout << "No funca" << endl;
    }

    cout << "Conversion de Jugada a Celda: ";
    Celda c1(j1);
    if(j1.getPosicion()[0] == c1.getPosicion()[0] &&
       j1.getPosicion()[1] == c1.getPosicion()[1])
    {
        cout << "Funciona." << endl;
    }
    else
    {
        cout << "No funca" << endl;
    }
}
