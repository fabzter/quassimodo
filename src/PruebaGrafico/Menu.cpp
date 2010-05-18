

#include "Menu.hpp"
using namespace irr;
using namespace Grafico;

Menu::Menu(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Skin* skin) {
    this->env=env;
    this->smgr=smgr;
    this->skin=skin;
    this->jugadores.reserve(4);
    this->jugadores.resize(4);
    this->botones.reserve(B_COUNT);
    this->setMenuP(t);

}


Menu::Menu(const Menu& orig) {
}

Menu::~Menu() {
    this->dropMenuP();
}

 void Menu::SetJugadores(Grafico::Tablero* t){

    for(int i=0;i<t->num_jugadores;i++){
         this->jugadores.at(i)=new Grafico::Pieza();
        this->jugadores.at(i)->setMesh( this->skin->getJugador1() );
        this->jugadores.at(i)->dibuja( this->smgr );
        core::vector3df p=t->getPosicionCelda(4,(i*8));
        p.Y+=t->getsizeCelda().Y;
        this->jugadores.at(i)->setPosicion( p );
    }

}
 void Menu::setCamara(){
     scene::ICameraSceneNode *cam= this->smgr->addCameraSceneNode();
     cam->setTarget(core::vector3df(-12.2491,8.5962,107.647));
     cam->setPosition(core::vector3df(-66.0566,28.3357,174.094));
    this->smgr->setActiveCamera(cam);
 }
 void Menu::setMenuP(Grafico::Tablero* t){

     haymenu=true;
     this->SetJugadores(t);
     this->setCamara();
     this->setSkin();
     this->setBotones();
 }
void Menu::dropMenuP(){
    if( haymenu){
         for (int i=0;i<B_COUNT;i++){
             this->botones.at(i)->remove();
             if(i<2)
                  this->jugadores.at(i)->drop();
        }
         haymenu=false;
         this->botones.clear();
    }
  
/*    this->botones.clear();
    this->jugadores.clear();*/
}
 void Menu::setSkin(bool dialogo){
     gui::IGUISkin* skin = this->env->createSkin(gui::EGST_WINDOWS_METALLIC);
	this->env->setSkin(skin);
   
         skin->setFont(this->skin->getMenuBoton(),gui::EGDF_BUTTON);
         skin->setFont(this->skin->getDefault(),gui::EGDF_DEFAULT);
         skin->setFont(this->skin->getMenuToolTip(),gui::EGDF_TOOLTIP);
      
      skin->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,255,255,255));


    skin->drop();
 }
 void Menu::setBotones(){
      core::dimension2d<unsigned int> Scren_Size=this->smgr->getVideoDriver()->getScreenSize();
      int dis_ancho=100,dis_alto=70;
      float t_alto=(Scren_Size.Height- (2*dis_alto) )/B_COUNT, t_ancho=Scren_Size.Width-(2*dis_ancho);
      int p_alto=dis_alto;
      for(int i=0;i<B_COUNT;i++){
          
            this->botones.push_back( this->env->addButton(core::rect<s32>( dis_ancho, p_alto, t_ancho+dis_ancho, p_alto+t_alto ),0,
              i,GUI_BOTONES_TEXTO[i],GUI_BOTONES_TOLTIP[i]) );
             p_alto+=t_alto;
      this->botones.at(i)->setDrawBorder(false);

      }
    
 }
