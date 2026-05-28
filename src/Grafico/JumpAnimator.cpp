#include "JumpAnimator.hpp"
using namespace irr;

JumpAnimator::JumpAnimator(const core::vector3df &startPoint,
                           const core::vector3df &endPoint, f32 speed,
                           f32 radiusEllipsoid, scene::ISceneNode *target)
    : Start(startPoint), End(endPoint), RadiusEllipsoid(radiusEllipsoid),
      Speed(speed), StartTime(-1), HasFinished(false), Target(target) {
  init();
  if (Target)
    Target->setPosition(Start);
}

void JumpAnimator::init() {
  core::vector3df aux = (End - Start) / 2.0f;
  Center = Start + aux;
  Center.Y = Start.Y;
  Direction = End - Start;
  Radius = aux.getLength();

  if (Radius < 0.001f) {
    HasFinished = true;
    if (Target)
      Target->setPosition(End);
    return;
  }

  Direction.normalize();

  if (RadiusEllipsoid <= 2.0f)
    RadiusEllipsoid = 140.0f;
  else
    RadiusEllipsoid = 0.0f;

  VecV = core::vector3df(0, 1, 0);
  VecU = Direction * -1.0f;
}

void JumpAnimator::update(u32 timeMs) {
  if (!Target || HasFinished)
    return;

  if (StartTime == -1)
    StartTime = static_cast<s32>(timeMs);

  f32 time = (static_cast<s32>(timeMs) - StartTime) / Speed;
  f32 r2 = RadiusEllipsoid == 0.0f ? Radius : RadiusEllipsoid;

  Target->setPosition(Center + (Radius * cosf(time) * VecU) +
                      (r2 * sinf(time) * VecV));

  f32 dist = Target->getPosition().getDistanceFromSQ(End);
  if (dist < 100.0f) {
    HasFinished = true;
    Target->setPosition(End);
  }
}
