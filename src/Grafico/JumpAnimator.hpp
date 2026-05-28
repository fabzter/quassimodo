#ifndef _JUMP_ANIMATOR_HPP_
#define _JUMP_ANIMATOR_HPP_
#include <irrlicht.h>

// D2: IrrlichtMt removed ISceneNodeAnimator. JumpAnimator is now a standalone
// tweening class. Call update(timeMs) each frame from endAnimacion();
// position is interpolated between Start and End using an elliptic arc.

using namespace irr;

class JumpAnimator {
public:
  //! \param startPoint  Starting position in 3D space
  //! \param endPoint    Target position
  //! \param speed       Animation speed (higher = slower)
  //! \param radiusEllipsoid  Height of the arc (0 = flat line)
  JumpAnimator(const core::vector3df &startPoint,
               const core::vector3df &endPoint, f32 speed, f32 radiusEllipsoid,
               scene::ISceneNode *target);

  //! Call each frame. Interpolates target node's position.
  void update(u32 timeMs);

  bool hasFinished() const { return HasFinished; }

private:
  void init();

  core::vector3df Start, End, Center;
  core::vector3df Direction, VecU, VecV;
  f32 Radius, RadiusEllipsoid, Speed;
  s32 StartTime;
  bool HasFinished;
  scene::ISceneNode *Target;
};

#endif
