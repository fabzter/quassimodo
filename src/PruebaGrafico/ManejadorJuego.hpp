
#ifndef _MANEJADORJUEGO_HPP
#define	_MANEJADORJUEGO_HPP

#include "ManejadorGUI.hpp"
#include "Partida.hpp"
#include "AnimacionEnd.hpp"
#include <Grafico/Skin.hpp>
#include<Reglas/Excepciones.hpp>
#include<Scripting/Excepciones.hpp>
#include<string.h>
#include <string>
//TODO: documentar!!
using namespace irr;
class ManejadorJuego {
public:
   ManejadorJuego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env);
    ManejadorJuego(const ManejadorJuego& orig);
    virtual ~ManejadorJuego();
    void setMenu();
    void setPartida();
    void setEscala(int x,int y,int z);
   bool SiguienteJugada();
    bool hayGanador();
    ManejadorGUI* getManejadorGUI();
    void setAgente(std::string Agente,int noAgente);
    void clearAgentes();
    void quick();
    void printCam();
    void CambiaTextoAgnt(int bAgente);
private:
    std::string SplitNombre(std::string str);
    void setCamJuego();
    void setCamMenu();
    void setSkinAmbiente();
    void dropSkinAmbiente();
    scene::ISceneManager* smgr;
    gui::IGUIEnvironment* env;
    std::vector<std::string> Agentes;
    bool hayagente;
    ManejadorGUI *mgui;
    Partida *partida;
    Grafico::Skin* skin;
    AnimacionEnd *aniend;
    scene::ITerrainSceneNode* terrain ;
    scene::ISceneNode* skydome;

};

#endif	/* _MANEJADORJUEGO_HPP */

