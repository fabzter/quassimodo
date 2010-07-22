#ifndef _JUMP_ANIMATOR_HPP_
#define _JUMP_ANIMATOR_HPP_
#include<irrlicht/irrlicht.h>
#include <irrlicht/ISceneNodeAnimator.h>
#include<iostream>

using namespace irr;
class JumpAnimator : public scene::ISceneNodeAnimator
{
public:

	//! constructor
	JumpAnimator(const core::vector3df& startPoint,
			const core::vector3df& endPoint,
			f32 speed,f32 radiusEllipsoid);

	//! animates a scene node
	virtual void animateNode(scene::ISceneNode* node, u32 timeMs);

	//! Writes attributes of the scene node animator.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

	//! Reads attributes of the scene node animator.
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

	//! Returns type of the scene node animator
	virtual scene::ESCENE_NODE_ANIMATOR_TYPE getType() const { return scene::ESNAT_FLY_CIRCLE; }
		
	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer after calling
	this. */
	virtual scene::ISceneNodeAnimator* createClone(scene::ISceneNode* node, scene::ISceneManager* newManager=0);
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
	// circle center
	core::vector3df Center;
	// up-vector, normal to the circle's plane
	core::vector3df Direction;
	// Two helper vectors
	core::vector3df VecU;
	core::vector3df VecV;
	core::vector3df Start;
	core::vector3df End;
	f32 Radius;
	f32 RadiusEllipsoid;
	f32 Speed;
	u32 StartTime;
	bool HasFinished;
};


#endif

