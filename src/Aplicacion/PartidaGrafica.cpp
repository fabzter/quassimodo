
#include <irrlicht/vector3d.h>

#include "PartidaGrafica.hpp"
using namespace irr;
using namespace Grafico;


PartidaGrafica::PartidaGrafica(scene::ISceneManager* smgr,Grafico::Skin* skin,gui::IGUIEnvironment* env,int VelAnim) {
    this->juez = NULL;
    this->b_tmp=NULL;
    this->jugadores.reserve(2);
    this->Resaltado_celda=this->en_curso = this->hay_ganador = false;
    this->jugador_ganador = this->jugador_en_turno = 0;
    this->skin=skin;
    this->smgr=smgr;
    this->env=env;
    this->escala.X=1,this->escala.Y=1,this->escala.Z=1;
    this->antorchas.reserve(4);
    this->antorchas.resize(4);
    this->Barreras.reserve(20);
    this->parent=smgr->addEmptySceneNode();
    this->parent->setPosition(core::vector3df(0,0,0));
    this->parent->setName("PARENTPG");
    this->t=new Tablero(smgr,this->skin,this->parent);
    this->juez = new Reglas::Juez(*t);
    this->velAnim=VelAnim;
    this->ManAgentes=new Agentes::ManejadorAgentes();
    for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)=new Antorcha(smgr,this->parent);
    }
     this->ColocaAntorchas();

}

PartidaGrafica::PartidaGrafica(const PartidaGrafica& orig) {
}

