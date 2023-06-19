#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "drawing.h"

//接口
const std::string mario_modelRelPath = "obj/mario (2).obj";
const std::string mario_2 = "texture/miscellaneous/Frame 34.jpg";
const std::string mario_1= "texture/miscellaneous/F.jpg";

const std::string castle_modelRelPath = "obj/castle.obj";
const std::string castle_1 = "texture/miscellaneous/C.jpg";
const std::string castle_2 = "texture/miscellaneous/E.jpg";


const std::string texture_path = "texture/miscellaneous/green1.jpg";
const std::string texture_path1 = "texture/miscellaneous/lightblue1.jpg";
const std::string texture_path2 = "texture/miscellaneous/blue1.jpg";
const std::string texture_land = "texture/miscellaneous/B.jpg";
const std::string texture_ball = "texture/miscellaneous/blue2.jpg";
const std::string texture_pris = "texture/miscellaneous/green2.jpg";
const std::string texture_pyramid = "texture/miscellaneous/lightblue2.jpg";
const std::string texture_cylinder = "texture/miscellaneous/red2.jpg";


//bk ft exchange
const std::vector<std::string> skyboxTextureRelPaths = {
    "texture/skybox/gloomy_rt.png", "texture/skybox/gloomy_lf.png",  "texture/skybox/gloomy_up.png",
    "texture/skybox/gloomy_dn.png",  "texture/skybox/gloomy_bk.png", "texture/skybox/gloomy_ft.png"};

void Drawing::settransform()
{

    mario.model->transform.scale = glm::vec3(0.08f, 0.08f, 0.08f);
    mario.model->transform.position = glm::vec3(3.0f, -10.0f, -20.0f);
    mario.model->transform.rotation = { 1.0f, 0.0f, 0.0f, 0.0f };

    castle.model->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    castle.model->transform.position = glm::vec3(50.0f, -10.0f, -100.0f);
    castle.model->transform.rotation = { 1.0f, 0.0f, -0.2f, 0.0f };

    cube.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    cube.transform.position = glm::vec3(-50.0f, -10.0f, -100.0f);
    cube.transform.rotation = { 1.0f, 0.0f, 0.0f, 0.0f };

    cube1.transform.scale = glm::vec3(5.0f, 5.0f, 5.0f);
    cube1.transform.position = glm::vec3(-30.0f, -10.0f, -100.0f);
    cube1.transform.rotation = { 1.0f, 0.0f, 0.0f, 0.0f };

    cube2.transform.scale = glm::vec3(5.0f, 5.0f, 5.0f);
    cube2.transform.position = glm::vec3(-10.0f, -10.0f, -100.0f);
    cube2.transform.rotation = { 1.0f, 0.0f, 0.0f, 0.0f };

    ball.transform.scale = glm::vec3(5.0, 5.0, 5.0);
    ball.transform.position = glm::vec3(20.0f, 0.0f, -70.0f);
    ball.transform.rotation = { 1.0f, 0.0f, 0.0f, 0.0f };

    pris.transform.scale = glm::vec3(5.0, 5.0, 5.0);
    pris.transform.position = glm::vec3(0.0f, 0.0f, -80.0f);
    pris.transform.rotation = { 1.0f, 0.0f, 0.0f, 0.0f };

    pyramid.transform.scale = glm::vec3(5.0, 5.0, 5.0);
    pyramid.transform.position = glm::vec3(-10.0f, -10.0f, -80.0f);
    pyramid.transform.rotation = { 1.0f, -0.6f, 0.0f, 0.0f };

    cylinder.transform.scale = glm::vec3(5.0, 5.0, 5.0);
    cylinder.transform.position = glm::vec3(20.0f, -10.0f, -60.0f);
    cylinder.transform.rotation = { 1.0f, 0.0f, 0.0f, 0.0f };

    land.transform.scale = glm::vec3(150.0f, 100.0f, 100.0f);
    land.transform.position = glm::vec3(-50.0f, -110.0f, -10.0f);
    land.transform.rotation = { 1.0f, 0.0f, 0.0f, 0.0f };

    return;
}

