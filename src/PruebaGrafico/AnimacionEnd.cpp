/* 
 * File:   AnimacionEnd.cpp
 * Author: tokayo
 * 
 * Created on 4 de mayo de 2010, 11:09
 */

#include "AnimacionEnd.hpp"

using namespace irr;

AnimacionEnd::AnimacionEnd(PartidaGrafica* partida,scene::ISceneManager* smgr) {
    this->partida=partida;
    this->smgr=smgr;
}

AnimacionEnd::AnimacionEnd(const AnimacionEnd& orig){
}

AnimacionEnd::~AnimacionEnd(){
}
void AnimacionEnd::OnAnimationEnd(scene::IAnimatedMeshSceneNode* node){
    std::cout<<"Llego"<<std::endl;
    this->partida->siguienteJugada();

}

