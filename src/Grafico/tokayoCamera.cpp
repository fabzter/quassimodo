
#include "tokayoCamera.hpp"

using namespace irr;

Grafico::TokayoCamera::TokayoCamera(f32 rotate, f32 zoom, f32 translation)
    : Zooming(false), Rotating(false), Translating(false), ZoomSpeed(zoom),
      RotateSpeed(rotate), TranslateSpeed(translation), CurrentZoom(400.0f),
      RotX(0.0f), RotY(0.0f), MousePos(0.5f, 0.5f), atFirstMouse(false) {
  allKeysUp();
}

Grafico::TokayoCamera::~TokayoCamera() {}

bool Grafico::TokayoCamera::OnEvent(const SEvent &event) {
  if (event.EventType != EET_MOUSE_INPUT_EVENT)
    return false;

  switch (event.MouseInput.Event) {
  case EMIE_MOUSE_MOVED:
    MousePos.X = static_cast<f32>(event.MouseInput.X);
    MousePos.Y = static_cast<f32>(event.MouseInput.Y);
    atFirstMouse = false;
    return true;
  case EMIE_MOUSE_WHEEL:
    CurrentZoom += event.MouseInput.Wheel * ZoomSpeed * -1.0f;
    return true;
  case EMIE_LMOUSE_PRESSED_DOWN:
    MouseKeys[0] = true;
    return true;
  case EMIE_RMOUSE_PRESSED_DOWN:
    MouseKeys[1] = true;
    return true;
  case EMIE_MMOUSE_PRESSED_DOWN:
    MouseKeys[2] = true;
    return true;
  case EMIE_LMOUSE_LEFT_UP:
    MouseKeys[0] = false;
    return true;
  case EMIE_RMOUSE_LEFT_UP:
    MouseKeys[1] = false;
    return true;
  case EMIE_MMOUSE_LEFT_UP:
    MouseKeys[2] = false;
    return true;
  default:
    return false;
  }
}

void Grafico::TokayoCamera::update(scene::ICameraSceneNode *camera,
                                   u32 timeMs) {
  if (!camera)
    return;

  if (!camera->isInputReceiverEnabled())
    return;

  scene::ISceneManager *smgr = camera->getSceneManager();
  if (smgr && smgr->getActiveCamera() != camera)
    return;

  if (OldCamera != camera) {
    OldTarget = camera->getTarget();
    OldCamera = camera;
  }

  core::vector3df target = camera->getTarget();
  f32 nRotX = RotX;
  f32 nRotY = RotY;
  f32 nZoom = CurrentZoom;
  const f32 targetMinDistance = 0.1f, targetMaxDistance = 900.0f;

  // Zoom
  if ((isMouseKeyDown(0) && isMouseKeyDown(2)) || isMouseKeyDown(1)) {
    if (!Zooming) {
      ZoomStart = MousePos;
      Zooming = true;
      nZoom = CurrentZoom;
    } else {
      nZoom += (ZoomStart.X - MousePos.X) * ZoomSpeed;
      if (nZoom < targetMinDistance)
        nZoom = targetMinDistance;
      if (nZoom > targetMaxDistance)
        nZoom = targetMaxDistance;
    }
  } else if (Zooming) {
    const f32 old = CurrentZoom;
    CurrentZoom = CurrentZoom + (ZoomStart.X - MousePos.X) * ZoomSpeed;
    nZoom = CurrentZoom;
    if (nZoom < 0)
      nZoom = CurrentZoom = old;
    if (nZoom > targetMaxDistance)
      nZoom = CurrentZoom = targetMaxDistance;
    Zooming = false;
  }

  // Translation
  core::vector3df translate(OldTarget), upVector(camera->getUpVector());
  core::vector3df tvectX = Pos - target;
  tvectX = tvectX.crossProduct(upVector);
  tvectX.normalize();

  const scene::SViewFrustum *va = camera->getViewFrustum();
  core::vector3df tvectY = (va->getFarLeftDown() - va->getFarRightDown());
  tvectY = tvectY.crossProduct(upVector.Y > 0 ? Pos - target : target - Pos);
  tvectY.normalize();

  if (isMouseKeyDown(2) && !Zooming) {
    if (!Translating) {
      TranslateStart = MousePos;
      Translating = true;
    } else {
      translate += tvectX * (TranslateStart.X - MousePos.X) * TranslateSpeed +
                   tvectY * (TranslateStart.Y - MousePos.Y) * TranslateSpeed;
    }
  } else if (Translating) {
    translate += tvectX * (TranslateStart.X - MousePos.X) * TranslateSpeed +
                 tvectY * (TranslateStart.Y - MousePos.Y) * TranslateSpeed;
    OldTarget = translate;
    Translating = false;
  }

  // Rotation
  if (isMouseKeyDown(0) && !Zooming) {
    if (!Rotating) {
      RotateStart = MousePos;
      Rotating = true;
      nRotX = RotX;
      nRotY = RotY;
    } else {
      nRotX += (RotateStart.X - MousePos.X) * RotateSpeed;
      nRotY += (RotateStart.Y - MousePos.Y) * RotateSpeed;
    }
  } else if (Rotating) {
    RotX += (RotateStart.X - MousePos.X) * RotateSpeed;
    RotY += (RotateStart.Y - MousePos.Y) * RotateSpeed;
    nRotX = RotX;
    nRotY = RotY;
    Rotating = false;
  }

  // Set position
  target = translate;
  Pos.X = nZoom + target.X;
  Pos.Y = target.Y;
  Pos.Z = target.Z;
  Pos.rotateXYBy(nRotY, target);
  Pos.rotateXZBy(-nRotX, target);

  upVector.set(0, 1, 0);
  upVector.rotateXYBy(-nRotY);
  upVector.rotateXZBy(-nRotX + 180.f);

  camera->setPosition(Pos);
  camera->setTarget(target);
  camera->setUpVector(upVector);
}

bool Grafico::TokayoCamera::isMouseKeyDown(s32 key) { return MouseKeys[key]; }

void Grafico::TokayoCamera::allKeysUp() {
  for (s32 i = 0; i < 3; ++i)
    MouseKeys[i] = false;
}

void Grafico::TokayoCamera::setRotateSpeed(f32 speed) { RotateSpeed = speed; }
void Grafico::TokayoCamera::setZoomSpeed(f32 speed) { ZoomSpeed = speed; }
f32 Grafico::TokayoCamera::getRotateSpeed() const { return RotateSpeed; }
f32 Grafico::TokayoCamera::getZoomSpeed() const { return ZoomSpeed; }
f32 Grafico::TokayoCamera::getMoveSpeed() const { return TranslateSpeed; }
void Grafico::TokayoCamera::setMoveSpeed(f32 speed) { TranslateSpeed = speed; }

void Grafico::TokayoCamera::setRotationNumbers(f32 rx, f32 ry) {
  RotX = rx;
  RotY = ry;
}
f32 Grafico::TokayoCamera::getRotationNumX() const { return RotX; }
f32 Grafico::TokayoCamera::getRotationNumY() const { return RotY; }
void Grafico::TokayoCamera::setZoom(f32 zoom) { CurrentZoom = zoom; }
f32 Grafico::TokayoCamera::getZoom() const { return CurrentZoom; }