PartidaGrafica::~PartidaGrafica() {
      delete this->juez;
     this->dropJugadores();
     for(int i=0;i<this->antorchas.size();i++){
        delete this->antorchas.at(i);
     }
     this->dropBarreras();
     delete(this->t);
     this->parent->removeAll();
     this->parent->remove();
     delete(this->ManAgentes);
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
    this->dropOpcionesMover();
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

    if(this->en_curso)
        ju->Mover( smgr,p,this->t->getCeldasAMover( this->jugadores.at(idJugador)->getPosicion(),j.getPosicion() ) );
    else
        ju->setPosicion(p);
    this->t->moverJugador(idJugador, j.getPosicion());
    return true;
}

 void PartidaGrafica::SetBarrera(Reglas::Jugada &j, int idJugador){

     this->Barreras.push_back( new Barrera(smgr,this->skin,this->velAnim,this->t->getNodo(),this->jugador_en_turno ) );
        unsigned int pos=this->Barreras.size();
        const std::vector<int> p=j.getPosicion();
        this->Barreras.at(pos-1)->ColocaBarrera( this->t->getPosicionCelda( p ),p,j.getDireccion(),this->smgr );
        this->t->setBarrera(idJugador, *this->Barreras.at(pos-1));

 }

 void PartidaGrafica::ColocaAntorchas(){


     int x,x1,z,z1,y,aumento=50;
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
      this->parent->setScale(this->escala);
     this->t->setEscalaTablero(this->escala.X,this->escala.Y,this->escala.Z);
      for(int i=0;i<this->jugadores.size();i++){
          Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
          ju->setEscala(this->escala.X,this->escala.Y,this->escala.Z);
      }
      for(std::size_t i = 0; i < this->antorchas.size(); i++){
         this->antorchas.at(i)->setEscalaAntorcha(this->escala.X,this->escala.Y,this->escala.Z);
    }

 }

 bool PartidaGrafica::SetJugadores(std::string rutaAgente1,std::string rutaAgente2,bool ambosHumanos){

    this->jugadores.push_back(new Grafico::Jugador(smgr,0,this->ManAgentes->makeAgente(rutaAgente1)  ,this->skin,this->velAnim,this->parent,true));

     if(rutaAgente2!="Agente Humano")
         this->jugadores.push_back(new Grafico::Jugador(smgr,1,new AgenteThreads( this->ManAgentes->makeAgente(rutaAgente2) ),this->skin,this->velAnim,this->parent,ambosHumanos));
     else
         this->jugadores.push_back(new Grafico::Jugador(smgr,1,this->ManAgentes->makeAgente(rutaAgente2) ,this->skin,this->velAnim,this->parent,ambosHumanos));

     this->t->setJugadores( this->jugadores);
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

std::vector<std::string> PartidaGrafica::getNombresAgentes(){
    return this->ManAgentes->getNombresAgentes();
}

void PartidaGrafica::dropBarreras(){

     for(int i=0;i<this->Barreras.size();i++){
         delete( this->Barreras.at(i) );
     }
     this->Barreras.clear();
 }

void PartidaGrafica::dropJugadores() {
     for(int i=0;i<this->jugadores.size();i++){
            Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(i);
            delete(ju);
      }
     this->jugadores.clear();
 }


bool PartidaGrafica::estaEnCurso()
{
    return this->en_curso;
}

bool PartidaGrafica::hayGanador()
{
    return this->hay_ganador;
}
int PartidaGrafica::getJugadorGanador(){

    return this->jugador_ganador;
}

 char PartidaGrafica::getAgenteConError(){
     return this->errorEnAgente;
 }

int PartidaGrafica::getJugadorEnTurno(){
    return this->jugador_en_turno;
}

bool PartidaGrafica::JugadorIsHumano(int idJugador){

    Grafico::Jugador *j= (Grafico::Jugador*)&(this->t->getJugador(idJugador));
    return j->IsHumano();

}
void PartidaGrafica::setOpcionesMover(){
    if(this->Resaltado_celda)
        return;
    Reglas::AyudanteDeAgente ayudante(this->t);
    std::list<Reglas::Jugada> mov=ayudante.getMovimientosPosibles(this->jugador_en_turno);

    for( std::list<Reglas::Jugada>::iterator i=mov.begin();i!=mov.end();i++ ){
        Grafico::Celda* c=this->t->getCeldaGrafica( (*i).getPosicion() );
        c->ResaltarCelda();
    }
    this->Resaltado_celda=true;

}
void PartidaGrafica::dropOpcionesMover(){
    if(!this->Resaltado_celda)
        return;
     Reglas::AyudanteDeAgente ayudante(this->t);
    std::list<Reglas::Jugada> mov=ayudante.getMovimientosPosibles(this->jugador_en_turno);

    for( std::list<Reglas::Jugada>::iterator i=mov.begin();i!=mov.end();i++ ){
        Grafico::Celda* c=this->t->getCeldaGrafica( (*i).getPosicion() );
        c->dropResaltado();
    }
    this->Resaltado_celda=false;
}
bool PartidaGrafica::Siguiente(Reglas::Tablero *t){

     if(!this->en_curso)
        throw Reglas::PartidaTerminada();
    this->en_curso = false; //si todo sale bien, la regresamos a en_curso = true
    //pedimos la Jugada y enviamos excepciones
    Reglas::Jugada j= this->juez->siguienteJugada(this->jugador_en_turno);
   this->en_curso = true;

    this->actualizarTablero(j, this->jugador_en_turno);
    //actualizamos el Jugador en turno.
    this->jugador_en_turno =
            ++(this->jugador_en_turno) % t->num_jugadores;

    //Actualizamos Banderas:
    int idGanador = this->juez->hayGanador();
    if(idGanador >= 0)
    {
        this->hay_ganador = true;
        this->jugador_ganador = idGanador+1;
        this->en_curso = false;
    }
    else{
         Grafico::Jugador *j= (Grafico::Jugador*)&(this->t->getJugador(this->jugador_en_turno));
         j->setHaciendoJugada(true);
    }
    return this->en_curso;
}
int PartidaGrafica::ChecaJugada(core::position2d<s32>& pos,bool movimiento,bool barrera,int Direccion){
    scene::ISceneNode* n=this->smgr->getSceneCollisionManager()-> getSceneNodeFromScreenCoordinatesBB(pos,0,true,this->t->getNodo())   ;
    if(n!=0 && (movimiento||barrera) ){
        std::string nom=n->getName();
        if(nom=="CELDA"){
            Reglas::Jugada j=this->ArmaJugada(n->getID(),movimiento,barrera,Direccion);
            Reglas::AyudanteDeAgente ayudante(this->t);
            std::list<Reglas::Jugada>::iterator i;
            if(movimiento){
                std::list<Reglas::Jugada> mov=ayudante.getMovimientosPosibles(this->jugador_en_turno);
                i=std::find(mov.begin(),mov.end(),j);
                if( (*i)==j )
                    return n->getID();
                else
                    return -1;
            }
            else if(barrera){
                try{
                    ayudante.revisarReglas(j,this->jugador_en_turno);
                }
                catch(Reglas::Excepcion &e){
                    return -1;
                }
                return n->getID();
            }
        }
    }
        return -1;

}
void PartidaGrafica::setJugada(int celda,bool movimiento,bool barrera,int Direccion){
    if(this->b_tmp!=NULL){
        delete (this->b_tmp);
        this->b_tmp=NULL;
    }
    Reglas::Jugada j=this->ArmaJugada(celda,movimiento,barrera,Direccion);
    Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(this->jugador_en_turno);
    ju->setJugada(j);
}
bool PartidaGrafica::HaciendoJugada(){
    Grafico::Jugador *ju=(Grafico::Jugador*)this->jugadores.at(this->jugador_en_turno);
    if(ju->IsHumano())
        return ju->IsHaciendoJugada();
    AgenteThreads *a=(AgenteThreads *)ju->getAgente();
    this->ayu=new Reglas::AyudanteDeAgente(this->t);
    a->run(this->t, *ayu);
    ju->setHaciendoJugada( !a->estaListaJugada() );
    return ju->IsHaciendoJugada();

}
void PartidaGrafica::setBarreraT(int celda,int Direccion){
    if(this->b_tmp!=NULL)
        delete this->b_tmp;
    
    Reglas::Jugada j=this->ArmaJugada(celda,false,true,Direccion);

    this->b_tmp=new Barrera(smgr,this->skin,this->velAnim,this->t->getNodo(),this->jugador_en_turno);
    const std::vector<int> p=j.getPosicion();
    this->b_tmp->ColocaBarrera( this->t->getPosicionCelda( p ),p,j.getDireccion(),this->smgr,false );

}
Reglas::Jugada PartidaGrafica::ArmaJugada(int celda,bool movimiento,bool barrera ,int direccion){
    int x=celda/10, y=celda%10;
    Reglas::Jugada j;
    j.setPosicion(x,y);
    if(movimiento){
        j.setTipoDeJugada(Reglas::MOVIMIENTO);
    }
    else if(barrera){
        j.setTipoDeJugada(Reglas::BARRERA);
        if(direccion==0){
            j.setDireccion(Reglas::NORTE);}
        else{
            j.setDireccion(Reglas::ESTE);}
    }

    return j;
}
void PartidaGrafica::cambiaGiro(bool este){
    if(this->b_tmp==NULL)
        return;
    if(este)
        this->b_tmp->giraEste(this->b_tmp->getPosicionEscena());
    else
        this->b_tmp->giraNorte();
}