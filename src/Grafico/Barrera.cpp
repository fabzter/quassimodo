

#include "Barrera.hpp"

using namespace irr;

Grafico::Barrera:: Barrera(scene::ISceneManager* smgr,Skin* skin,int VelAnim,scene::ISceneNode* parent):Grafico::Pieza(parent),Reglas::Barrera() {

      this->mesh=skin->getBarrera();
     this->dibuja(smgr);
     {
         video::ITexture* tex=skin->getTBarrera();
         this->nodoA->forEachMaterial([tex](video::SMaterial& m){ m.setTexture(0, tex); });
     }
     this->nodoA->getMaterial(0).SpecularColor.set(0,0,0,0);
     this->velAnim=VelAnim;
}

Grafico::Barrera::Barrera(const Barrera& orig):Grafico::Pieza(orig),Reglas::Barrera(orig) {
}

Grafico::Barrera::~Barrera() {
}

core::vector3df Grafico::Barrera::giraEste(core::vector3df pos){

    if(this->nodoA->getRotation().Y==0){
        this->nodoA->setRotation(core::vector3df(0,90,0));
        core::vector3df p=pos;
        p.X+=this->size.X*this->getEscala().X;
        return p;
    }
    // D1-SIGTRAP class fix: non-void fn must return on all paths (clang brk #0x1).
    // If already rotated, leave the position unchanged.
    return pos;
}

void Grafico::Barrera::giraNorte(){
    if(this->nodoA->getRotation().Y==90){
        this->nodoA->setRotation(core::vector3df(0,0,0));
        core::vector3df p=this->getPosicionEscena();
         p.X-=this->size.X*this->getEscala().X;
        this->setPosicion(p);
    }
}

void Grafico::Barrera::ColocaBarrera(irr::core::vector3df posg, const std::vector<int>& pos, Reglas::Direccion dir,scene::ISceneManager* smgr){

    core::vector3df pos_final=core::vector3df( posg.X-(this->getSize().X*this->getEscala().X) , posg.Y, posg.Z );
    if(dir==Reglas::ESTE)
        pos_final=this->giraEste(pos_final);

    // D2.1 KB-D2-004: animator subsystem removed from IrrlichtMt fork.
    // Instant placement replaces the createFlyStraightAnimator drop animation.
    this->setPosicion(pos_final);
    this->posiciong=pos_final;
    this->colocar(pos,dir);
}

bool Grafico::Barrera::endAnimacion(){
    // D2.1: no pending animation with instant moves.
    return true;
}
