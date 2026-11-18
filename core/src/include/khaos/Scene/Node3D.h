#pragma once
#include "khaos/Rendering/Transform.h"
#include "khaos/Scene/Node.h"
namespace KhaosCore {
    class Node3D : public Node {
      protected:
        Transform mTransform;
    };
} // namespace KhaosCore
