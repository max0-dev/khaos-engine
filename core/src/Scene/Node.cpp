#include "khaos/Scene/Node.h"

using namespace KhaosCore;

void Node::EditorUpdate(double dt) {
    OnEditorUpdate(dt);

    for (const auto& child : mChildren) {
        child->EditorUpdate(dt);
    }
}

void Node::RuntimeUpdate(double dt) {
    OnRuntimeUpdate(dt);

    for (const auto& child : mChildren) {
        child->RuntimeUpdate(dt);
    }
}
