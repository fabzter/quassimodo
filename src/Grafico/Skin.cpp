#include "Skin.hpp"
#include <vector>

Grafico::Skin::Skin(scene::ISceneManager* smgr,gui::IGUIEnvironment* env, io::IFileSystem* fsys,
        Opciones::ManejadorOpciones &opciones) {
    this->opciones = &opciones;
    this->setFuego(smgr);
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
    this->setSkinGui(env,fsys,smgr->getVideoDriver());
    this->setBotonesPartida(smgr);
    this->setBotonesJugador(smgr);
    this->setImagenCreditos(smgr);
}

Grafico::Skin::Skin(const Skin& orig) {
}

Grafico::Skin::~Skin() {
}
void Grafico::Skin:: setFuego(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->TAntorcha=smgr->getVideoDriver()->getTexture(this->opciones->getAntorchaTexturaPath().c_str());
      if( this->TAntorcha== 0  )
        {
            strs << "No pudo ser cargado el Skin en la parte del Fuego ";
            throw SkinNoCargado(strs.str().c_str());
        }
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
     
}

void Grafico::Skin::setBarrera(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->Barrera=smgr->getMesh(this->opciones->getBarreraModeloPath().c_str());
    this->TBarrera=smgr->getVideoDriver()->getTexture(this->opciones->getBarrera1TexturaPath().c_str());
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
     this->MenuBoton=env->getFont(this->opciones->getMenuFontPath().c_str());
     if(this->MenuBoton==0){
          strs << "No pudo ser cargada la Fuente de los Botones del Menú ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setDefault(gui::IGUIEnvironment* env){
    std::ostringstream strs;
     this->Default=env->getFont(this->opciones->getDefaultFontPath().c_str());
     if(this->Default==0){
          strs << "No pudo ser cargada la Fuente Default ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setMenuToolTip(gui::IGUIEnvironment* env){
    std::ostringstream strs;
     this->MenuToolTip=env->getFont(this->opciones->getTooltipFontPath().c_str() );
     if(this->MenuToolTip==0){
          strs << "No pudo ser cargada la Fuente del Tooltip del Menú ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setGUIBoton(gui::IGUIEnvironment* env){
     std::ostringstream strs;
     this->GUIBoton=env->getFont(this->opciones->getButtonFontPath().c_str());
     if(this->GUIBoton==0){
          strs << "No pudo ser cargada la Fuente de los Botones de la GUI ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setGUIWindow(gui::IGUIEnvironment* env){
      std::ostringstream strs;
     this->GUIWindow=env->getFont(this->opciones->getWindowFontPath().c_str());
     if(this->GUIWindow==0){
          strs << "No pudo ser cargada la Fuente de la ventana de la GUI ";
            throw SkinNoCargado(strs.str().c_str());
     }
}
void Grafico::Skin::setTerrain(scene::ISceneManager* smgr){
     std::ostringstream strs;
    this->TTerrain =smgr->getVideoDriver()->getTexture(this->opciones->getTerrenoTexturaPath().c_str());
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

    gui::SImageGUISkinConfig guicfg = LoadGUISkinFromFile(fsys, driver, this->opciones->getGUIConfig().c_str());
    this->skin = new gui::CImageGUISkin(driver, env->getSkin());
    this->skin->loadConfig(guicfg);
    

}
void Grafico::Skin::setBotonesPartida(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonVuelta1Path().c_str()));
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonVuelta2Path().c_str()));
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonFrentePath().c_str() ) );
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonVuelta3Path().c_str()));
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonPausa1Path().c_str() ) );
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonMenuPath().c_str() ) );
    this->botonesPartida.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonPausa2Path().c_str() ) );
    this->botonesPartidaPresionado.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonVuelta1PresPath().c_str() ));
    this->botonesPartidaPresionado.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonVuelta2PresPath().c_str()));
    this->botonesPartidaPresionado.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonFrentePresPath().c_str() ) );
    this->botonesPartidaPresionado.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonVuelta3PresPath().c_str()));
    this->botonesPartidaPresionado.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonPausa1PresPath().c_str() ) );
    this->botonesPartidaPresionado.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonMenuPresPath().c_str() ) );
    this->botonesPartidaPresionado.push_back( smgr->getVideoDriver()->getTexture(this->opciones->getBotonPausa2PresPath().c_str() ) );
    for(int i=0;i<this->botonesPartida.size();i++){
         if( this->botonesPartida.at(i)== NULL || this->botonesPartidaPresionado.at(i)== NULL )
            {
                strs << "No pudo ser cargado el Skin en la parte de los botones de la partida ( el boton "<<i<<" )";
                throw SkinNoCargado(strs.str().c_str());
            }
    }
}
void Grafico::Skin::setBotonesJugador(scene::ISceneManager* smgr){
    std::ostringstream strs;

    this->botonesJugador.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getMoverPinguinoPath().c_str() ));
    this->botonesJugador.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getSetBarreraPinguinoPath().c_str() ));
    this->botonesJugador.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getGiraEsteBarreraPinguinoPath().c_str() ));
    this->botonesJugador.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getGiraNorteBarreraPinguinoPath().c_str() ));
    this->botonesJugador.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getMoverRobotPath().c_str() ));
    this->botonesJugador.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getSetBarreraRobotPath().c_str() ));
    this->botonesJugador.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getGiraEsteBarreraRobotPath().c_str() ));
    this->botonesJugador.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getGiraNorteBarreraRobotPath().c_str() ));

    this->botonesJugadorPresionado.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getMoverPinguinoPressPath().c_str() ));
    this->botonesJugadorPresionado.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getSetBarreraPinguinoPressPath().c_str() ));
    this->botonesJugadorPresionado.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getGiraEsteBarreraPinguinoPressPath().c_str() ));
    this->botonesJugadorPresionado.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getGiraNorteBarreraPinguinoPressPath().c_str() ));
    this->botonesJugadorPresionado.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getMoverRobotPressPath().c_str() ));
    this->botonesJugadorPresionado.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getSetBarreraRobotPressPath().c_str() ));
    this->botonesJugadorPresionado.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getGiraEsteBarreraRobotPressPath().c_str() ));
    this->botonesJugadorPresionado.push_back(smgr->getVideoDriver()->getTexture(this->opciones->getGiraNorteBarreraRobotPressPath().c_str() ));
     for(int i=0;i<this->botonesJugador.size();i++){
         if( this->botonesJugador.at(i)== NULL || this->botonesJugadorPresionado.at(i)== NULL )
            {
                strs << "No pudo ser cargado el Skin en la parte de los botones del Jugador ( el boton "<<i<<" )";
                throw SkinNoCargado(strs.str().c_str());
            }
    }

}
void Grafico::Skin::setImagenCreditos(scene::ISceneManager* smgr){
    std::ostringstream strs;
    this->creditos=smgr->getVideoDriver()->getTexture(this->opciones->getCreditos().c_str() );
    if( this->creditos == NULL)
    {
        strs << "No pudo ser cargado el Skin en la parte de la imagen de los Creditos ";
        throw SkinNoCargado(strs.str ().c_str());
    }

}

scene::IAnimatedMesh* Grafico::Skin::getCelda(){
    return this->Celda;
}
scene::IAnimatedMesh* Grafico::Skin::getTablero(){
    return this->Tablero;
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
video::ITexture*  Grafico::Skin::getBotonPartidaPres(int i){
    return this->botonesPartidaPresionado.at(i);
}
video::ITexture* Grafico::Skin::getBotonJugador(int i){
    return this->botonesJugador.at(i);
}
video::ITexture* Grafico::Skin::getBotonJugadorPres(int i){
    return this->botonesJugadorPresionado.at(i);
}
video::ITexture* Grafico::Skin::getImagenCreditos(){
    return this->creditos;
}