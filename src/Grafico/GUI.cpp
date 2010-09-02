#include "GUI.hpp"
#include <boost/filesystem.hpp>
using namespace irr;

Grafico::GUI::GUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Skin* Skin) {
    this->smgr=smgr;
    this->env=env;
    this->skin=Skin;
    this->botonPartida.resize(BP_COUNT);
    this->botonesJugador.resize(2);
    this->AvsA=0;
    this->T_Pausa=NULL;
    this->T_Pensando=NULL;
    botonJugador=botonesPartida=false;
   this->setSkin();
}

Grafico::GUI::GUI(const GUI& orig) {

    this->smgr=orig.smgr;
    this->skin=orig.skin;
    this->env=orig.env;
    this->T_Pausa=orig.T_Pausa;
    this->AvsA=orig.AvsA;
    this->botonesPartida=orig.botonesPartida;
    this->setSkin();
}

Grafico::GUI::~GUI() {
    this->dropBotonesPartida();
    this->dropAvsA();
    this->dropTextPausa();
    this->dropBotonesJugador();
}
void Grafico::GUI::setSkin(){

    gui::IGUISkin* sskin = this->skin->getSkinGui();
    sskin->setFont(this->skin->getDefault(),gui::EGDF_DEFAULT);
    sskin->setFont(this->skin->getMenuToolTip(),gui::EGDF_TOOLTIP);
    //ponemos mas opaco los colores de la GUI
     for (u32 i=0; i<gui::EGDC_COUNT ; ++i)
     {
         video::SColor  col = sskin->getColor((gui::EGUI_DEFAULT_COLOR)i);
         col.setAlpha(150);
         sskin->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
     }

    sskin->setColor( gui::EGDC_BUTTON_TEXT,video::SColor(255,255,255,255) );
    
    this->env->setSkin( sskin );


 }
void Grafico::GUI::MsgBox(const char* msg ,GUI_BOTONES_OK idMsg)
{
    std::wstringstream wsstring;
    wsstring << msg;
    this->env->addMessageBox(L"Quassimodo dice:", wsstring.str().c_str(),
                                    true, gui::EMBF_OK,0, idMsg);
}
 void Grafico::GUI::AgntVSMkn(std::vector<std::string> lista){
     this->dibujaSelector(lista);
 }
std::string Grafico::GUI::getAgenteSeleccionado()
{
    wchar_t* m = (wchar_t*)this->combo->getItem(this->combo->getSelected());
    boost::filesystem::wpath w_path(m);

    return std::string(w_path.string().begin(), w_path.string().end());
}

 
void Grafico::GUI::dropAvsA(){
    if(this->AvsA!=0){
        this->AvsA->remove();
        this->AvsA=0;
    }
        
}

void Grafico::GUI::dibujaSelector(std::vector<std::string> lista){

      core::dimension2d<unsigned int> S_S=core::dimension2d<unsigned int>(500,240);
      int d_a=(this->smgr->getVideoDriver()->getScreenSize().Width-S_S.Width)/2, d_al=(this->smgr->getVideoDriver()->getScreenSize().Height-S_S.Height)/2;
      core::rect<s32> recW=core::rect<s32>(d_a, d_al , S_S.Width+d_a ,S_S.Height+d_al);

      AvsA= this->env->addWindow(recW,true,L"Agente:");

      this->env->addStaticText( L"Selecciona el Agente con el que competirás", core::rect<s32>(35,35,500,60), false, false, AvsA );

      this->combo=this->env->addComboBox( core::rect<s32>((recW.getWidth()/2)-85,(recW.getHeight()/2)-20,(recW.getWidth()/2)+85,(recW.getHeight()/2)+20) ,\
               AvsA);
      this->setOpcionesCombo(lista);
       //botones de Opcion
       this->env->addButton( core::rect<s32>( recW.getWidth()-350,recW.getHeight()-60 ,recW.getWidth()-190,recW.getHeight()-20) ,\
               AvsA, BO_CANCELA, L"Cancelar");
       this->env->addButton( core::rect<s32>( recW.getWidth()-160,recW.getHeight()-60 ,recW.getWidth()-20,recW.getHeight()-20 ),\
               AvsA, BO_INICIA, L"Inicia");

}

