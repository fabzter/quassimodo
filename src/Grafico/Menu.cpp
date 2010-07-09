

#include "Menu.hpp"
using namespace irr;
using namespace Grafico;

Grafico::Menu::Menu(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Skin* skin) {
    this->env=env;
    this->smgr=smgr;
    this->skin=skin;
    this->t=t;
    this->escala.X=1 ,this->escala.Y=1, this->escala.Z=1;
    this->jugadores.reserve(2);
    this->botones.reserve(B_COUNT);
    this->haymenu=false;

}

Grafico::Menu::Menu(const Menu& orig) {
    
}

Grafico::Menu::~Menu() {
    this->dropMenuP();
}

 void Grafico::Menu::SetJugadores(){

    for(int i=0;i<this->t->num_jugadores;i++){
         this->jugadores.push_back(new Grafico::Pieza());
         if(i==0)
             this->jugadores.at(i)->setMesh( this->skin->getJugador1() );
         else
             this->jugadores.at(i)->setMesh( this->skin->getJugador2() );

        this->jugadores.at(i)->dibuja( this->smgr );
        this->jugadores.at(i)->setEscala( this->escala.X,this->escala.Y, this->escala.Z);
    }
    this->setPosicionJugadores();

}
 void Grafico::Menu::setPosicionJugadores(){
     
     for(int i=0;i<this->t->num_jugadores;i++){
        core::vector3df p=this->t->getPosicionCelda(4,(i*8));
        p.Y+=this->t->getsizeCelda().Y;
        this->jugadores.at(i)->setPosicion( p );
     }
 }
 void Grafico::Menu::SetEscala(int x,int y,int z){
      this->escala.X=x ,this->escala.Y=y, this->escala.Z=z;

      if(this->menuEnEscena()){

         for(int i=0;i<this->t->num_jugadores;i++){
             this->jugadores.at(i)->setEscala(x,y,z);
         }
          this->setPosicionJugadores();

      }
 }

 void Grafico::Menu::setMenuP(){
     if(!this->menuEnEscena()){
         haymenu=true;
         this->SetJugadores();
         this->setBotones();
         this->setSkin();
         this->SetEscala( this->escala.X ,this->escala.Y, this->escala.Z);
     }
 }
void Grafico::Menu::dropMenuP(){
    if( this->menuEnEscena()){
         for (int i=0;i<B_COUNT;i++){
             this->botones.at(i)->remove();
             this->botones.at(i)=0;
             if(i<2)
                  delete this->jugadores.at(i);
        }
         haymenu=false;
         this->botones.clear();
         this->jugadores.clear();
    }
  
}
 void Grafico::Menu::setSkin(){

     for(int i=0;i<B_COUNT;i++){
            this->botones.at(i)->setOverrideFont(this->skin->getMenuBoton());

      }

 }
 void Grafico::Menu::setBotones(){

      core::dimension2d<unsigned int> Scren_Size=this->smgr->getVideoDriver()->getScreenSize();
      int dis_ancho=Scren_Size.Width/15 ,dis_alto=70;
      core::dimension2d<unsigned int> recBotones=core::vector2d<unsigned int>( 2*(Scren_Size.Width/4), (Scren_Size.Height/2)-70 );
      float t_alto= recBotones.Height/B_COUNT, t_ancho=recBotones.Width;
      int p_alto=50;//Scren_Size.Height/2;
      for(int i=0;i<B_COUNT;i++){
          
            this->botones.push_back( this->env->addButton(core::rect<s32>( dis_ancho, p_alto, t_ancho+dis_ancho, p_alto+t_alto ),0,
              i,GUI_BOTONES_TEXTO[i],GUI_BOTONES_TOLTIP[i]) );
             p_alto+=t_alto;
      this->botones.at(i)->setDrawBorder(false);

      }
    
 }

  char Grafico::Menu::MenuConsola(){
    char op;
    std::cout<<std::endl<<"\t\t¡¡Bienvenido a Quassimodo!!"<<std::endl<<std::endl;
    std::cout<<"Por favor selecciona la letra indicada según la Opcion que desees: "<<std::endl<<std::endl;
    std::cout<<"a) Agente Vs Agente"<<std::endl;
    std::cout<<"b) Agente Vs Maquina"<<std::endl;
    std::cout<<"c) Creditos"<<std::endl;
    std::cout<<"d) Ayuda"<<std::endl;
    std::cout<<"e) Salir"<<std::endl;
    std::cin>>op;
    return op;
  }
  
 bool Grafico::Menu::menuEnEscena(){
     return this->haymenu;
 }
