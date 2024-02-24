#include "Application.h"

#include "Gizmos.h"
#include <iostream>

bool Application::startup()
{
    if (glfwInit() == false) {
        return false;
    }

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Model Viewer", nullptr,
        nullptr);

    if (m_window == nullptr) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL()) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

    glClearColor(0.25f, 0.25f, 0.25f, 1); // Grey Window
    glEnable(GL_DEPTH_TEST);

    aie::Gizmos::create(10000, 10000, 0, 0);

    /*m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/vertex/simple.vert");
    m_shader.loadShader(aie::eShaderStage::FRAGMENT,
                      "./shaders/fragment/simple.frag");

    if (m_shader.link() == false) {
      printf("Shader Error %s\n", m_shader.getLastError());
      return false;
    }

    m_quadMesh.initialiseQuad();*/

    //// quad transform - make it 10 units wide x 10 units high
    // m_quadTransform = {10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 1};

    return true;
}

bool Application::update()
{
    if (glfwWindowShouldClose(m_window) == true || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return false;
    }
    return true;
}

void Application::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    aie::Gizmos::clear();

    aie::Gizmos::addTransform(glm::mat4(1)); // 3-Colour XYZ Tool

    createGrid(); // 10x10 square grid (21 x 21 grid lines)
    createViewPort();

    //////////// Everything goes in here related to drawing ////////////

    //// Bind shader
    // shader.bind();

    //// Bind Transform
    // auto pvm = projection * view * quadTransform;
    // shader.bindUniform("ProjectionViewModel", pvm);

    //// Draw Quad
    // quadMesh.draw();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Application::shutdown()
{
    aie::Gizmos::destroy();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

/// <summary>
/// Taking in the number of grid lines and two colours, using Gizmos it will
/// draw out a grid with the minor colour colouring the middle crossed lines.
/// </summary>
/// <param name="numberGridLines"></param>
/// <param name="middleLineColour"></param>
/// <param name="gridLineColour"></param>
void Application::createGrid() const
{
    for (int i = 0; i < 21; i++) {
        aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10),
            i == 10 ? WHITE : BLACK);
        aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i),
            i == 10 ? WHITE : BLACK);
    }
}

/// <summary>
/// Taking in projection and view, use Gizmos to draw the viewport.
/// </summary>
/// <param name="projection"></param>
/// <param name="view"></param>
void Application::createViewPort() const { aie::Gizmos::draw( m_projection * m_view ); }
