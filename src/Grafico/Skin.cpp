
#include "Skin.hpp"

Grafico::Skin::Skin(scene::ISceneManager* smgr) {
    this->setAntorcha(smgr);
    this->setBarrera(smgr);
    this->setCelda(smgr);
    this->setJugadores(smgr);
    this->setTablero(smgr);
}

Grafico::Skin::Skin(const Skin& orig) {
}

Grafico::Skin::~Skin() {
}
void Grafico::Skin::setTablero(scene::ISceneManager* smgr){
     std::ostringstream strs;
    this->Tablero=smgr->getMesh("Texturas/Tablero4.3ds");
    this->TTAblero=smgr->getVideoDriver()->getTexture( "Texturas/arena1.jpg" );
    if( this->Tablero== NULL || this->TTAblero==0 )
        {
            strs << "No pudo ser cargado el Skin en la parte del Tablero ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setJugadores(scene::ISceneManager* smgr){
     std::ostringstream strs;
    this->Jugador1=smgr->getMesh("Texturas/Mono.3ds");
    this->Jugador2=smgr->getMesh("Texturas/Mono.3ds");
     if( this->Jugador1== NULL || this->Jugador2== NULL  )
        {
            strs << "No pudo ser cargado el Skin en la parte de los Jugadores ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin:: setAntorcha(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Antorcha=smgr->getMesh("Texturas/Lumbrera4.3ds");
    this->TAntorcha=smgr->getVideoDriver()->getTexture("Texturas/fire.bmp");
      if( this->Antorcha== NULL || this->TAntorcha== 0  )
        {
            strs << "No pudo ser cargado el Skin en la parte de la antorcha ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setBarrera(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Barrera=smgr->getMesh("Texturas/Barrera3.3ds");
    this->TBarrera=smgr->getVideoDriver()->getTexture( "Texturas/wall.jpg" );
      if( this->Barrera== NULL || this->TBarrera==0 )
        {
            strs << "No pudo ser cargado el Skin en la parte de la Barrera ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setCelda(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Celda=smgr->getMesh("Texturas/Celda4.3ds");
    this->TCelda=smgr->getVideoDriver()->getTexture( "Texturas/arena2.JPG" );
    if( this->Celda== NULL || this->TCelda== 0 )
        {
            strs << "No pudo ser cargado el Skin en la parte de la Celda ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
scene::IAnimatedMesh* Grafico::Skin::getCelda(){
    return this->Celda;
}
scene::IAnimatedMesh* Grafico::Skin::getTablero(){
    return this->Tablero;
}
scene::IAnimatedMesh* Grafico::Skin::getAntorcha(){
    return this->Antorcha;
}
scene::IAnimatedMesh* Grafico::Skin::getJugador1(){
    return this->Jugador1;
}
scene::IAnimatedMesh* Grafico::Skin::getJugador2(){
    return this->Jugador2;
}
scene::IAnimatedMesh* Grafico::Skin::getBarrera(){
    return this->Barrera;
}
video::ITexture* Grafico::Skin::getTTAblero(){
    return this->TTAblero;
}
video::ITexture* Grafico::Skin::getTCelda(){
    return this->TCelda;
}
video::ITexture* Grafico::Skin::getTAntorcha(){
    return this->TAntorcha;
}
video::ITexture* Grafico::Skin::getTBarrera(){
    return this->TBarrera;
}

