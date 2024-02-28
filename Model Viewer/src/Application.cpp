#include "Application.h"
#include "Camera.h"
#include "Gizmos.h"
#include "imgui_glfw3.h"
#include <iostream>

Application* Application::m_instance;

bool Application::Startup() {
    if (glfwInit() == false) { return false; }

    // Create Window
    m_window
        = glfwCreateWindow(m_windowWidth, m_windowHeight, "OpenGL Class Project", nullptr, nullptr);

    if (m_window == nullptr) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window); // Sets specified window as the one that will be used

    if (!gladLoadGL()) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

    glClearColor(0.25, 0.25, 0.25, 1); // Grey
    glEnable(GL_DEPTH_TEST);

    aie::Gizmos::create(10000, 10000, 0, 0);
    aie::ImGui_Init(m_window, true);

    // Setting Up Mouse Input
    m_instance = this;
    glfwSetCursorPosCallback(m_window, &Application::SetMousePosition);

    // Creating viewport
    m_view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
    m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

    // Setting Light & Ambient Colours
    m_light.direction = glm::normalize(glm::vec3(-1));
    m_light.colour = { 1, 1, 1 };
    m_ambientLight = { 0.25f, 0.25f, 0.25f };

    // Load shaders
    // Shader List: simpleShader, phongNormal, phongTextured
    //
    m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/Vertex/simpleShader.vert");
    m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/Fragment/simpleShader.frag");

    m_simplePhongShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/Vertex/phongShader.vert");
    m_simplePhongShader.loadShader(aie::eShaderStage::FRAGMENT,
                                   "./Shaders/Fragment/phongShader.frag");

    m_normalPhongShader.loadShader(aie::eShaderStage::VERTEX,
                                   "./Shaders/Vertex/normalPhongShader.vert");
    m_normalPhongShader.loadShader(aie::eShaderStage::FRAGMENT,
                                   "./Shaders/Fragment/normalPhongShader.frag");

    if (m_simpleShader.link() == false) {
        printf("Shader Error: %s\n", m_simpleShader.getLastError());
        return false;
    }

    if (m_simplePhongShader.link() == false) {
        printf("Shader Error: %s\n", m_simplePhongShader.getLastError());
        return false;
    }

    if (m_normalPhongShader.link() == false) {
        printf("Shader Error: %s\n", m_normalPhongShader.getLastError());
        return false;
    }

    // Quad Texture & Initialise
    m_gridTexture.load("./Textures/numbered_grid.tga");
    m_quadMesh.InitialiseQuad();

    // Quad Transform - Make it 10 units wide and high
    m_quadTransform = { 10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 10, 0, 0, 0, 0, 1 };

    // Load Bunny from File
    m_bunnyMesh.InitialiseFromFile("./Models/Bunny.obj");
    m_bunnyMesh.LoadMaterial("./Models/Bunny.mtl");
    m_bunnyTransform = { 0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 1 };

    // Load Soul Spear from file
    m_soulspearMesh.InitialiseFromFile("./Models/soulspear.obj");
    m_soulspearMesh.LoadMaterial("./Models/soulspear.mtl");
    m_soulspearTransform = {
        0.75, 0, 0, 0, 0, 0.75, 0, 0, 0, 0, 0.75, 0, 5, 0, 0, 1
    }; // 4th last digit will move on X-axis, class had it as 5

    return true;
}

// Set Mouse Position Function
void Application::SetMousePosition(GLFWwindow* window, double x, double y) {
    m_instance->m_mousePosition.x = (float)x;
    m_instance->m_mousePosition.y = (float)y;
}

bool Application::Update() {
    aie::ImGui_NewFrame();
    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
    ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
    ImGui::End();

    m_camera.Update(0.1f, m_window);
    m_lastMousePosition = m_mousePosition;

    // Query time since application started
    float time = glfwGetTime();

    // Close window on ESC or pressing X in top right
    if (glfwWindowShouldClose(m_window) == true
        || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return false;
    }

    return true;
}

void Application::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 pv
        = m_camera.GetProjectionMatrix(m_windowWidth, m_windowHeight) * m_camera.GetViewMatrix();

    aie::Gizmos::clear();

    aie::Gizmos::addTransform(glm::mat4(1));

    ///// Everything goes below here related to drawing /////

    // Create 10x10 grid
    CreateGrid();

    aie::Gizmos::draw(pv);

    // Idealy, a MeshRenderer would handle working out which shader is being used, it would bundle
    // all that are using the same one together, process those and then do the next batch, etc. 
    // (Check Week 05 to see what he said around the 50 minute mark)
    
    // Bind shader
    m_simplePhongShader.bind();

    // Bind lighting
    m_simplePhongShader.bindUniform("AmbientColour", m_ambientLight);
    m_simplePhongShader.bindUniform("LightColour", m_light.colour);
    m_simplePhongShader.bindUniform("LightDirection", m_light.direction);

    // Bind camera position
    m_simplePhongShader.bindUniform("CameraPosition", m_camera.GetPosition());

    // Quad
    auto pvm = pv * m_quadTransform;
    m_simplePhongShader.bindUniform("ProjectionViewModel", pvm);
    m_simplePhongShader.bindUniform("ModelMatrix", m_quadTransform);
    m_gridTexture.bind(0);
    m_simplePhongShader.bindUniform("diffuseTex", 0);
    m_quadMesh.ApplyMaterial(&m_simplePhongShader);
    m_quadMesh.Draw();

    // Bunny
    pvm = pv * m_bunnyTransform;
    m_simplePhongShader.bindUniform("ProjectionViewModel", pvm);
    m_simplePhongShader.bindUniform("ModelMatrix", m_bunnyTransform);
    m_simplePhongShader.bindUniform("diffuseTex", 0);
    m_bunnyMesh.ApplyMaterial(&m_simplePhongShader);
    m_bunnyMesh.Draw();

    // Bind shader
    m_normalPhongShader.bind();

    // Bind lighting
    m_normalPhongShader.bindUniform("AmbientColour", m_ambientLight);
    m_normalPhongShader.bindUniform("LightColour", m_light.colour);
    m_normalPhongShader.bindUniform("LightDirection", m_light.direction);

    // Bind camera position
    m_normalPhongShader.bindUniform("CameraPosition", m_camera.GetPosition());

    // Soulspear
    pvm = pv * m_soulspearTransform;
    m_normalPhongShader.bindUniform("ProjectionViewModel", pvm);
    m_normalPhongShader.bindUniform("ModelMatrix", m_soulspearTransform);
    m_soulspearMesh.ApplyMaterial(&m_normalPhongShader);
    m_soulspearMesh.Draw();

    ImGui::Render();

    glfwSwapBuffers(m_window); // Draw the front, then the back, and alternate.
    glfwPollEvents(); // Windows related stuff - moving the window, etc.
}

void Application::Shutdown() {
    aie::ImGui_Shutdown();
    aie::Gizmos::destroy();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

/// <summary>
/// Creates a 10x10 grid of black lines with the centre
/// crossed lines being white.
/// </summary>
void Application::CreateGrid() const {
    for (int i = 0; i < 21; i++) {
        aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10),
                             i == 10 ? WHITE : BLACK);
        aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i),
                             i == 10 ? WHITE : BLACK);
    }
}
