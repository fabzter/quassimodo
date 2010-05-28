#ifndef _SKIN_HPP
#define	_SKIN_HPP
#include"Exepciones.hpp"
#include<irrlicht/irrlicht.h>
//TODO: Documentar aqui!!!!
using namespace irr;
namespace Grafico{
class Skin {
public:
    Skin(scene::ISceneManager* smgr,gui::IGUIEnvironment* env);
    Skin(const Skin& orig);
    virtual ~Skin();
    scene::IAnimatedMesh* getCelda();
    scene::IAnimatedMesh* getTablero();
    scene::IAnimatedMesh* getAntorcha();
    scene::IAnimatedMesh* getJugador1();
    scene::IAnimatedMesh* getJugador2();
    scene::IAnimatedMesh* getBarrera();
    video::ITexture* getTTAblero();
    video::ITexture* getTCelda();
    video::ITexture* getTAntorcha();
    video::ITexture* getTBarrera();
    gui::IGUIFont* getMenuBoton();
    gui::IGUIFont* getDefault();
    gui::IGUIFont* getMenuToolTip();
    gui::IGUIFont* getGUIBoton();
    gui::IGUIFont* getGUIWindow();
    io::path getHTerrain();
    video::ITexture* getTTerrain();
    video::ITexture* getTSkydome();
private:
    void setTablero(scene::ISceneManager* smgr);
    void setJugadores(scene::ISceneManager* smgr);
    void setAntorcha(scene::ISceneManager* smgr);
    void setBarrera(scene::ISceneManager* smgr);
    void setCelda(scene::ISceneManager* smgr);
    void setMenuBoton(gui::IGUIEnvironment* env);
    void setDefault(gui::IGUIEnvironment* env);
    void setMenuToolTip(gui::IGUIEnvironment* env);
    void setGUIBoton(gui::IGUIEnvironment* env);
    void setGUIWindow(gui::IGUIEnvironment* env);
    void setTerrain(scene::ISceneManager* smgr);
    void setSkyDome(scene::ISceneManager* smgr);
     scene::IAnimatedMesh* Tablero;
     scene::IAnimatedMesh* Celda;
     scene::IAnimatedMesh* Antorcha;
     scene::IAnimatedMesh* Jugador1;
     scene::IAnimatedMesh* Jugador2;
     scene::IAnimatedMesh* Barrera;
     video::ITexture* TBarrera;
     video::ITexture* TCelda;
     video::ITexture* TTAblero;
     video::ITexture* TAntorcha;
     gui::IGUIFont*  MenuBoton;
     gui::IGUIFont* Default;
     gui::IGUIFont* MenuToolTip;
     gui::IGUIFont* GUIBoton;
     gui::IGUIFont* GUIWindow;
     io::path heightMTerrain;
     video::ITexture* TTerrain;
     video::ITexture* Tskydome;

};
}
#endif	/* _SKIN_HPP */

