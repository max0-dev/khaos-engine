#pragma once
#include "khaos/App/App.h"

namespace KhaosEditor {
    class EditorApp : public KhaosCore::App {
      private:
        void Update() override;
        void Render() override;
        void CreateEditorViewport();

      public:
        EditorApp();
    };
} // namespace KhaosEditor
