

#include "Menu.hpp"
using namespace irr;
using namespace Grafico;

Menu::Menu(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Skin* skin) {
    this->env=env;
    this->smgr=smgr;
    this->skin=skin;
    this->t=t;
    this->jugadores.reserve(4);
    this->jugadores.resize(4);
    this->botones.reserve(B_COUNT);
    this->setMenuP();

}


Menu::Menu(const Menu& orig) {
}

Menu::~Menu() {
    this->dropMenuP();
}

 void Menu::SetJugadores(){

    for(int i=0;i<this->t->num_jugadores;i++){
         this->jugadores.at(i)=new Grafico::Pieza();
         if(i==0)
             this->jugadores.at(i)->setMesh( this->skin->getJugador1() );
         else
             this->jugadores.at(i)->setMesh( this->skin->getJugador2() );
        this->jugadores.at(i)->dibuja( this->smgr );
    }
    this->setPosicionJugadores();

}
 void Menu::setPosicionJugadores(){
     
     for(int i=0;i<this->t->num_jugadores;i++){
        core::vector3df p=this->t->getPosicionCelda(4,(i*8));
        p.Y+=this->t->getsizeCelda().Y;
        this->jugadores.at(i)->setPosicion( p );
     }
 }
 void Menu::SetEscala(int x,int y,int z){
     for(int i=0;i<this->t->num_jugadores;i++){
         this->jugadores.at(i)->setEscala(x,y,z);
     }
      this->setPosicionJugadores();
 }

 void Menu::setMenuP(){

     haymenu=true;
     this->SetJugadores();
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
  
}
//TODO: arreglar las letras del menú! las de la ventana que no se ven
 void Menu::setSkin(){

     gui::IGUISkin* skin = this->env->createSkin(gui::EGST_WINDOWS_METALLIC);
	this->env->setSkin(skin);
   
         skin->setFont(this->skin->getMenuBoton(),gui::EGDF_BUTTON);
         skin->setFont(this->skin->getDefault(),gui::EGDF_DEFAULT);
         skin->setFont(this->skin->getMenuToolTip(),gui::EGDF_TOOLTIP);
      
      skin->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,255,255,255));
            //ponemos mas opaco los colores de la GUI
     for (u32 i=0; i<gui::EGDC_COUNT ; ++i)
     {
         video::SColor  col = skin->getColor((gui::EGUI_DEFAULT_COLOR)i);
         col.setAlpha(250);
         skin->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
     }


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