void Grafico::GUI::setBotonesPartida(){
    
      int dis_ancho=20,dis_alto=20;
      float size=50,dan=dis_ancho;
      for(int i=0;i<BP_COUNT;i++){
          
              this->botonPartida.at(i)= this->env->addButton(core::rect<s32>( dan,dis_alto,dan+size, dis_alto+size ),0,i+1000,L"") ;
              dan+=size+10;
              this->botonPartida.at(i)->setDrawBorder(true);
              this->botonPartida.at(i)->setImage( this->skin->getBotonPartida(i) );
              this->botonPartida.at(i)->setPressedImage( this->skin->getBotonPartidaPres(i) );
              this->botonPartida.at(i)->setUseAlphaChannel(true);
      }
      this->botonPartida.at(BP_COUNT-1)->setToolTipText(GUI_BOTONES_TOLTIP_PARTIDA[0]);
      botonesPartida=true;
    
}
void Grafico::GUI::dropBotonesPartida(){
    if(botonesPartida){
          for(int i=0;i<BP_COUNT;i++){
            this->botonPartida.at(i)->remove();
            this->botonPartida.at(i)=0;
          }
           botonesPartida=false;
    }

}

void Grafico::GUI::Pausar(bool pausa){
    int ultpos=this->botonPartida.size();
    if(pausa){
        this->botonPartida.at( ultpos-2 )->setImage( this->skin->getBotonPartida( ultpos) ) ;
        this->botonPartida.at( ultpos-2 )->setPressedImage( this->skin->getBotonPartidaPres( ultpos ) );
        this->setTextPausa();
    }
    else{
        this->botonPartida.at( ultpos-2 )->setImage( this->skin->getBotonPartida( ultpos-2 ) );
        this->botonPartida.at( ultpos-2 )->setPressedImage( this->skin->getBotonPartidaPres( ultpos-2 ) );
        this->dropTextPausa();
    }
    this->botonPartida.at(ultpos-2 )->setUseAlphaChannel(true);
    this->botonPartida.at(ultpos-2 )->setDrawBorder(true);
}

void Grafico::GUI::setTextPausa(){
    core::dimension2d<unsigned int> S_S=core::dimension2d<unsigned int>(100,50);
      int d_a=(this->smgr->getVideoDriver()->getScreenSize().Width-S_S.Width)/2, d_al=(this->smgr->getVideoDriver()->getScreenSize().Height-S_S.Height)/2;
      core::rect<s32> rec=core::rect<s32>(d_a, d_al , S_S.Width+d_a ,S_S.Height+d_al);

    core::dimension2d<unsigned int> SS=this->smgr->getVideoDriver()->getScreenSize();
    this->T_Pausa=this->env->addStaticText(L"PAUSA",rec);
    this->T_Pausa->setOverrideFont(this->skin->getMenuBoton());
    this->T_Pausa->setOverrideColor(video::SColor(255,130,0,0)); 
}

void Grafico::GUI::dropTextPausa(){

    if(this->T_Pausa!=NULL||this->T_Pausa!=0){
        this->T_Pausa->remove();
        this->T_Pausa=NULL;
    }

}

void Grafico::GUI::setOpcionesCombo(std::vector<std::string> lista){

    for(int i=0;i<lista.size();i++){
        if(lista.at(i) !="Agente Humano"){
        std::wstringstream wsstream;
        wsstream << lista.at(i).c_str();
        this->combo->addItem(wsstream.str().c_str(),i+1);}
    }

}

void Grafico::GUI::setCreditos(){

    core::dimension2d<u32> S_S= this->skin->getImagenCreditos()->getSize();

    int d_a=(this->smgr->getVideoDriver()->getScreenSize().Width-S_S.Width)/2, d_al=(this->smgr->getVideoDriver()->getScreenSize().Height-S_S.Height)/2;
    this->creditos=this->env->addImage(this->skin->getImagenCreditos(),core::position2d<s32>(d_a,d_al),true,0);
    this->env->addButton(core::rect<s32>( 815,645,905, 675 ),this->creditos,BO_CREDITOS,L"OK") ;

}

