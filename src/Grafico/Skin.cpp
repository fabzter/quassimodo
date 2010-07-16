#include "Skin.hpp"
#include <vector>

Grafico::Skin::Skin(scene::ISceneManager* smgr,gui::IGUIEnvironment* env, io::IFileSystem* fsys,
        Opciones::ManejadorOpciones &opciones) {
    this->opciones = &opciones;
    this->setAntorcha(smgr);
    this->setBarrera(smgr);
    this->setCelda(smgr);
    this->setJugadores(smgr);
    this->setTablero(smgr);
    this->setDefault(env);
    this->setGUIBoton(env);
    this->setMenuBoton(env);
    this->setMenuToolTip(env);
    this->setTerrain(smgr,fsys);
    this->setSkyDome(smgr);
    this->setSkinGui(env,fsys,smgr->getVideoDriver());
    this->setBotonesPartida(smgr);
}

Grafico::Skin::Skin(const Skin& orig) {
}

Grafico::Skin::~Skin() {
}
void Grafico::Skin::setTablero(scene::ISceneManager* smgr){
     std::ostringstream strs;
    this->Tablero=smgr->getMesh(this->opciones->getTableroModeloPath().c_str());
    this->TTAblero=smgr->getVideoDriver()->getTexture(this->opciones->getTableroTexturaPath().c_str());
    if( this->Tablero== NULL || this->TTAblero==0 )
        {
            strs << "No pudo ser cargado el Skin en la parte del Tablero ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setJugadores(scene::ISceneManager* smgr){
     std::ostringstream strs;

     this->Jugador2=smgr->getMesh(this->opciones->getJugador2ModeloPath().c_str());
      if( this->Jugador2== NULL  )
        {
            strs << "No pudo ser cargado el Skin del jugador 2 ";
            throw SkinNoCargado(strs.str().c_str());
        }

     this->Jugador1=smgr->getMesh(this->opciones->getJugador1ModeloPath().c_str());
    
     if( this->Jugador1== NULL  )
        {
            strs << "No pudo ser cargado el Skin del jugador 1 ";
            throw SkinNoCargado(strs.str().c_str());
        }
     this->SombraJugador=smgr->getMesh(this->opciones->getJugadorSombraModeloPath().c_str());

     if( this->SombraJugador== NULL  )
         {
            strs << "No pudo ser cargado el Skin de la sombra de los jugadores ";
            throw SkinNoCargado(strs.str().c_str());
        }
     
}
void Grafico::Skin:: setAntorcha(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Antorcha=smgr->getMesh(this->opciones->getAntorchaModeloPath().c_str());
    this->TAntorcha=smgr->getVideoDriver()->getTexture(this->opciones->getAntorchaTexturaPath().c_str());
      if( this->Antorcha== NULL || this->TAntorcha== 0  )
        {
            strs << "No pudo ser cargado el Skin en la parte de la antorcha ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setBarrera(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Barrera=smgr->getMesh(this->opciones->getBarreraModeloPath().c_str());
    this->TBarrera=smgr->getVideoDriver()->getTexture(this->opciones->getBarreraTexturaPath().c_str());
      if( this->Barrera== NULL || this->TBarrera==0 )
        {
            strs << "No pudo ser cargado el Skin en la parte de la Barrera ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setCelda(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Celda=smgr->getMesh(this->opciones->getCeldaModeloPath().c_str());
    this->TCelda=smgr->getVideoDriver()->getTexture(this->opciones->getCeldaTexturaPath().c_str());
    if( this->Celda== NULL || this->TCelda== 0 )
        {
            strs << "No pudo ser cargado el Skin en la parte de la Celda ";
            throw SkinNoCargado(strs.str().c_str());
        }
}
void Grafico::Skin::setMenuBoton(gui::IGUIEnvironment* env){
     std::ostringstream strs;
     this->MenuBoton=env->getFont("conf/gui_default/bigfont.png");
     if(this->MenuBoton==0){
          strs << "No pudo ser cargada la Fuente de los Botones del Menú ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setDefault(gui::IGUIEnvironment* env){
    std::ostringstream strs;
     this->Default=env->getFont("conf/gui_default/defaultfont2.png");
     if(this->Default==0){
          strs << "No pudo ser cargada la Fuente Default ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setMenuToolTip(gui::IGUIEnvironment* env){
    std::ostringstream strs;
     this->MenuToolTip=env->getFont("conf/gui_default/tooltipfont.png");
     if(this->MenuToolTip==0){
          strs << "No pudo ser cargada la Fuente del Tooltip del Menú ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setGUIBoton(gui::IGUIEnvironment* env){
     std::ostringstream strs;
     this->GUIBoton=env->getFont("conf/gui_default/botonFont.png");
     if(this->GUIBoton==0){
          strs << "No pudo ser cargada la Fuente de los Botones de la GUI ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setGUIWindow(gui::IGUIEnvironment* env){
      std::ostringstream strs;
     this->GUIWindow=env->getFont("conf/gui_default/windowfont.png");
     if(this->GUIWindow==0){
          strs << "No pudo ser cargada la Fuente de la ventana de la GUI ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setTerrain(scene::ISceneManager* smgr, io::IFileSystem* fsys){
     std::ostringstream strs;
    this->TTerrain =smgr->getVideoDriver()->getTexture(this->opciones->getTerrenoTexturaPath().c_str());
   this->heightMapFile=fsys->createAndOpenFile(this->opciones->getTerrenoHeightTexturaPath().c_str());
     if( this->TTerrain== NULL  )
        {
            strs << "No pudo ser cargado el Skin en la parte del Terreno ";
            throw SkinNoCargado(strs.str().c_str());
        }

}
void Grafico::Skin::setSkyDome(scene::ISceneManager* smgr){
     std::ostringstream strs;
    this->Tskydome =smgr->getVideoDriver()->getTexture(this->opciones->getSkydomeTexturaPath().c_str());
     if( this->Tskydome== NULL  )
        {
            strs << "No pudo ser cargado el Skin en la parte del skydome";
            throw SkinNoCargado(strs.str().c_str());
        }

}
void Grafico::Skin::setSkinGui(gui::IGUIEnvironment* env, io::IFileSystem* fsys,video::IVideoDriver* driver){

    gui::SImageGUISkinConfig guicfg = LoadGUISkinFromFile(fsys, driver, "conf/gui_default/guiskin.cfg");
    this->skin = new gui::CImageGUISkin(driver, env->getSkin());
    this->skin->loadConfig(guicfg);
    

}
void Grafico::Skin::setBotonesPartida(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture( "conf/gui_default/boton4_2-1_vuelta.png" ) );
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture( "conf/gui_default/boton4_2-2_vuelta.png" ) );
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture( "conf/gui_default/boton5_2-1_frente.png" ) );
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture( "conf/gui_default/boton5_2-2_vuelta.png" ) );
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture( "conf/gui_default/boton1_2_pausa.png" ) );
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture( "conf/gui_default/boton3_2_menu.png" ) );
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture( "conf/gui_default/boton2_2_pausa.png" ) );
    for(int i=0;i<this->botonesPartida.size();i++){
         if( this->botonesPartida.at(i)== NULL  )
            {
                strs << "No pudo ser cargado el Skin en la parte de los botones de la partida ( el boton "<<i<<" )";
                throw SkinNoCargado(strs.str().c_str());
            }
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
io::IReadFile* Grafico::Skin::getheightMapFile(){
    return this->heightMapFile;
}
video::ITexture* Grafico::Skin::getTTerrain(){
    return this->TTerrain;
}
video::ITexture* Grafico::Skin::getTSkydome(){
    return this->Tskydome;
}
gui::IGUISkin*  Grafico::Skin::getSkinGui(){
    return this->skin;
}
video::ITexture*  Grafico::Skin::getBotonPartida(int i){
    return this->botonesPartida.at(i);
}