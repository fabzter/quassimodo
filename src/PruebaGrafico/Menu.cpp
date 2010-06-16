

#include "Menu.hpp"
using namespace irr;
using namespace Grafico;

Menu::Menu(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Skin* skin) {
    this->env=env;
    this->smgr=smgr;
    this->skin=skin;
    this->t=t;
    this->escala.X=1 ,this->escala.Y=1, this->escala.Z=1;
    this->jugadores.reserve(2);
    this->botones.reserve(B_COUNT);

}

Menu::Menu(const Menu& orig) {
}

Menu::~Menu() {
    this->dropMenuP();
}

 void Menu::SetJugadores(){

    for(int i=0;i<this->t->num_jugadores;i++){
         this->jugadores.push_back(new Grafico::Pieza());
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
      this->escala.X=x ,this->escala.Y=y, this->escala.Z=z;
     for(int i=0;i<this->t->num_jugadores;i++){
         this->jugadores.at(i)->setEscala(x,y,z);
     }
      this->setPosicionJugadores();
 }

 void Menu::setMenuP(){

     haymenu=true;
     this->SetJugadores();
     this->setBotones();
     this->setSkin();
     this->SetEscala( this->escala.X ,this->escala.Y, this->escala.Z);
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
 void Menu::setSkin(){

     for(int i=0;i<B_COUNT;i++){
            this->botones.at(i)->setOverrideFont(this->skin->getMenuBoton());

      }

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
  char Menu::MenuConsola(){
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
  
 
