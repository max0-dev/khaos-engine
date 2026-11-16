#pragma once
#include "khaos/Rendering/GLHandle.h"
#include <string>

namespace KhaosCore {
    class GLShader {
      private:
        GLHandle<GLProgramDestructor> mHandle;

      public:
        GLShader(std::string vertexPath, std::string fragmentPath);
        void Bind() const;
    };
} // namespace KhaosCore
