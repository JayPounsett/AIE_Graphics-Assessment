#include "Application.h"
#include "Camera.h"
#include "Gizmos.h"
#include "imgui_glfw3.h"
#include <iostream>

Application* Application::p_instance;

bool Application::Startup()
{
    if (glfwInit() == false)
    {
        return false;
    }

    // Create Window
    p_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "OpenGL Class Project",
        nullptr, nullptr);

    if (p_window == nullptr)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(
        p_window); // Sets specified window as the one that will be used

    if (!gladLoadGL())
    {
        glfwDestroyWindow(p_window);
        glfwTerminate();
        return false;
    }

    printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

    glClearColor(0.25, 0.25, 0.25, 1); // Grey
    glEnable(GL_DEPTH_TEST);

    aie::Gizmos::create(10000, 10000, 0, 0);
    aie::ImGui_Init(p_window, true);

    // Setting Up Mouse Input
    p_instance = this;

    glfwSetCursorPosCallback(p_window, &Application::SetMousePosition);

    // Creating viewport
    m_view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
    m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

    // Load shaders
    // shaderSimple.loadShader(aie::eShaderStage::VERTEX, "./Shader/Vertex/simple.vert");
    // shaderSimple.loadShader(aie::eShaderStage::FRAGMENT,"./Shader/Fragment/simple.frag");
    m_shaderPhong.loadShader(aie::eShaderStage::VERTEX, "./Shader/Vertex/phong.vert");
    m_shaderPhong.loadShader(aie::eShaderStage::FRAGMENT,
        "./Shader/Fragment/phong.frag");

    //if (shaderSimple.link() == false)
    //{
    //    printf("Shader Error: %s\n", shaderSimple.getLastError());
    //    return false;
    //}

    if (m_shaderPhong.link() == false)
    {
        printf("Shader Error: %s\n", m_shaderPhong.getLastError());
        return false;
    }

    m_quadMesh.InitialiseQuad();

    // quad transform - Make it 10 units wide and high
    m_quadTransform = { 10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 1 };

    // Setting Light & Ambient Colours
    m_light.direction = glm::normalize(glm::vec3(-1));
    m_light.colour = { 1, 1, 0 };
    m_ambientLight = { 0.25f, 0.25f, 0.25f };

    // Load Model from File
    m_bunnyMesh.InitialiseFromFile("./Model/Bunny.obj");
    m_bunnyMesh.LoadMaterial("./Model/Bunny.mtl");

    // Halving the size of the bunny so it fits better
    m_bunnyTransform = { 0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 1 };

    return true;
}

// Set Mouse Position Function
void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
    p_instance->m_mousePosition.x = (float)x;
    p_instance->m_mousePosition.y = (float)y;
}

bool Application::Update()
{
    aie::ImGui_NewFrame();
    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f,
        1.0f);
    ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
    ImGui::End();

    m_camera.Update(0.1f, p_window);
    m_lastMousePosition = m_mousePosition;

    // Query time since application started
    float time = glfwGetTime();

    // Close window on ESC or pressing X in top right
    if (glfwWindowShouldClose(p_window) == true || glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        return false;
    }

    return true;
}

void Application::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 pv = m_camera.GetProjectionMatrix(m_windowWidth, m_windowHeight) * m_camera.GetViewMatrix();

    aie::Gizmos::clear();

    aie::Gizmos::addTransform(glm::mat4(1));

    ///// Everything goes below here related to drawing /////

    // Create 10x10 grid
    CreateGrid();

    aie::Gizmos::draw(pv);

    // Bind shader
    m_shaderPhong.bind();

    // Bind lighting
    m_shaderPhong.bindUniform("AmbientColour", m_ambientLight);
    m_shaderPhong.bindUniform("LightColour", m_light.colour);
    m_shaderPhong.bindUniform("LightDirection", m_light.direction);

    // Bind camera position
    m_shaderPhong.bindUniform("CameraPosition", m_camera.GetPosition());

    // Quad - Bind transforms
    auto pvm = pv * m_quadTransform;
    m_shaderPhong.bindUniform("ProjectionViewModel", pvm);
    m_shaderPhong.bindUniform("ModelMatrix", m_quadTransform);
    m_quadMesh.ApplyMaterial(&m_shaderPhong);
    m_quadMesh.Draw();

    // Bunny - Bind transforms
    pvm = pv * m_bunnyTransform;
    m_shaderPhong.bindUniform("ProjectionViewModel", pvm);
    m_shaderPhong.bindUniform("ModelMatrix", m_bunnyTransform);
    m_bunnyMesh.ApplyMaterial(&m_shaderPhong);
    m_bunnyMesh.Draw();

    ImGui::Render();

    glfwSwapBuffers(p_window); // Draw the front, then the back, and alternate.
    glfwPollEvents(); // Windows related stuff - moving the window, etc.
}

void Application::Shutdown()
{
    aie::ImGui_Shutdown();
    aie::Gizmos::destroy();
    glfwDestroyWindow(p_window);
    glfwTerminate();
}

/// <summary>
/// Creates a 10x10 grid of black lines with the centre
/// crossed lines being white.
/// </summary>
void Application::CreateGrid() const
{
    for (int i = 0; i < 21; i++)
    {
        aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10),
            i == 10 ? WHITE : BLACK);
        aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i),
            i == 10 ? WHITE : BLACK);
    }
}
