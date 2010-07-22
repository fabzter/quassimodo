#include "JumpAnimator.hpp"
using namespace irr;
//! constructor
JumpAnimator::JumpAnimator(const core::vector3df& startPoint,
			const core::vector3df& endPoint,
			f32 speed,f32 radiusEllipsoid)
	:Start(startPoint), End(endPoint),RadiusEllipsoid(radiusEllipsoid),
	Speed(speed), StartTime(-1), scene::ISceneNodeAnimator()
{
	#ifdef _DEBUG
	setDebugName("JumpAnimator");
	#endif
	init();
}


void JumpAnimator::init()
{
	core::vector3df aux=(this->End-this->Start)/2;
	this->Center= this->Start+aux;
	this->Center.Y=this->Start.Y ;
	this->Direction= this->End-this->Start;
	this->Radius=aux.getLength();
        HasFinished = false;
	Direction.normalize();

	VecV =core::vector3df(0,1,0);
	VecU = Direction*-1;


}
bool JumpAnimator::llego_fin(scene::ISceneNode* node){
	int error=5.0;
        core::vector3di end( (int)this->End.X,(int)this->End.Y,(int)this->End.Z );
        core::vector3di ahora( (int)node->getPosition().X,(int)node->getPosition().Y,(int)node->getPosition().Z );
        return end.equals(ahora,error);
}

//! animates a scene node
void JumpAnimator::animateNode(scene::ISceneNode* node, u32 timeMs)
{
	if ( 0 == node ||HasFinished )
		return;
	if(StartTime==-1)
		StartTime=timeMs;

	f32 time = ((s32)timeMs-StartTime) / Speed;	

	f32 r2 = RadiusEllipsoid == 0.f ? Radius : RadiusEllipsoid;
	node->setPosition( (Center + (Radius*cosf(time)*VecU) + (r2*sinf(time)*VecV )) );
	
	if (this->llego_fin(node))
	{
		HasFinished = true;
		node->setPosition(this->End);
	}
}


//! Writes attributes of the scene node animator.
void JumpAnimator::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addFloat("RadiusEllipsoid", Radius);
	out->addVector3d("Start", Start);
	out->addVector3d("End", End);
	out->addFloat("Speed", Speed);
}


//! Reads attributes of the scene node animator.
void JumpAnimator::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{

	End = in->getAttributeAsVector3d("End");
	Speed = in->getAttributeAsFloat("Speed");
	Start = in->getAttributeAsVector3d("Start");
	StartTime = -1;
	RadiusEllipsoid = in->getAttributeAsFloat("RadiusEllipsoid");
	init();
}


scene::ISceneNodeAnimator* JumpAnimator::createClone(scene::ISceneNode* node, scene::ISceneManager* newManager)
{
	JumpAnimator * newAnimator = 
		new JumpAnimator(Start,End, Speed,RadiusEllipsoid);

	return newAnimator;
}