Drawing::Drawing(const Options& options) : 
    Application(options),
    cube(texture_path),
    cube1(texture_path1),
    cube2(texture_path2),
    land(texture_land),
    mario(mario_modelRelPath, mario_2, mario_1),
    castle(castle_modelRelPath, castle_2, castle_1),
    ball(texture_ball),
    pris(texture_pris),
    pyramid(texture_pyramid),
    cylinder(texture_cylinder)
{

    // init model
    settransform();

    // init skybox
    std::vector<std::string> skyboxTextureFullPaths;
    for (size_t i = 0; i < skyboxTextureRelPaths.size(); ++i) {
        skyboxTextureFullPaths.push_back(getAssetFullPath(skyboxTextureRelPaths[i]));
    }
    _skybox.reset(new SkyBox(skyboxTextureFullPaths));

    // init cameras
    _cameras.resize(2);

    const float aspect = 1.0f * _windowWidth / _windowHeight;
    constexpr float znear = 0.1f;
    constexpr float zfar = 10000.0f;

    // perspective camera
    _cameras[0].reset(new PerspectiveCamera(glm::radians(60.0f), aspect, 0.1f, 10000.0f));
    _cameras[0]->transform.position = glm::vec3(0.0f, 0.0f, 15.0f);

    // orthographic camera
    _cameras[1].reset(
        new OrthographicCamera(-4.0f * aspect, 4.0f * aspect, -4.0f, 4.0f, znear, zfar));
    _cameras[1]->transform.position = glm::vec3(0.0f, 0.0f, 15.0f);

    // init light
    _light.reset(new DirectionalLight());
    _light->transform.rotation =
        glm::angleAxis(glm::radians(45.0f), glm::normalize(glm::vec3(-1.0f, -2.0f, -1.0f)));


    // init imGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init();
}

Drawing::~Drawing() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void Drawing::handleInput() {

    constexpr float cameraMoveSpeed = 5.0f;
    constexpr float cameraRotateSpeed = 0.02f;

    if (_input.keyboard.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
        glfwSetWindowShouldClose(_window, true);
        return;
    }

    if (_input.keyboard.keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
        std::cout << "switch camera" << std::endl;
        // switch camera
        activeCameraIndex = (activeCameraIndex + 1) % _cameras.size();
        _input.keyboard.keyStates[GLFW_KEY_SPACE] = GLFW_RELEASE;
        return;
    }

    Camera* camera = _cameras[activeCameraIndex].get();

    if (_input.keyboard.keyStates[GLFW_KEY_W] != GLFW_RELEASE) {
        std::cout << "W" << std::endl;
        camera->transform.position.z -= cameraMoveSpeed * 0.02;
    }

    if (_input.keyboard.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
        std::cout << "A" << std::endl;
        camera->transform.position.x -= cameraMoveSpeed * 0.02;
    }

    if (_input.keyboard.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
        std::cout << "S" << std::endl;
        camera->transform.position.z += cameraMoveSpeed * 0.02;
    }

    if (_input.keyboard.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
        std::cout << "D" << std::endl;
        camera->transform.position.x += cameraMoveSpeed * 0.02;
    }

    /*
    if (_input.mouse.move.xNow != _input.mouse.move.xOld) {
        std::cout << "mouse move in x direction" << std::endl;

        float mouse_x = _input.mouse.move.xNow - _input.mouse.move.xOld;
        glm::quat original_rotation = camera->transform.rotation;

        if (mouse_x > 0)
        {
            camera->transform.rotation = glm::rotate(camera->transform.rotation, cameraRotateSpeed * mouse_x * 0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else
        {
            camera->transform.rotation = glm::rotate(camera->transform.rotation, cameraRotateSpeed * mouse_x * -0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        glm::vec3 rotate_axis = glm::normalize(glm::cross(original_rotation * glm::vec3(1, 0, 0), camera->transform.getRight())); // Calculate the rotation axis by taking the cross product of two vectors

        // Rotate the camera around the calculated rotation axis
        camera->transform.rotation = glm::rotate(original_rotation, cameraRotateSpeed * mouse_x * 0.02f, rotate_axis);

    }

    if (_input.mouse.move.yNow != _input.mouse.move.yOld) {
        std::cout << "mouse move in y direction" << std::endl;
        float mouse_y = _input.mouse.move.yNow - _input.mouse.move.yOld;
        glm::vec3 forward = -camera->transform.getFront();
        glm::quat original_rotation = camera->transform.rotation;

        if (mouse_y > 0)
        {
            camera->transform.rotation = glm::rotate(camera->transform.rotation, cameraRotateSpeed * mouse_y * 0.02f, camera->transform.getRight());
        }
        else
        {
            camera->transform.rotation = glm::rotate(camera->transform.rotation, cameraRotateSpeed * mouse_y * -0.02f, camera->transform.getRight());
        }
        glm::vec3 new_forward = -camera->transform.getFront();  // Get the new forward vector after rotating
        glm::vec3 rotate_axis = glm::cross(forward, new_forward); // Calculate the rotation axis by taking the cross product of the original and new forward vectors
        camera->transform.rotation = glm::rotate(original_rotation, cameraRotateSpeed * mouse_y * 0.02f, rotate_axis);

    }
    */

}

