
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/vector3d.h>

#include "GUI.hpp"
using namespace irr;
using namespace Grafico;

GUI::GUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env) {
    this->t=new Tablero(smgr);
    this->antorchas.reserve(4);
    this->antorchas.resize(4);
    this->smgr=smgr;
    this->env=env;
    this->jugadores.reserve(4);
    this->jugadores.resize(4);
    this->botones.reserve(B_COUNT);
    for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)=new Antorcha(smgr,0,0);
    }
    this->ColocaAntorchas();
    this->SetJugadores();
    this->setCamara();
    this->setGUI();
}

GUI::GUI(const GUI& orig) {
}

GUI::~GUI() {
}
 void GUI::ColocaAntorchas(){

      int x,x1,z,z1,y;
     x=this->t->getPosicionTablero().X+(this->t->getEscala().X);
    x1=this->t->getPosicionTablero().X+(this->t->getsizeLineaCeldas().X*this->t->getEscala().X);
    z=this->t->getPosicionTablero().Z+(this->t->getEscala().Z);
    z1=this->t->getPosicionTablero().Z+(this->t->getsizeLineaCeldas().Z*this->t->getEscala().Z);
    y=this->t->getPosicionTablero().Y+(this->t->getEscala().Y);
     this->antorchas.at(0)->setPosicionAntorcha(x,y,z);
     this->antorchas.at(1)->setPosicionAntorcha(x1,y,z);
     this->antorchas.at(2)->setPosicionAntorcha(x1,y,z1);
     this->antorchas.at(3)->setPosicionAntorcha(x,y,z1);
 }
 void GUI::SetJugadores(){

    scene::IAnimatedMesh *mono=this->smgr->getMesh("Texturas/Mono.3ds") ;
    this->jugadores.at(0)=new Grafico::Pieza();
    this->jugadores.at(0)->setMesh( mono);
    this->jugadores.at(0)->dibuja( this->smgr );
    core::vector3df p=this->t->getPosicionCelda(4,0);
    p.Y+=this->t->getsizeCelda().Y;
     this->jugadores.at(0)->setPosicion( p );

      this->jugadores.at(1)=new Grafico::Pieza();
    this->jugadores.at(1)->setMesh(mono );
    this->jugadores.at(1)->dibuja( this->smgr );
    p=this->t->getPosicionCelda(4,8);
    p.Y+=this->t->getsizeCelda().Y;
     this->jugadores.at(1)->setPosicion( p );
}
 void GUI::setCamara(){
     scene::ICameraSceneNode *cam= smgr->addCameraSceneNode();
     cam->setTarget(core::vector3df(18.993,22.82,100.268));
     cam->setPosition(core::vector3df(-3.18,24.48,122.98));
     this->smgr->setActiveCamera(cam);
 }
 void GUI::setGUI(){
     this->setSkin();
     this->setBotones();


 }

 void GUI::setSkin(){
     gui::IGUISkin *skin=env->getSkin();
     gui::IGUIFont* font = env->getFont("Texturas/bigfont.png");
     if (font)
         skin->setFont(font,gui::EGDF_BUTTON);
     video::SColor coln=video::SColor();
     coln.set(0,0,0,0);
     video::SColor col= video::SColor();
     col.set(255,255,255,255);
   //  col.setAlpha(10);//EGUI_DEFAULT_COLOR
     skin->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,255,255,255));
      skin->setColor(gui::EGDC_3D_FACE,coln);
    /*  skin->setColor(gui::EGDC_3D_DARK_SHADOW ,coln);
       skin->setColor(gui::EGDC_3D_SHADOW ,coln);
skin->setColor(gui::EGDC_3D_LIGHT ,coln);*/
       //skin->setColor(gui::EGDC_3D_SHADOW,video::SColor(50,50,50,50));


 }
 void GUI::setBotones(){
      core::dimension2d<unsigned int> Scren_Size= this->smgr->getVideoDriver()->getScreenSize();
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

