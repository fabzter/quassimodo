
#include <irrlicht/vector3d.h>

#include "PartidaGrafica.hpp"
using namespace irr;
using namespace Grafico;


PartidaGrafica::PartidaGrafica(scene::ISceneManager* smgr,Grafico::Skin* skin,gui::IGUIEnvironment* env,int VelAnim): Partida() {
    this->skin=skin;
    this->smgr=smgr;
    this->env=env;
    this->escala.X=1,this->escala.Y=1,this->escala.Z=1;
    this->antorchas.reserve(4);
    this->antorchas.resize(4);
    this->Barreras.reserve(20);
    this->t=new Tablero(smgr,this->skin);
    this->juez = new Reglas::Juez(*t);
    this->velAnim=VelAnim;
    for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)=new Antorcha(smgr,0,0,this->skin);
    }
    
     core::vector3df v= this->t->getSize();
     this->t->setPosicionTablero(-(v.X) /2,-15,-( (v.Z) /2));
     this->ColocaAntorchas();

}

PartidaGrafica::PartidaGrafica(const PartidaGrafica& orig) {
}

PartidaGrafica::~PartidaGrafica() {

     this->dropJugadores();
     for(int i=0;i<this->antorchas.size();i++){
        delete this->antorchas.at(i);
     }
     this->dropBarreras();
     delete(this->t);
     //this->triangle->drop();
}

void PartidaGrafica::iniciarPartida()
{
    if(this->en_curso)
        throw Reglas::PartidaNoIniciada();

    for(int id = 0; id < this->t->num_jugadores; id++)
    {
        Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(id);
        this->t->getJugador(id).iniciar(id);
        Reglas::Jugada j;
        j.setPosicion( this->t->getJugador(id).getPosicion() );
        this->MoverJugador(j,id);
        ju->Gira(core::vector3df(0,90-(id*180),0));

    }

    this->en_curso = true;
}

bool PartidaGrafica::siguienteJugada()
{
    return this->Siguiente(this->t);
}

void PartidaGrafica::actualizarTablero(Reglas::Jugada &j, int idJugador)
{
    if(j.getTipoDeJugada() == Reglas::MOVIMIENTO)
    {
        this->MoverJugador(j,idJugador);
    }
    else if(j.getTipoDeJugada() == Reglas::BARRERA)
    {
        this->SetBarrera(j,idJugador);
    }
}

bool PartidaGrafica::MoverJugador(Reglas::Jugada &j, int idJugador){
    Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(idJugador);
    core::vector3df p=this->t->getPosicionCelda(  j.getPosicion() )  ;
    p.Y+=this->t->getsizeCelda().Y*this->escala.Y;
    p.Z+=( ( this->t->getsizeCelda().Z*this->escala.Z ) /2);
    p.X+=( ( this->t->getsizeCelda().X*this->escala.X ) /2);
    bool termino=true;
    if(this->en_curso)
        termino=ju->Mover(smgr,p);
    else
        ju->setPosicion(p);
    this->t->moverJugador(idJugador, j.getPosicion());
    return termino;
}

 void PartidaGrafica::SetBarrera(Reglas::Jugada &j, int idJugador){

     this->Barreras.push_back(new Barrera(smgr,this->skin,this->velAnim));
        unsigned int pos=this->Barreras.size();
        const std::vector<int> p=j.getPosicion();
        this->Barreras.at(pos-1)->setEscala(this->escala.X,this->escala.Y,this->escala.Z);
        this->Barreras.at(pos-1)->ColocaBarrera( this->t->getPosicionCelda( p ),p,j.getDireccion(),this->smgr );
        this->t->setBarrera(idJugador, *this->Barreras.at(pos-1));
        this->setTopeSombra(pos-1);

 }

 void PartidaGrafica::ColocaAntorchas(){

      int x,x1,z,z1,y,aumento=5;
     core::vector3df v=this->t->getPosicionCelda(0,0);
    x=v.X+(-aumento-this->t->getEscala().X);
    x1=(aumento+this->t->getsizeLineaCeldas().X);
    z=v.Z+(-aumento-this->t->getEscala().Z);
    z1=(aumento+this->t->getsizeLineaCeldas().Z);
    y=v.Y+(this->t->getEscala().Y);
     this->antorchas.at(0)->setPosicionAntorcha(x,y,z);
     this->antorchas.at(1)->setPosicionAntorcha(x1,y,z);
     this->antorchas.at(2)->setPosicionAntorcha(x1,y,z1);
     this->antorchas.at(3)->setPosicionAntorcha(x,y,z1);
 }

 void PartidaGrafica::SetEscala(int x, int y, int z){
      this->escala.X=x,this->escala.Y=y,this->escala.Z=z;
     this->t->setEscalaTablero(this->escala.X,this->escala.Y,this->escala.Z);
      for(int i=0;i<this->jugadores.size();i++){
          Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
          ju->setEscala(this->escala.X,this->escala.Y,this->escala.Z);
      }

      for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)->setEscalaAntorcha(this->escala.X,this->escala.Y,this->escala.Z);
    }
       core::vector3df v= this->t->getSize();
     this->t->setPosicionTablero(-(v.X*x) /2,-13.3*y,-( (v.Z*z) /2));

     this->ColocaAntorchas();
 }

 bool PartidaGrafica::SetJugadores(std::string rutaAgente1,std::string rutaAgente2){
     


      std::vector<Reglas::Agente*> agentes=this->getAgentes(rutaAgente1,rutaAgente2,this->t);

      this->jugadores.push_back(new Grafico::Jugador(smgr,0, agentes[0],this->skin,this->velAnim));
      this->jugadores.push_back(new Grafico::Jugador(smgr,1, agentes[1],this->skin,this->velAnim));

      this->t->setJugadores( this->jugadores);
      this->SetEscala(this->escala.X,this->escala.Y,this->escala.Z);
 }

