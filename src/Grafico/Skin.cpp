
#include "Skin.hpp"

Grafico::Skin::Skin(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {
    this->setAntorcha(smgr);
    this->setBarrera(smgr);
    this->setCelda(smgr);
    this->setJugadores(smgr);
    this->setTablero(smgr);
    this->setDefault(env);
    this->setGUIBoton(env);
    this->setMenuBoton(env);
    this->setMenuToolTip(env);
    this->setTerrain(smgr);
    this->setSkyDome(smgr);
}

Grafico::Skin::Skin(const Skin& orig) {
}

Grafico::Skin::~Skin() {
}
void Grafico::Skin::setTablero(scene::ISceneManager* smgr){
     std::ostringstream strs;
    this->Tablero=smgr->getMesh("Texturas/Tablero.3ds");
    this->TTAblero=smgr->getVideoDriver()->getTexture( "Texturas/Moss0138_10_S.jpg" );
    if( this->Tablero== NULL || this->TTAblero==0 )
        {
            strs << "No pudo ser cargado el Skin en la parte del Tablero ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setJugadores(scene::ISceneManager* smgr){
     std::ostringstream strs;

     this->Jugador2=smgr->getMesh("Texturas/MonoBlanco.3ds");
      if( this->Jugador2== NULL  )
        {
            strs << "No pudo ser cargado el Skin del jugador 2 ";
            throw SkinNoCargado(strs.str().c_str());
        }

     this->Jugador1=smgr->getMesh("Texturas/Mono.3ds");
    
     if( this->Jugador1== NULL  )
        {
            strs << "No pudo ser cargado el Skin del jugador 1 ";
            throw SkinNoCargado(strs.str().c_str());
        }
     this->SombraJugador=smgr->getMesh("Texturas/MonoSombra.3ds");

     if( this->SombraJugador== NULL  )
         {
            strs << "No pudo ser cargado el Skin de la sombra de los jugadores ";
            throw SkinNoCargado(strs.str().c_str());
        }
     
}
void Grafico::Skin:: setAntorcha(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Antorcha=smgr->getMesh("Texturas/Lumbrera.3ds");
    this->TAntorcha=smgr->getVideoDriver()->getTexture("Texturas/fire.bmp");
      if( this->Antorcha== NULL || this->TAntorcha== 0  )
        {
            strs << "No pudo ser cargado el Skin en la parte de la antorcha ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setBarrera(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Barrera=smgr->getMesh("Texturas/Barrera.3ds");
    this->TBarrera=smgr->getVideoDriver()->getTexture( "Texturas/BrickOldDirty0078_S.jpg" );
      if( this->Barrera== NULL || this->TBarrera==0 )
        {
            strs << "No pudo ser cargado el Skin en la parte de la Barrera ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setCelda(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Celda=smgr->getMesh("Texturas/Celda.3ds");//Moss0138_2_S.jpg RockMossy0018_S.jpg
    this->TCelda=smgr->getVideoDriver()->getTexture( "Texturas/Moss0138_2_S.jpg" );
    if( this->Celda== NULL || this->TCelda== 0 )
        {
            strs << "No pudo ser cargado el Skin en la parte de la Celda ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setMenuBoton(gui::IGUIEnvironment* env){
     std::ostringstream strs;
     this->MenuBoton=env->getFont("Texturas/bigfont.png");
     if(this->MenuBoton==0){
          strs << "No pudo ser cargada la Fuente de los Botones del Menú ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setDefault(gui::IGUIEnvironment* env){
    std::ostringstream strs;
     this->Default=env->getFont("Texturas/defaultfont2.png");
     if(this->Default==0){
          strs << "No pudo ser cargada la Fuente Default ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setMenuToolTip(gui::IGUIEnvironment* env){
    std::ostringstream strs;
     this->MenuToolTip=env->getFont("Texturas/tooltipfont.png");
     if(this->MenuToolTip==0){
          strs << "No pudo ser cargada la Fuente del Tooltip del Menú ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setGUIBoton(gui::IGUIEnvironment* env){
     std::ostringstream strs;
     this->GUIBoton=env->getFont("Texturas/botonFont.png");
     if(this->GUIBoton==0){
          strs << "No pudo ser cargada la Fuente de los Botones de la GUI ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setGUIWindow(gui::IGUIEnvironment* env){
      std::ostringstream strs;
     this->GUIWindow=env->getFont("Texturas/windowfont.png");
     if(this->GUIWindow==0){
          strs << "No pudo ser cargada la Fuente de la ventana de la GUI ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setTerrain(scene::ISceneManager* smgr){
   // this->heightMTerrain=;
     std::ostringstream strs;
    this->TTerrain =smgr->getVideoDriver()->getTexture( "Texturas/piso3_TX.jpg" );
    this->terrain=smgr->addTerrainSceneNode("Texturas/piso3_HM.bmp",
		0,-1,core::vector3df(-4200.f, -80.f, -3000.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		core::vector3df(12.0f, 0.5f, 12.0f),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,					// maxLOD
		scene::ETPS_17,				// patchSize
		4					// smoothFactor
		);
     if( this->TTerrain== NULL || this->terrain==NULL  )
        {
            strs << "No pudo ser cargado el Skin en la parte del Terreno ";
            throw SkinNoCargado(strs.str().c_str());
        }

}
void Grafico::Skin::setSkyDome(scene::ISceneManager* smgr){
     std::ostringstream strs;
    this->Tskydome =smgr->getVideoDriver()->getTexture( "Texturas/3.tree.skydome.png" );
     if( this->Tskydome== NULL  )
        {
            strs << "No pudo ser cargado el Skin en la parte del skydome";
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
 scene::IAnimatedMesh* Grafico::Skin::getSombraJugador(){
   return  this->SombraJugador;
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
gui::IGUIFont* Grafico::Skin::getMenuBoton(){
    return this->MenuBoton;
}
gui::IGUIFont* Grafico::Skin::getDefault(){
    return this->Default;
}
gui::IGUIFont* Grafico::Skin::getMenuToolTip(){
    return this->MenuToolTip;
}
gui::IGUIFont* Grafico::Skin::getGUIBoton(){
    return this->GUIBoton;
}
gui::IGUIFont* Grafico::Skin::getGUIWindow(){
    return this->GUIWindow;
}
scene::ITerrainSceneNode* Grafico::Skin::getterrain(){
    return this->terrain;
}
video::ITexture* Grafico::Skin::getTTerrain(){
    return this->TTerrain;
}
video::ITexture* Grafico::Skin::getTSkydome(){
    return this->Tskydome;
}