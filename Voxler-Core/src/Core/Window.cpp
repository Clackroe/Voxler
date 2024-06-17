#include "Window.hpp"
#include <Event/EventSystem.hpp>
#include <glad.h>

#include "GLFW/glfw3.h"
#include "Input/KeyCodes.hpp"

namespace Voxler {

Window::Window(const WindowSettings settings)
{
    glfwSwapInterval(0);
    m_Settings.Title = settings.Title;
    m_Settings.Width = settings.Width;
    m_Settings.Height = settings.Height;

    initWindow(settings);
}

void Window::update()
{
    glfwSwapBuffers(m_Window);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
}

void Window::initWindow(const WindowSettings& settings)
{

    VX_CORE_ASSERT(glfwInit(), "Failed to initialize GLFW")

    // define openGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(settings.Width, settings.Height, settings.Title.c_str(), nullptr, nullptr);

    VX_CORE_ASSERT(m_Window, "Failed to create GLFW window. Terminating.");

    glfwMakeContextCurrent(m_Window);
    VX_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD. Aborting");
    glfwSetWindowUserPointer(m_Window, &m_Settings);
    initCallbacks();
}
void Window::shutdown() { }

void Window::initCallbacks()
{
    glfwSetErrorCallback([](int error, const char* description) {
        VX_CORE_ERROR("GLFW Error {0}: {0}", error, description);
    });

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowSettings& settings = *(WindowSettings*)glfwGetWindowUserPointer(window);
        settings.Height = height;
        settings.Width = width;
        glViewport(0, 0, width, height);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scanCode, int action, int mods) {
        Mode mode = -1;
        switch (action) {
        case GLFW_REPEAT:
            mode = Key::REPEAT;
            break;
        case GLFW_PRESS:
            mode = Key::PRESS;
            break;
        case GLFW_RELEASE:
            mode = Key::RELEASE;
            break;
        default:
            VX_CORE_WARN("UNKNOWN KEY MODE/ACTION");
            break;
        }
        EventSystem::RaiseEvent<KeyEvent>((KeyCode)key, (Mode)mode);
    });
}

void Window::setVsync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    m_Settings.VSync = enabled;
}
}
