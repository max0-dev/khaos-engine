#include "khaos/EditorApp.h"
#include "khaos/App/App.h"
#include "khaos/Rendering/OpenGL/GLShader.h"
#include "khaos/Rendering/OpenGL/GLVertexArrayObject.h"
#include "khaos/Rendering/OpenGL/GLVertexBufferObject.h"
#include <GL/glew.h>
#include <array>

using namespace KhaosEditor;
using namespace KhaosCore;

void EditorApp::CreateEditorViewport() {}

EditorApp::EditorApp() { CreateEditorViewport(); }

void EditorApp::Update() {}

void EditorApp::Render() {
    // TEST: VAO/VBO/shader check, one triangle with a color per corner that the GPU
    // interpolates across the face. Created on the first frame, kept in statics.
    static constexpr std::array<float, 15> vertices = {
        // position    // color
        -0.5F, -0.5F, 1.0F, 0.0F, 0.0F, // bottom left, red
        0.5F,  -0.5F, 0.0F, 1.0F, 0.0F, // bottom right, green
        0.0F,  0.5F,  0.0F, 0.0F, 1.0F, // top, blue
    };
    static GLShader shader("res/editor/test_triangle.vert", "res/editor/test_triangle.frag");
    static GLVertexBufferObject vbo;
    static GLVertexArrayObject vao;
    static bool initialized = false;

    if (!initialized) {
        vbo.BufferData(static_cast<GLsizeiptr>(sizeof(vertices)), vertices.data());
        vao.GetLayout().PushFloat(2); // location 0: aPos
        vao.GetLayout().PushFloat(3); // location 1: aColor
        vao.AddBuffer(vbo);
        initialized = true;
    }

    glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.Bind();
    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
