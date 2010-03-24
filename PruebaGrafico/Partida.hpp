/* 
 * File:   Partida.hpp
 * Author: tokayo
 *
 * Created on 23 de marzo de 2010, 12:19
 */

#ifndef _PARTIDA_HPP
#define	_PARTIDA_HPP
#include<irrlicht/irrlicht.h>
#include<Grafico/Antorcha.hpp>
#include<Grafico/Tablero.hpp>
#include<Grafico/Barrera.hpp>
#include<Grafico/Celda.hpp>
#include<vector>
using namespace irr;

class Partida {
public:
    Partida(scene::ISceneManager* smgr);
    Partida(const Partida& orig);
    virtual ~Partida();
    void SetEscala(int x,int y,int z);
     void SetBarrera(int x,int y);
private:
    void ColocaAntorchas();
    Grafico::Tablero *t;
    std::vector<Grafico::Antorcha*> antorchas;
    Grafico::Barrera *b;
    Grafico::Celda *c;

};

#endif	/* _PARTIDA_HPP */

