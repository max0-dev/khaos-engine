#include "khaos/Scene/FreeLook3D.h"
#include "khaos/App/App.h"
#include "khaos/Input/InputManager.h"
using namespace KhaosCore;

FreeLook3D::FreeLook3D() {
    mYaw = glm::degrees(std::atan2(mFront.z, mFront.x));
    mPitch = glm::degrees(std::asin(mFront.y));
}

void FreeLook3D::OnEditorUpdate(double dt) {
    auto window = App::instance().GetWindow()->GetNative();
    glm::vec2 mousePos = InputManager::GetMousePosition();

    if (InputManager::GetCursorMode() == CursorMode::Visible) {
        mFirst = true;
    }

    if (mFirst) {
        mFirst = false;
        mLastx = mousePos.x;
        mLasty = mousePos.y;
    }

    double dx = mousePos.x - mLastx;
    double dy = mLasty - mousePos.y;

    mLastx = mousePos.x;
    mLasty = mousePos.y;

    using namespace glm;
    mYaw += dx * mLookSpeed;
    mPitch += dy * mLookSpeed;
    mPitch = clamp(mPitch, -89.0, 89.0);

    double sinz = sin(radians(mYaw));
    double cosz = cos(radians(mYaw));

    double siny = sin(radians(mPitch));
    double cosy = cos(radians(mPitch));

    glm::dvec3 front(cosz * cosy, siny, sinz * cosy);
    mFront = normalize(front);
    // mFront.x = cosz * cosy;
    // mFront.y = siny;
    // mFront.z = sinz * cosy;

    vec3 moveDir = vec3(0.0f, 0.0f, 0.0f);

    if (InputManager::IsKeyDown(Key::W)) {
        moveDir += mFront;
    }
    if (InputManager::IsKeyDown(Key::S)) {
        moveDir -= mFront;
    }
    if (InputManager::IsKeyDown(Key::A)) {
        moveDir -= cross(mFront, mUp);
    }
    if (InputManager::IsKeyDown(Key::D)) {
        moveDir += cross(mFront, mUp);
    }

    if (moveDir != vec3(0.0f, 0.0f, 0.0f)) {
        mPosition += normalize(moveDir) * mMoveSpeed * (float)dt;
    }
}

void FreeLook3D::UpdateProjectionMatrix() {}

void FreeLook3D::UpdateViewMatrix() {}
