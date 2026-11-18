#pragma once
#include "khaos/Scene/Node3D.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace KhaosCore {
    class Camera3D : public Node3D {
      protected:
        glm::mat4 mViewMatrix = glm::identity<glm::mat4>();
        glm::mat4 mProjectionMatrix = glm::identity<glm::mat4>();

      public:
        const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
        const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
    };
} // namespace KhaosCore