core::vector3df PartidaGrafica::getCentro(){
     core::vector3df pos=this->t->getPosicionTablero();
     core::vector3df tam=this->t->getSize()*this->escala;
     core::vector3df cen;
     cen.X=pos.X+(tam.X/2);
     cen.Z=pos.Z+(tam.Z/2);
     cen.Y=pos.Y;
     return cen;
}
bool PartidaGrafica::animacionesEnd(){

    if(this->jugadores.size()<=0){
        return true;
    }
    else{
        Grafico::Jugador *ju0=(Grafico::Jugador*)this->jugadores.at(0);
        Grafico::Jugador *ju1=(Grafico::Jugador*)this->jugadores.at(1);
        bool end=ju0->endAnimacion() & ju1->endAnimacion() ;
        if(this->Barreras.size()<=0){
            return end;
        }
        else{
            return end & this->Barreras.at(this->Barreras.size()-1)->endAnimacion();
        }
    }
}
void PartidaGrafica::dropBarreras(){

     for(int i=0;i<this->Barreras.size();i++){
         std::cout<<"borrando la barrera : "<<i<<" de "<<this->Barreras.size()<<std::endl;
         delete( this->Barreras.at(i) );
     }
     this->Barreras.clear();
 }

void PartidaGrafica::dropJugadores() {
     for(int i=0;i<this->jugadores.size();i++){
         std::cout<<"borrando el jugador: "<<i<<" de "<<this->jugadores.size()<<std::endl;
            Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
            delete(ju);
      }
     this->jugadores.clear();
 }
 void PartidaGrafica::setTopeSombra(int i){


         /*scene::ITriangleSelector* selector= smgr->createTriangleSelectorFromBoundingBox(this->Barreras.at(i)->getNodo());
            this->Barreras.at(i)->getNodo()->setTriangleSelector(selector);
            this->triangle->addTriangleSelector(selector);
            selector->drop();
            for(int j=0;j<this->jugadores.size();j++){
                Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(j);
                if(ju->getNodoSombra()->getAnimators().size()>0)
                    ju->getNodoSombra()->removeAnimators();
                const core::aabbox3d<f32>& box = ju->getNodoSombra()->getBoundingBox();
                core::vector3df radius = box.MaxEdge - box.getCenter();
                // create collision response animator and attach it to the camera
                scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
                        this->triangle,ju->getNodoSombra() ,radius,
                        core::vector3df(0,0,0),
                        core::vector3df(70,0,0));
//
                ju->getNodoSombra()->addAnimator(anim);
                anim->drop();
            }*/

 }
 



