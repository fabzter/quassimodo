

#ifndef _JUEGO_HPP
#define	_JUEGO_HPP
#include"ManejadorJuego.hpp"
#include"EventReceiver.hpp"
#include<irrlicht/irrlicht.h>
using namespace irr;
class Juego {
public:
    Juego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env);
    Juego(const Juego& orig);
    virtual ~Juego();
    EventReceiver* getEventReceiver();
private:
    ManejadorJuego* manj;
    EventReceiver *event;

};

#endif	/* _JUEGO_HPP */

