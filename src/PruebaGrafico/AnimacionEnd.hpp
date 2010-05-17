
#ifndef _ANIMACIONEND_HPP
#define	_ANIMACIONEND_HPP
#include "Partida.hpp"

using namespace irr;
class AnimacionEnd:public scene::IAnimationEndCallBack {
public:
    AnimacionEnd(Partida *partida,scene::ISceneManager* smgr);
    AnimacionEnd(const AnimacionEnd& orig);
    virtual ~AnimacionEnd();
    void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node);
private:
    Partida *partida;
    scene::ISceneManager* smgr;

};

#endif	/* _ANIMACIONEND_HPP */

