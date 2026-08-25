#include "BGLApp.h"

BGLApp::BGLApp()
{
    appWindow = std::make_unique<BGLWindow>(800, 600, "OpenGL App");
}

BGLApp::BGLApp(int w, int h, const char* title)
{
    appWindow = std::make_unique<BGLWindow>(w, h, title);
}

BGLApp::~BGLApp()
{
    appWindow.reset();
}

void BGLApp::run()
{
    const BGLShader shader("./shaders/OpenGL/gl_shader.vs", "./shaders/OpenGL/gl_shader.fs");

    while (!appWindow->shouldClose())
    {
        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            PLOGI << "Escape key pressed. Closing app...";
            glfwSetWindowShouldClose(appWindow->getWindow(), GLFW_TRUE);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glfwPollEvents();
        glfwSwapBuffers(appWindow->getWindow());
    }
}
