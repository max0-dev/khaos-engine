#include "EditorApp.h"

#include "khaos/core/Log.h"

int main(int, char**) {
    khaos::Log::init("editor");
    KHAOS_INFO("Starting Khaos Editor v{}", khaos::version());

    int exitCode = 0;
    {
        khaos::editor::EditorApp app;
        if (app.init(1600, 900, "Khaos Editor")) {
            app.run();
        } else {
            exitCode = 1;
        }
    }

    khaos::Log::shutdown();
    return exitCode;
}