void Drawing::renderFrame() {
    // some options related to imGUI
    static bool wireframe = false;

    // trivial things
    showFpsInWindowTitle();

    glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    const glm::mat4 projection = _cameras[activeCameraIndex]->getProjectionMatrix();
    const glm::mat4 view = _cameras[activeCameraIndex]->getViewMatrix();

   
    switch (mario_renderMode) {
    case RenderMode::Simple:
        mario.setRenderMode(RenderMode::Simple);
        break;
    case RenderMode::Blend:
        mario.setRenderMode(RenderMode::Blend);
		break;
    }

    switch (castle_renderMode) {
    case RenderMode::Simple:
        castle.setRenderMode(RenderMode::Simple);
        break;
    case RenderMode::Blend:
        castle.setRenderMode(RenderMode::Blend);
        break;
    }

    mario.draw(projection, view,_light);
    castle.draw(projection, view, _light);

    // draw skybox
    _skybox->draw(projection, view);

    cube.draw(projection,view,_light);
    cube1.draw(projection, view, _light);
    cube2.draw(projection, view, _light);

    ball.draw(projection, view, _light);
    pris.draw(projection, view, _light);
    pyramid.draw(projection, view, _light);

    land.draw(projection, view, _light);
    cylinder.draw(projection, view, _light);

    // draw ui elements
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

    if (!ImGui::Begin("Control Panel", nullptr, flags)) {
        ImGui::End();
    }
    else {
        ImGui::Text("Mario Render Mode");
        ImGui::Separator();
        ImGui::RadioButton("mario Simple Texture Shading", (int*)&mario_renderMode, (int)(RenderMode::Simple));
        ImGui::NewLine();

        ImGui::RadioButton("mario Blend Texture Shading", (int*)&mario_renderMode, (int)(RenderMode::Blend));
        ImGui::ColorEdit3("mario kd1", (float*)&mario._blendMaterial->kds[0]);
        ImGui::ColorEdit3("mario kd2", (float*)&mario._blendMaterial->kds[1]);
        ImGui::SliderFloat("mario blend", &mario._blendMaterial->blend, 0.0f, 1.0f);
        ImGui::NewLine();
        ImGui::NewLine();

        ImGui::Text("Castle Render Mode");
        ImGui::Separator();
        ImGui::RadioButton("castle Simple Texture Shading", (int*)&castle_renderMode, (int)(RenderMode::Simple));
        ImGui::NewLine();

        ImGui::RadioButton("castle Blend Texture Shading", (int*)&castle_renderMode, (int)(RenderMode::Blend));
        ImGui::ColorEdit3("castle kd1", (float*)&castle._blendMaterial->kds[0]);
        ImGui::ColorEdit3("castle kd2", (float*)&castle._blendMaterial->kds[1]);
        ImGui::SliderFloat("castle blend", &castle._blendMaterial->blend, 0.0f, 1.0f);
        ImGui::NewLine();


        ImGui::Text("Directional light");
        ImGui::Separator();
        ImGui::SliderFloat("intensity", &_light->intensity, 0.0f, 2.0f);
        ImGui::ColorEdit3("color", (float*)&_light->color);
        ImGui::NewLine();

        ImGui::SliderFloat("length of cube", &cube.transform.scale.x, 10.0f, 20.0f);
        ImGui::NewLine();

        ImGui::SliderFloat("height of cube", &cube.transform.scale.y, 10.0f, 20.0f);
        ImGui::NewLine();

        ImGui::SliderFloat("width of cube", &cube.transform.scale.z, 10.0f, 20.0f);
        ImGui::NewLine();

        ImGui::SliderFloat("radius", &ball.transform.scale.x, 1.0f, 10.0f);
        ball.transform.scale.y = ball.transform.scale.x;
        ball.transform.scale.z = ball.transform.scale.x;
        ImGui::NewLine();

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
