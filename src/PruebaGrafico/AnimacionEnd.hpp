
#ifndef _ANIMACIONEND_HPP
#define	_ANIMACIONEND_HPP
#include "PartidaGrafica.hpp"

using namespace irr;
class AnimacionEnd:public scene::IAnimationEndCallBack {
public:
    AnimacionEnd();
    AnimacionEnd(const AnimacionEnd& orig);
    virtual ~AnimacionEnd();
    void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node);
private:
    PartidaGrafica *partida;
    scene::ISceneManager* smgr;

};

#endif	/* _ANIMACIONEND_HPP */

