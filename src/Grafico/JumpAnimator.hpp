#ifndef _JUMP_ANIMATOR_HPP_
#define _JUMP_ANIMATOR_HPP_
#include<irrlicht/irrlicht.h>
#include <irrlicht/ISceneNodeAnimator.h>
#include<iostream>

using namespace irr;
class JumpAnimator : public scene::ISceneNodeAnimator
{
public:

	/**
         * Constructor de nuestro animador
         * @param startPoint punto del cual iniciará a moverse
         * @param endPoint Punto en el que terminará el movimiento
         * @param speed velocidad de la animación, entre mas grande sea el numero, mas tiempo se tardará
         * @param radiusEllipsoid radio del giro del movimiento, lo utilizo con el número de celdas a saltar
         * si salta mas de una celda entonces el radio es mayor. (un número por defecto)
         */
	JumpAnimator(const core::vector3df& startPoint,
			const core::vector3df& endPoint,
			f32 speed,f32 radiusEllipsoid);

	/**
         * Anima el nodo Escena
         * @param node nodo escena que se esta animando.
         * @param timeMs tiempo en Milisegundos
         */
	virtual void animateNode(scene::ISceneNode* node, u32 timeMs);

	/**
         * Escribe los atributos al nodo escena animador
         */
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

	/**
         * lee los Atributos del nodo escena animador
         */
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

	/**
         * Regresa el tipo del nodo escena animador
         * @return Un valor del enum irr::scene::ESCENE_NODE_ANIMATOR_TYPE
         */
	virtual scene::ESCENE_NODE_ANIMATOR_TYPE getType() const { return scene::ESNAT_FLY_CIRCLE; }
		
	/***
         * Crea un clone de este animador.
         */
	virtual scene::ISceneNodeAnimator* createClone(scene::ISceneNode* node, scene::ISceneManager* newManager=0);
        /**
         * Indica si la animacion ha terminado o no.
         * @return true si termino, false en caso contrario.
         */
	virtual bool hasFinished(void) const { return HasFinished; }

private:
	/**
	*Realiza algunos calculos iniciales.
	*/
	void init();
	/**
	*Método que nos indica si el nodo ha llegado a la posicion destino
	*@param node nodo en el que se está animando
	*@return true si ya llego o esta en un ranco considerablemente cercano. false
	*en caso contrario
	*/
	bool llego_fin(scene::ISceneNode* node);
	/**
         * Centro de la distancia a recorrer.
         */
	core::vector3df Center;
	/**
         * Dirección del movimiento.
         */
	core::vector3df Direction;
	/**
         * Vector de ayuda.
         */
	core::vector3df VecU;
        /**
         * Vector de ayuda.
         */
	core::vector3df VecV;
        /**
         * Punto de inicio del recorrido de la animacion.
         */
	core::vector3df Start;
        /**
         *Punto de fin del recorrido de la animacion.
         */
	core::vector3df End;
        /**
         * radio de cuando salta una sola celda
         */
	f32 Radius;
        /**
         * Radio de cuendo va a saltar mas de una celda.
         */
	f32 RadiusEllipsoid;
        /**
         * Velocidad de la animacion.
         */
	f32 Speed;
        /**
         * Tiempo en que inicia la animaición
         */
	u32 StartTime;
        /**
         * bandere que indica si la animacion termino o no.
         */
	bool HasFinished;
};


#endif

