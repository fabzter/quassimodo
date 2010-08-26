#ifndef AGENTETHREADS_HPP
#define	AGENTETHREADS_HPP
#include <Reglas/Agente.hpp>
#include <boost/thread.hpp>

class AgenteThreads: public Reglas::Agente
{
public:
    AgenteThreads(Reglas::Agente *agente);
    AgenteThreads(const AgenteThreads& orig);
    virtual ~AgenteThreads();

    virtual void iniciar(int id);
    virtual Reglas::Jugada siguienteJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante);
    virtual void terminar();

    void run(const Reglas::Tablero tablero, Reglas::AyudanteDeAgente &ayudante);

    bool estaListaJugada();
   
private:
    void pedirJugada(const Reglas::Tablero tablero,
                                            Reglas::AyudanteDeAgente &ayudante);
    Reglas::Agente* agente;
    bool esta_lista_jugada;
    bool esta_thread_corriendo;
    boost::thread thread;

    Reglas::Jugada jugada;
};

#endif	/* AGENTETHREADS_HPP */

