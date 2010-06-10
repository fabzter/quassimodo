

#ifndef _JUEGO_HPP
#define	_JUEGO_HPP
#include"ManejadorJuego.hpp"
#include<irrlicht/irrlicht.h>
//TODO: Documentar
using namespace irr;
class Juego {
public:
    Juego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env);
    Juego(const Juego& orig);
    virtual ~Juego();
    bool getSalir();
    void setSalir(bool valor);
   ManejadorJuego* getManJ();
private:
    bool salir;
    ManejadorJuego* manj;

};

#endif	/* _JUEGO_HPP */

