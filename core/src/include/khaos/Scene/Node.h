#pragma once

#include <memory>
#include <vector>
namespace KhaosCore {
    class Node {
      protected:
        std::vector<std::unique_ptr<Node>> mChildren;
        virtual void OnEditorUpdate(double dt) {}
        virtual void OnRuntimeUpdate(double dt) {}

      public:
        void EditorUpdate(double dt);
        void RuntimeUpdate(double dt);
        virtual void OnRender() {}
        virtual ~Node() = default;
    };
} // namespace KhaosCore
