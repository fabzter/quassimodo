

#ifndef THREADGRAFICO_HPP
#define	THREADGRAFICO_HPP
class Partida;

class ThreadGrafico {
public:
    ThreadGrafico(Partida* partida);
    ThreadGrafico(const ThreadGrafico& orig);
    virtual ~ThreadGrafico();
    void operator()();
    void stop();
private:
    Partida* partida;
    bool corriendo;

};

#endif	/* THREADGRAFICO_HPP */

