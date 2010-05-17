

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
    this->AvsA=NULL;
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
        if(this->AvsA!=NULL)
        this->AvsA->remove();
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
 void Menu::setSkin(bool dialogo){
     gui::IGUISkin* skin = this->env->createSkin(gui::EGST_WINDOWS_METALLIC);
	this->env->setSkin(skin);
        gui::IGUIFont* font;
     if(!dialogo){
        font = this->env->getFont("Texturas/bigfont.png");
     }
     else
         font = this->env->getFont("Texturas/defaultfont2.png");
     if (font)
         skin->setFont(font,gui::EGDF_BUTTON);
    font = this->env->getFont("Texturas/defaultfont2.png");
     if (font)
         skin->setFont(font,gui::EGDF_DEFAULT);
     font = this->env->getFont("Texturas/tooltipfont.png");
     if (font)
         skin->setFont(font,gui::EGDF_TOOLTIP);
     font = this->env->getFont("Texturas/windowfont.png");
     if (font)
         skin->setFont(font,gui::EGDF_WINDOW);
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
void Menu::MsgBox(const char* msg ){

    wchar_t* m;
    size_t requiredSize=strlen(msg);
    m = (wchar_t*)malloc( requiredSize);
    mbstowcs( m,msg, requiredSize);
    gui::IGUIWindow* window=this->env->addMessageBox(L"Quassimodo dice:",m);
    //free(m);
}
 void Menu::AgntVSAgnt(){

    // this->setSkin(this->env,true);
        core::dimension2d<unsigned int> S_S=this->smgr->getVideoDriver()->getScreenSize();
      int d_a=100,d_al=150;
        core::rect<s32> recW=core::rect<s32>(100, 150 , S_S.Width-d_a ,S_S.Height-d_al);
    
      AvsA= this->env->addWindow(recW,true,L"Agentes:");

       this->env->addStaticText(L"Vs",core::rect<s32>((recW.getWidth()/2)-15,(recW.getHeight()/2)-30,(recW.getWidth()/2)+15,(recW.getHeight()/2)+40),\
               false,false,AvsA);

       this->env->addStaticText( L"Selecciona los Agentes", core::rect<s32>(35,35,300,60), false, false, AvsA );
       //botones de los agentes
       this->env->addButton( core::rect<s32>( (recW.getWidth()/2)-185,(recW.getHeight()/2)-50 ,(recW.getWidth()/2)-35,(recW.getHeight()/2) ),\
               AvsA, BA_AGENTE_1, L"Agente?");
       this->env->addButton( core::rect<s32>( (recW.getWidth()/2)+40,(recW.getHeight()/2)-50 ,(recW.getWidth()/2)+190,(recW.getHeight()/2) ),\
               AvsA, BA_AGENTE_2, L"Agente?");
       //botones de Opcion
       this->env->addButton( core::rect<s32>( recW.getWidth()-360,recW.getHeight()-60 ,recW.getWidth()-190,recW.getHeight()-20) ,\
               AvsA, BO_CANCELA, L"Cancelar");
       this->env->addButton( core::rect<s32>( recW.getWidth()-160,recW.getHeight()-60 ,recW.getWidth()-20,recW.getHeight()-20 ),\
               AvsA, BO_INICIA, L"Inicia");
       //this->env->addFileOpenDialog(L"Selecciona el Agente",true,0,-1);
 }
  std::string Menu::getPath(){

     const  wchar_t*   m=op->getFileName();
    size_t requiredSize=wcslen(m);
    char* msg = (char*)malloc( requiredSize*sizeof(wchar_t));
    wcstombs( msg,m, requiredSize);
    std::cout<<msg<<std::endl;
    //free(msg);
     
     return msg;
  }

 void  Menu::OpenFileDialog(){
      op = this->env->addFileOpenDialog(L"Selecciona el Agente",true,0,5);
  }