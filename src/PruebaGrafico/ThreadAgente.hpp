

#ifndef THREAD_AGANTE_HPP
#define	THREAD_AGANTE_HPP
#include<Reglas/Juez.hpp>
#include<Reglas/Jugada.hpp>
#include<string>
class ThreadAgente {
public:
    ThreadAgente(Reglas::Juez* juez,int j_turno);
    ThreadAgente(const ThreadAgente& orig);
    virtual ~ThreadAgente();
    void operator()();
    bool enCurso();
    bool isExitoso();
    std::string getError();
    Reglas::Jugada &getJugada();

private:
    Reglas::Juez* juez;
    Reglas::Jugada j;
    std::string error;
    int jugador_turno;
    bool exitoso;
    bool en_curso;


};

#endif	/* ThreadAgente_HPP */

