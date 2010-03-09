#include <stdlib.h>
#include <Reglas/Tablero.hpp>
#include <Reglas/Celda.hpp>
#include <Reglas/Barrera.hpp>
#include <Reglas/Jugador.hpp>
#include <Reglas/Juez.hpp>
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
    Celda c1;

    try{
        c1.colocar(9, 9);
    } catch(PiezaFueraDelTablero& e){
        c1.colocar(8, 8);
    }

    try{
        c1.colocar(1, 1);
    }
    catch(PiezaYaColocada & e){

    }

    Celda c2(c1);

    if(c2 == c1)
        cout << c2 << endl;

    Tablero t;

    Barrera b;

    vector<int> pos;
    pos.push_back(0);
    pos.push_back(1);
    b.colocar(pos, ESTE);

    std::vector<Jugador*> jugadores;
    for(int i = 0; i < Tablero::num_jugadores; i++)
        jugadores.push_back(new Jugador(i, new Player()));

    t.setJugadores(jugadores);
    
    cout << t << endl;
    for(int i = 0; i < 2; i++)
    {
        print_vector(t.getJugador(i).getPosicion());
        cout << t.getJugador(i).getBarrerasDisponibles() << endl;
    }
    cout << endl;
    cout << endl;

    t.setBarrera(0, b);
    cout << t << '\n' << t.getJugador(0).getBarrerasDisponibles() << " " <<
            t.getJugador(1).getBarrerasDisponibles() << endl;

    t.moverJugador(0, Tablero::size_x/2, t.getJugador(0).getPosicion().at(1) + 1);

    cout << t << endl;

    t.moverJugador(0, 1, 0);
    t.moverJugador(1, 2, 0);

    for(int i = 0; i < 2; i++)
        print_vector(t.getJugador(i).getPosicion());
    cout << endl;
    cout << endl;
    pos.at(0) = 3;
    pos.at(1) = 0;

    Barrera b1;
    b1.colocar(pos, NORTE);
    t.setBarrera(1, b1);

    cout << t << '\n' << t.getJugador(0).getBarrerasDisponibles() << " " <<
            t.getJugador(1).getBarrerasDisponibles() << endl;
    print_vector(jugadores.at(0)->getPosicion());
    print_vector(jugadores.at(1)->getPosicion());

    Juez j (t);

    Jugada jugada;
    try{
        jugada = j.siguienteJugada(0);
        Barrera b2;
        b2.colocar(jugada.getPosicion(), jugada.getDireccion());

        t.setBarrera(0, b2);
    } catch(ReglasRotas &e)
    {
        
    }

    

    cout << t << endl;
    cout << t.getJugador(0).getBarrerasDisponibles() << endl;

    Barrera b2;
    b2.colocar(pos, NORTE);
    b2 == b1? cout << "true" : cout << "false";
    cout << endl;

    pos[0] = 6;

    Barrera b3;
    b3.colocar(pos, NORTE);
    b2 == b3? cout << "true" : cout << "false";
    cout << endl;

    /*empieza la prueba de todo!! :D*/
    Scripting::Manejador m;
    //Agente* agente1 = m.getAgente("../bin/agente.py");
    Agente* agente2 = m.getAgente("../bin/agente.py");

    //agente2->iniciar(t, 0);
    
    return (EXIT_SUCCESS);
}

