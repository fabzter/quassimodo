#ifndef __TOKAYO_CAMERA_HPP__
#define __TOKAYO_CAMERA_HPP__

#include "IAnimatorCameraTokayo.hpp"
#include <ICameraSceneNode.h>
#include <ISceneManager.h>
#include <ISceneNode.h>
#include <IVideoDriver.h>
#include <SViewFrustum.h>
#include <iostream>
#include <stdio.h>

using namespace irr;

namespace Grafico {

/**
 * Camera controller — D2: decoupled from ISceneNodeAnimator (removed in
 * IrrlichtMt). Call update() each frame with the camera node and current time.
 * Also acts as an event receiver — call OnEvent() for mouse/keyboard input.
 */
class TokayoCamera : public IAnimatorCameraTokayo {
public:
  TokayoCamera(f32 rotateSpeed = 3.0f, f32 zoomSpeed = 3.0f,
               f32 translationSpeed = 3.0f);
  virtual ~TokayoCamera();

  //! Camera update — call each frame. Replaces old animateNode().
  virtual void update(scene::ICameraSceneNode *camera, u32 timeMs);

  //! Event receiver for mouse/keyboard input.
  virtual bool OnEvent(const SEvent &event);

  virtual f32 getRotateSpeed() const;
  virtual void setRotateSpeed(f32 rotateSpeed);

  virtual f32 getZoomSpeed() const;
  virtual void setZoomSpeed(f32 zoomSpeed);

  virtual f32 getMoveSpeed() const;
  virtual void setMoveSpeed(f32 moveSpeed);

  virtual void setRotationNumbers(f32 RotX, f32 RotY);
  virtual f32 getRotationNumX() const;
  virtual f32 getRotationNumY() const;

  virtual void setZoom(f32 Zoom);
  virtual f32 getZoom() const;

private:
  void allKeysUp();
  bool isMouseKeyDown(s32 key);

  f32 RotateSpeed;
  f32 ZoomSpeed;
  f32 TranslateSpeed;
  f32 RotX, RotY;
  f32 CurrentZoom;
  bool Rotating, Zooming, Translating;
  bool MouseKeys[3];
  core::position2df MousePos;
  core::vector3df Pos;
  bool atFirstMouse;
  scene::ICameraSceneNode *OldCamera;
  core::vector3df OldTarget;
  core::position2df ZoomStart, RotateStart, TranslateStart;
};

} // namespace Grafico

#endif