void Grafico::GUI::dropCreditos(){

    this->creditos->remove();
}
void Grafico::GUI::setBotonesJugador(int idjugador){
    
    if(this->botonJugador)
        return;
    this->idJugador=idjugador;
    core::dimension2d<u32> siz=this->smgr->getVideoDriver()->getScreenSize();
     int dis_ancho=20,dis_alto=20;
      float size=100,dan=dis_ancho;
      for(int i=0;i<2;i++){
          int aum;
          if(this->idJugador==0) aum=0;else aum=4;
              this->botonesJugador.at(i)= this->env->addButton(core::rect<s32>( siz.Width-(dan+size), siz.Height-(dis_alto+size),siz.Width-dan,siz.Height-dis_alto ) , 0,i+300,L"") ;
              dan+=size+15;
              //this->botonesJugador.at(i)->setDrawBorder(true);
              this->botonesJugador.at(i)->setImage( this->skin->getBotonJugador(i+aum) );
              this->botonesJugador.at(i)->setPressedImage( this->skin->getBotonJugadorPres(i+aum) );
              this->botonesJugador.at(i)->setUseAlphaChannel(true);
              this->botonesJugador.at(i)->setToolTipText(GUI_BOTONES_TOLTIP_JUGADOR[i]);
      }
      this->botonJugador=true;
}
void Grafico::GUI::dropBotonesJugador(){

 if(botonJugador){
          for(int i=0;i<2;i++){
            this->botonesJugador.at(i)->remove();
            this->botonesJugador.at(i)=0;
          }
          botonJugador=false;
    }
}
void Grafico::GUI::setBotonesBarrera(){
    int a=0;
    if(this->idJugador==0) a=0;else a=4;
    this->botonesJugador.at(0)->setImage( this->skin->getBotonJugador(a+1) );
    this->botonesJugador.at(0)->setPressedImage( this->skin->getBotonJugadorPres(a+1) );
    this->botonesJugador.at(0)->setID(Grafico::BJ_LISTO);
    this->botonesJugador.at(0)->setToolTipText(GUI_BOTONES_TOLTIP_JUGADOR[4]);
    this->botonesJugador.at(1)->setImage( this->skin->getBotonJugador(a+2) );
    this->botonesJugador.at(1)->setPressedImage( this->skin->getBotonJugadorPres(a+2) );
    this->botonesJugador.at(1)->setID(Grafico::BJ_GIRA_ESTE);
    this->botonesJugador.at(1)->setToolTipText(GUI_BOTONES_TOLTIP_JUGADOR[2]);

}
void Grafico::GUI::cambiaGiro(bool este){
    int a,aum;
    if(this->idJugador==0) a=0;else a=4;
    if(este){
        this->botonesJugador.at(1)->setID(Grafico::BJ_GIRA_NORTE);
        aum=3;
    }
    else{
        this->botonesJugador.at(1)->setID(Grafico::BJ_GIRA_ESTE);

        aum=2;
    }
    this->botonesJugador.at(1)->setImage( this->skin->getBotonJugador(a+aum) );
    this->botonesJugador.at(1)->setPressedImage( this->skin->getBotonJugadorPres(a+aum) );
    this->botonesJugador.at(1)->setToolTipText(GUI_BOTONES_TOLTIP_JUGADOR[aum]);

}
void Grafico::GUI::setAyuda(){
    core::dimension2d<u32> S_S= this->skin->getImagenCreditos()->getSize();

    int d_a=(this->smgr->getVideoDriver()->getScreenSize().Width-S_S.Width)/2, d_al=(this->smgr->getVideoDriver()->getScreenSize().Height-S_S.Height)/2;
    this->ayuda=this->env->addImage(this->skin->getImagenAyuda(),core::position2d<s32>(d_a,d_al),true,0);
    this->env->addButton(core::rect<s32>( 815,705,905, 735 ),this->ayuda,BOK_AYUDA,L"OK") ;
}

void Grafico::GUI::dropAyuda(){
    this->ayuda->remove();
}
void Grafico::GUI::setPensando(){

        core::dimension2d<unsigned int> S_S=core::dimension2d<unsigned int>(180,60);
      int d_a=(this->smgr->getVideoDriver()->getScreenSize().Width-S_S.Width)/2, d_al=(this->smgr->getVideoDriver()->getScreenSize().Height-S_S.Height)/2;
      core::rect<s32> rec=core::rect<s32>(d_a, d_al , S_S.Width+d_a ,S_S.Height+d_al);

    core::dimension2d<unsigned int> SS=this->smgr->getVideoDriver()->getScreenSize();
    this->T_Pensando=this->env->addStaticText(L"*Pensando*",rec);
    this->T_Pensando->setOverrideFont(this->skin->getMenuBoton());
    this->T_Pensando->setOverrideColor(video::SColor(255,130,0,0));
}

void Grafico::GUI::dropPensando(){
      if(this->T_Pensando!=NULL||this->T_Pensando!=0){
        this->T_Pensando->remove();
        this->T_Pensando=NULL;
    }

}