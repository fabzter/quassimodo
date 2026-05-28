#ifndef __I_ANIMATOR_CAMERA_TOKAYO_HPP__
#define __I_ANIMATOR_CAMERA_TOKAYO_HPP__

// D2: IrrlichtMt removed ISceneNodeAnimator. IAnimatorCameraTokayo is now a
// standalone interface (no longer inherits from anything). The concrete
// TokayoCamera is called directly instead of being attached as an animator.

namespace Grafico {

class IAnimatorCameraTokayo {
public:
  virtual ~IAnimatorCameraTokayo() {}
  virtual f32 getRotateSpeed() const = 0;
  virtual void setRotateSpeed(f32 rotateSpeed) = 0;
  virtual f32 getZoomSpeed() const = 0;
  virtual void setZoomSpeed(f32 zoomSpeed) = 0;
  virtual f32 getMoveSpeed() const = 0;
  virtual void setMoveSpeed(f32 moveSpeed) = 0;
  virtual void setRotationNumbers(f32 RotX, f32 RotY) = 0;
  virtual f32 getRotationNumX() const = 0;
  virtual f32 getRotationNumY() const = 0;
  virtual void setZoom(f32 Zoom) = 0;
  virtual f32 getZoom() const = 0;
};

} // namespace Grafico
#endif
