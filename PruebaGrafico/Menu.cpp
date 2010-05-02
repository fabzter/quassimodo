

#include "Menu.hpp"
using namespace irr;
using namespace Grafico;

Menu::Menu(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t) {
    this->jugadores.reserve(4);
    this->jugadores.resize(4);
    this->botones.reserve(B_COUNT);
    this->setMenuP(smgr,env,t);
}


Menu::Menu(const Menu& orig) {
}

Menu::~Menu() {
    this->dropMenuP();
}

 void Menu::SetJugadores(scene::ISceneManager* smgr,Grafico::Tablero* t){

    scene::IAnimatedMesh *mono=smgr->getMesh("Texturas/Mono.3ds") ;
    for(int i=0;i<t->num_jugadores;i++){
         this->jugadores.at(i)=new Grafico::Pieza();
        this->jugadores.at(i)->setMesh( mono);
        this->jugadores.at(i)->dibuja( smgr );
        core::vector3df p=t->getPosicionCelda(4,(i*8));
        p.Y+=t->getsizeCelda().Y;
        this->jugadores.at(i)->setPosicion( p );
    }

}
 void Menu::setCamara(scene::ISceneManager* smgr){
     scene::ICameraSceneNode *cam= smgr->addCameraSceneNode();
     cam->setTarget(core::vector3df(18.993,22.82,100.268));
     cam->setPosition(core::vector3df(-3.18,24.48,122.98));
    smgr->setActiveCamera(cam);
 }
 void Menu::setMenuP(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t){

     haymenu=true;
     this->SetJugadores(smgr,t);
     this->setCamara(smgr);
     this->setSkin(env);
     this->setBotones(smgr,env);
 }
void Menu::dropMenuP(){
    if( haymenu){
         for (int i=0;i<B_COUNT;i++){
             this->botones.at(i)->remove();
             if(i<2)
                  this->jugadores.at(i)->drop();
        }
         haymenu=false;
    }
  
/*    this->botones.clear();
    this->jugadores.clear();*/
}
 void Menu::setSkin(gui::IGUIEnvironment* env){
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
      //skin->setColor(gui::EGDC_3D_FACE,coln);
    /*  skin->setColor(gui::EGDC_3D_DARK_SHADOW ,coln);
       skin->setColor(gui::EGDC_3D_SHADOW ,coln);
skin->setColor(gui::EGDC_3D_LIGHT ,coln);*/
       //skin->setColor(gui::EGDC_3D_SHADOW,video::SColor(50,50,50,50));


 }
 void Menu::setBotones(scene::ISceneManager* smgr,gui::IGUIEnvironment* env){
      core::dimension2d<unsigned int> Scren_Size=smgr->getVideoDriver()->getScreenSize();
      int dis_ancho=100,dis_alto=70;
      float t_alto=(Scren_Size.Height- (2*dis_alto) )/B_COUNT, t_ancho=Scren_Size.Width-(2*dis_ancho);
      int p_alto=dis_alto;
      for(int i=0;i<B_COUNT;i++){
          
            this->botones.push_back( env->addButton(core::rect<s32>( dis_ancho, p_alto, t_ancho+dis_ancho, p_alto+t_alto ),0,
              i,GUI_BOTONES_TEXTO[i],GUI_BOTONES_TOLTIP[i]) );
             p_alto+=t_alto;
      this->botones.at(i)->setDrawBorder(false);

      }
    
 }
void Menu::MsgBox(gui::IGUIEnvironment* env,const char* msg ){

    wchar_t* m;
    size_t requiredSize=strlen(msg);
    m = (wchar_t*)malloc( requiredSize + 1);
    mbstowcs( m,msg, requiredSize);
    gui::IGUIWindow* window=env->addMessageBox(L"Quassimodo dice:",m);
     /*= env->addWindow(core::rect<s32>(100, 100 , 300 , 200 ),false,L"Quassimodo dice:");
                    env->addStaticText(m,
						core::rect<s32>(35,35,140,50),
						true, // border?
						false, // wordwrap?
						window);*/
                    free(m);
}
