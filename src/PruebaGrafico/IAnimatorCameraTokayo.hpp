#ifndef __I_ANIMATOR_CAMERA_TOKAYO_HPP__
#define __I_ANIMATOR_CAMERA_TOKAYO_HPP__
#include <irrlicht/ISceneNodeAnimator.h>

using namespace irr;
/**
 * Interfaz del animador de la camara que se ocupa en la partida, deriva de la clase
 * Animador nodo scena de irrlicht, casi es lo mismo que la interfaz de camara maya que trae integrado
 * irrlicht.
 */
 class IAnimatorCameraTokayo : public scene::ISceneNodeAnimator
{
public:
        /**
         * Método que indica la velocidad de rotación de la camara
         * @return un número flotante de 32 bits
         */
	virtual f32 getRotateSpeed() const = 0;
        /**
         *Coloca la velocidad de rotación que tendrá la camara
         * @param rotateSpeed velocidad deseada. números del 0 al 10
         */
	virtual void setRotateSpeed(f32 rotateSpeed) = 0;
        /**
         *Indica la velocidad del zoom de la camara
         * @return un número flotante de 32 bits
         */
	virtual f32 getZoomSpeed() const = 0;
        /**
         *Coloca la velocidad del zoom de la camara
         * @param zoomSpeed  velocidad deseada en el zoom. números del 0 al 10
         */
	virtual void setZoomSpeed(f32 zoomSpeed) = 0;
        /**
         *Regresa la velocidad de traslación de la camara
         * @return un número flotante de 32 bits
         */
        virtual f32 getMoveSpeed() const =0;
        /**
         *Pone la velocidad de traslacion de la camara
         * @param moveSpeed velocidad deseada  números del 0 al 10
         */
        virtual void setMoveSpeed(f32 moveSpeed)=0;
        /**
         *Coloca los grados de rotación que tendra la camara en el eje de las X y en el eje de las Y.
         * esto es para que la posicion de la camara sea alterada y que a partir de los grados de rotacion en cada eje
         * se puede cocar la camara en una posicion deseada
         * @param RotX
         * @param RotY
         */
        virtual void setRotationNumbers(f32 RotX, f32 RotY) =0;
        /**
         *Regresa los grados de rotacion que tiene la camara con respecto al objetivo en el eje
         * de las X
         * @return  un número flotante de 32 bits que indica los grados
         */
        virtual f32 getRotationNumX() const =0;
        /**
         *Regresa los grados de rotacion que tiene la camara con respecto al objetivo en el eje
         * de las Y
         * @return  un número flotante de 32 bits que indica los grados
         */
        virtual f32 getRotationNumY() const =0;
        /**
         *Coloca el Zoom de la camara
         * @param Zoom
         */
        virtual void setZoom(f32 Zoom) =0;
        /**
         *Regresa al zoom que tiene la camara
         * @return  un número flotante de 32 bits
         */
        virtual f32 getZoom() const =0;
};


#endif
