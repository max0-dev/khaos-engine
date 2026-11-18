#pragma once

#include "khaos/Scene/Camera3D.h"
namespace KhaosCore {
    class FreeLook3D : public Camera3D {
      protected:
        glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, -3.0f);
        glm::vec3 mFront = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);

        float mMoveSpeed = 15.0f;
        float mLookSpeed = 0.25f;
        double mYaw = 0;
        double mPitch = 0;

        bool mFirst = true;
        double mLastx;
        double mLasty;

        void OnEditorUpdate(double dt) override;

      public:
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();
        FreeLook3D();
    };
} // namespace KhaosCore
