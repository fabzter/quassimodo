

#ifndef __TOKAYO_CAMERA_HPP__
#define __TOKAYO_CAMERA_HPP__

#include <irrlicht/ISceneNodeAnimatorCameraMaya.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>
#include<irrlicht/ISceneNode.h>
#include<irrlicht/ICameraSceneNode.h>
#include<irrlicht/SViewFrustum.h>
#include <stdio.h>
#include"IAnimatorCameraTokayo.hpp"
#include<iostream>
using namespace irr;
namespace Grafico{
/**
 *Clase que implementa la interfaz de de animacion de la camara
 * @sa IAnimatorCameraTokayo
 * se ocupa de la siguiente forma:
 * @code
 * scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
 *scene::ISceneNodeAnimator* anm = new TokayoCamera();
 *cam->addAnimator(anm);
 *anm->drop();
 * @endcode
 */
class TokayoCamera : public IAnimatorCameraTokayo 
{
public:
        /**
         *Constructor de el animador de la camara
         * @param rotateSpeed velocidad de rotacion, por default 3.0
         * @param zoomSpeed velocidad del zoom de la camara, por default 3.0
         * @param translationSpeed velocidad de translacion de la camara por default 3.0
         */
        TokayoCamera(f32 rotateSpeed = 3.0f,f32 zoomSpeed = 3.0f,f32 translationSpeed = 3.0f);

        /**
         *Destructor
         */
        virtual ~TokayoCamera();

        /**
         *Anima el nodo escena!, actualmente solo funciona en camaras
         * @param node
         * @param timeMs
         */
        virtual void animateNode(scene::ISceneNode* node, u32 timeMs);

        /**
         *Recibidor de eventos
         * @param event el evento recibido
         */
        virtual bool OnEvent(const SEvent& event);

        /**
         * @sa IAnimatorCameraTokayo::getRotateSpeed
         */
        virtual f32 getRotateSpeed() const;

        /**
         * @sa IAnimatorCameraTokayo::setRotateSpeed
         */
        virtual void setRotateSpeed(f32 rotateSpeed);

        /**
         * @sa IAnimatorCameraTokayo::getZoomSpeed
         */
        virtual f32 getZoomSpeed() const;

        /**
         * @sa IAnimatorCameraTokayo::setZoomSpeed
         */
        virtual void setZoomSpeed(f32 zoomSpeed);
        /**
         * @sa IAnimatorCameraTokayo::getMoveSpeed
         */
        virtual f32 getMoveSpeed() const ;
        /**
         * @sa IAnimatorCameraTokayo::setMoveSpeed
         */
        virtual void setMoveSpeed(f32 moveSpeed);
        /**
         * @sa IAnimatorCameraTokayo::setRotationNumbers
         */
        virtual void setRotationNumbers(f32 RotX, f32 RotY) ;
        /**
         * @sa IAnimatorCameraTokayo::getRotationNumX
         */
        virtual f32 getRotationNumX()const ;
        /**
         * @sa IAnimatorCameraTokayo::getRotationNumY
         */
        virtual f32 getRotationNumY()const ;
        /**
         * @sa IAnimatorCameraTokayo::setZoom
         */
        virtual void setZoom(f32 Zoom) ;
        /**
         * @sa IAnimatorCameraTokayo::getZoom
         */
        virtual f32 getZoom() const ;

        /**
         *indica si este Animador recibirá los eventos una vez que este sea adjuntado a la camara
         * @return true
         */
        virtual bool isEventReceiverEnabled() const
        {
                return true;
        }

        /**
         *Regresa el tipo de Animador que es.
         * @return scene::ESNAT_CAMERA_MAYA pues se comporta de la misma manera que la camara maya de irrlicht.
         */
        virtual scene::ESCENE_NODE_ANIMATOR_TYPE getType() const
        {
                return scene::ESNAT_CAMERA_MAYA;
        }

        /**
         *Crea un clone de este animador puede pasarle ambos parametros en NULL
         * @returnun clone de este Animador
         */
        virtual scene::ISceneNodeAnimator* createClone(scene::ISceneNode* node, scene::ISceneManager* newManager=0);


private:
        /**
         *Coloca todos los botones del mouse  como no presionados
         */
        void allKeysUp();
        /**
         *indica si algun boton del mouse ha sido presionado
         * @param key el boton del mouse que se quiere saber su estado
         * @return true si fue presionado, false en caso contrario
         */
        bool isMouseKeyDown(s32 key);
        /**
         * arreglo que representa los botones del mouse
         */
        bool MouseKeys[3];
        /**
         * vector de tres dimensiones que indica la posicion actual de la camara
         */
        core::vector3df Pos;

        /**
         * bandera que indica si al presionar el boton del zoom hubo zoom o no.
         */
        bool Zooming;
        /**
         * bandera que indica si al presionar el boton de rotacion hubo rotacion o no.
         */
        bool Rotating;
        /**
         * bandera que indica si al presionar el boton de traslacion hubo traslacion o no.
         */
        bool Translating;
        /**
         * flotante que indica la velocidad del zoom
         */
        f32 ZoomSpeed;
        /**
         * flotante que indica la velocidad de rotacion
         */
        f32 RotateSpeed;
        /**
         * flotante que indica la velocidad de traslacion
         */
        f32 TranslateSpeed;
        /**
         * vector de dimension dos que indica los grados de rotacion cuando se presiono el boton de la acción
         */
        core::position2df RotateStart;
        /**
         * vector que indica el zoom que tenia  la camara cuando se presiono el boton de la acción
         */
        core::position2df ZoomStart;
        /**
         * vector de dos dimensiones que indica la traslacion que tenia la camara cuando se presiono el boton de la acción
         */
        core::position2df TranslateStart;
        /**
         * vector de 3 dimensiones que indica el viejo objetivo
         */
        core::vector3df OldTarget;
        /**
         * flotante que marca el zoom actual
         */
        f32 CurrentZoom;
        /**
         * flotantes que indican el grado de rotacion que tendrá la camara en el eje de las X y de las Y
         */
        f32 RotX, RotY;
        /**
         * vector de dimension 2 que marca la posición del mouse
         */
        core::position2df MousePos;
        /**
         * Un nodo camara.
         */
        scene::ICameraSceneNode* OldCamera;

};

}

#endif


