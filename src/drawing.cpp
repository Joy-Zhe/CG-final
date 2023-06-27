#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "drawing.h"

//接口
const std::string mario_modelRelPath[6] = {"obj/mario (2).obj", "obj/mario (2).obj", "obj/mario (2).obj", "obj/mario (2).obj", "obj/mario (2).obj", "obj/mario (2).obj" };
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
const std::string texture_pyramid = "texture/miscellaneous/lightblue1.jpg";
const std::string texture_cylinder = "texture/miscellaneous/red2.jpg";

//bk ft exchange
const std::vector<std::string> skyboxTextureRelPaths = {
    "texture/skybox/right.jpg", "texture/skybox/left.jpg",  "texture/skybox/top.jpg",
    "texture/skybox/down.jpg",  "texture/skybox/back.jpg", "texture/skybox/front.jpg"};

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

void Drawing::setscale()
{
    mario.model->transform.scale *= scale;
   
    castle.model->transform.scale *= scale;
   
    cube.transform.scale *= scale;
   
    cube1.transform.scale *= scale;
 
    cube2.transform.scale *= scale;

    ball.transform.scale *= scale;

    pris.transform.scale *= scale;

    pyramid.transform.scale *= scale;

    cylinder.transform.scale *= scale;

    return;
}


Drawing::Drawing(const Options& options) : 
    Application(options),
    cube(texture_path),
    cube1(texture_path1),
    cube2(texture_path2),
    land(texture_land),
    mario(mario_modelRelPath[0], mario_2, mario_1),
    castle(castle_modelRelPath, castle_2, castle_1),
    ball(texture_ball),
    pris(texture_pris),
    pyramid(texture_pyramid),
    cylinder(texture_cylinder),
    screenshot(_windowWidth, _windowHeight)
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
    _cameras[0]->transform.position = glm::vec3(0.0f, 0.0f, 20.0f);

    // orthographic camera
    _cameras[1].reset(
        new OrthographicCamera(-4.0f * aspect, 4.0f * aspect, -4.0f, 4.0f, znear, zfar));
    _cameras[1]->transform.position = glm::vec3(0.0f, 0.0f, 20.0f);

    // init light
    _light.reset(new DirectionalLight());
    _light->intensity = 0.5f;
    _light->transform.position = glm::vec3(0.0f, 1.0f, -10.0f);
    _light->transform.rotation =
        glm::angleAxis(glm::radians(45.0f), glm::normalize(glm::vec3(-1.0f)));

    _ambientLight.reset(new AmbientLight);

    _spotLight.reset(new SpotLight);
    _spotLight->intensity = 0.5f;
    _spotLight->angle = glm::radians(90.0f);
    _spotLight->transform.position = glm::vec3(20.0f, 0.0f, -60.0f);
    _spotLight->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    //init material
    _material.reset(new Material);
    _material->ka = glm::vec3(0.03f, 0.03f, 0.03f);
    _material->kd = glm::vec3(1.0f, 1.0f, 1.0f);
    _material->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    _material->ns = 10.0f;

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

float getDeltaTime() {
    static auto previousTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
    previousTime = currentTime;
    return deltaTime;
}

void rotateCameraAroundObject(Camera& camera, const glm::vec3& objectPosition, float speed, float deltaTime) {
    
    glm::vec3 toObject = objectPosition - camera.transform.position;

    // 计算相机当前位置到物体中心的距离
    float distanceToObject = glm::length(toObject);

    // 计算相机当前位置到物体中心的方向
    glm::vec3 directionToObject = toObject / distanceToObject;

    glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);  // 围绕Y轴旋转

    // 计算相机围绕物体的旋转角度
    float angle = speed * deltaTime;

    glm::quat rotation = glm::angleAxis(angle, axis);

    // 将旋转应用到相机当前位置到物体中心的方向向量
    directionToObject = rotation * directionToObject;

    // 计算新的相机位置
    glm::vec3 newPosition = objectPosition - directionToObject * distanceToObject;

    camera.transform.position = newPosition;
    camera.transform.rotation = rotation * camera.transform.rotation;
   
}


void Drawing::handleInput() {

    constexpr float cameraMoveSpeed = 5.0f;
    constexpr float cameraRotateSpeed = 0.0001f;

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
    glm::vec3 objectPosition = { 0.0f,0.0f,-70.0f };

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

    //截图
    if (_input.keyboard.keyStates[GLFW_KEY_P] != GLFW_RELEASE) {
        std::cout << "P" << std::endl;
        screenshot.capture();
        screenshot.save("screenshot.png");
    }

    //变焦
    constexpr float scrollSpeed = 0.01f;
    if (_input.keyboard.keyStates[GLFW_KEY_UP] == GLFW_PRESS)
    {
        scaleFactor *=1.1f;
    }

    if (_input.keyboard.keyStates[GLFW_KEY_DOWN] == GLFW_PRESS)
    {
        scaleFactor *=0.9f;
    }

    // 限制缩放因子的范围，例如防止过小或过大的缩放
    if (scaleFactor < 0.1f)
        scaleFactor = 0.1f;
    if (scaleFactor > 10.0f)
        scaleFactor = 10.0f;

    //recover
    if (_input.keyboard.keyStates[GLFW_KEY_ENTER] == GLFW_PRESS) {
        std::cout << "adjust object size" << std::endl;

        glm::vec3 toObject = objectPosition - camera->transform.position;
        // 计算相机当前位置到物体中心的距离
        float distanceToObject = glm::length(toObject);

       scale= distanceToObject /distance;
       distance = distanceToObject;
       setscale();

        return;
    }

    //rotate(没有要求的)
    if (_input.mouse.press.middle == GLFW_PRESS)
    {
        if (_input.mouse.move.xNow != _input.mouse.move.xOld) {

            float mouse_x = _input.mouse.move.xNow - _input.mouse.move.xOld;
            glm::quat original_rotation = camera->transform.rotation;

            if (mouse_x > 0)
            {
                std::cout << "mouse move in x direction +" << std::endl;
                camera->transform.rotation = glm::rotate(camera->transform.rotation, cameraRotateSpeed * mouse_x * 0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                std::cout << "mouse move in x direction -" << std::endl;
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
    }
    


    //自动旋转对准
    if (_input.mouse.press.right == GLFW_PRESS) {
        std::cout << "Right mouse button clicked" << std::endl;

        glm::vec3 cameraToTarget = objectPosition - camera->transform.position;
        glm::quat targetRotation = glm::quatLookAt(glm::normalize(cameraToTarget), camera->transform.getUp());

        // Smoothly interpolate towards the target rotation
        float rotationSpeed = 0.1f;
        camera->transform.rotation = glm::slerp(camera->transform.rotation, targetRotation, rotationSpeed);
    }

    //环绕
    if (_input.keyboard.keyStates[GLFW_KEY_O] != GLFW_RELEASE)
    {
        rotateCameraAroundObject(*camera, objectPosition, 0.1f, getDeltaTime());
    }

    //glm::vec3(0.0f, 1.0f, 0.0f))
    constexpr float directLightMoveSpeed = 0.02f;
    _light->transform.rotation = glm::normalize(
        glm::angleAxis(
            glm::radians(-offset_x * directLightMoveSpeed), 
            _light->transform.getUp())
        * /*_directionalLight->transform.rotation*/ glm::angleAxis(
            glm::radians(45.0f), glm::normalize(glm::vec3(-1.0f))));
    _light->transform.rotation = glm::normalize(
        glm::angleAxis(
            glm::radians(-offset_y * directLightMoveSpeed),
            _light->transform.getRight())
        * /*_directionalLight->transform.rotation*/ glm::angleAxis(
            glm::radians(45.0f), glm::normalize(glm::vec3(-1.0f))));

    constexpr float spotLightMoveSpeed = 5.0f;
    if (_input.keyboard.keyStates[GLFW_KEY_U] != GLFW_RELEASE) {
        _spotLight->transform.position +=
            _spotLight->transform.getFront() * spotLightMoveSpeed * _deltaTime;
    }

    if (_input.keyboard.keyStates[GLFW_KEY_H] != GLFW_RELEASE) {
        _spotLight->transform.position -=
            _spotLight->transform.getRight() * spotLightMoveSpeed * _deltaTime;
    }

    if (_input.keyboard.keyStates[GLFW_KEY_J] != GLFW_RELEASE) {
        _spotLight->transform.position -=
            _spotLight->transform.getFront() * spotLightMoveSpeed * _deltaTime;
    }

    if (_input.keyboard.keyStates[GLFW_KEY_K] != GLFW_RELEASE) {
        _spotLight->transform.position +=
            _spotLight->transform.getRight() * spotLightMoveSpeed * _deltaTime;
    }
}

void Drawing::renderFrame(){
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

    glm::mat4 projection = _cameras[activeCameraIndex]->getProjectionMatrix();
    projection[0][0] /= scaleFactor;
    projection[1][1] /= scaleFactor;

    const glm::mat4 view = _cameras[activeCameraIndex]->getViewMatrix();
    int i;
    i = ((int)(glfwGetTime() * 10.0f)) / 10 % 6;
    mario.setModel(mario_modelRelPath, mario_2, mario_1, i);
   
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

    mario.draw(projection, view, _light, (_cameras[activeCameraIndex])->transform.position, _ambientLight, _spotLight, _material);
    castle.draw(projection, view, _light, (_cameras[activeCameraIndex])->transform.position, _ambientLight, _spotLight, _material);

    // draw skybox
    _skybox->draw(projection, view);

    cube.draw(projection, view, _light, (_cameras[activeCameraIndex])->transform.position, _ambientLight, _spotLight, _material);
    cube1.draw(projection, view, _light, (_cameras[activeCameraIndex])->transform.position, _ambientLight, _spotLight, _material);
    cube2.draw(projection, view, _light, (_cameras[activeCameraIndex])->transform.position, _ambientLight, _spotLight, _material);

    ball.draw(projection, view, _light,(_cameras[activeCameraIndex])->transform.position,_ambientLight,_spotLight,_material);
    pris.draw(projection, view, _light, (_cameras[activeCameraIndex])->transform.position, _ambientLight, _spotLight, _material);
    pyramid.draw(projection, view, _light, (_cameras[activeCameraIndex])->transform.position, _ambientLight, _spotLight, _material);

    land.draw(projection, view, _light, (_cameras[activeCameraIndex])->transform.position, _ambientLight, _spotLight, _material);

    glm::quat rotationDelta = glm::angleAxis(angle_cy, glm::vec3(1.0, 0, 0.0));
    cylinder.transform.rotation = glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f } * rotationDelta;
    cylinder.draw(projection, view, _light, (_cameras[activeCameraIndex])->transform.position, _ambientLight, _spotLight, _material);

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
        ImGui::SliderFloat("intensity_1", &_light->intensity, 0.0f, 3.0f);
        ImGui::ColorEdit3("color_1", (float*)&_light->color);
        ImGui::SliderFloat("offset_x", &offset_x, 0.0f, 3600.0f);
        ImGui::SliderFloat("offset_y", &offset_y, 0.0f, 3600.0f);
        ImGui::NewLine();

        ImGui::Text("ambient light");
        ImGui::Separator();
        ImGui::SliderFloat("intensity_2", &_ambientLight->intensity, 0.0f, 20.0f);
        ImGui::ColorEdit3("color_2", (float*)&_ambientLight->color);
        ImGui::NewLine();

        ImGui::Text("spot light");
        ImGui::Separator();
        ImGui::SliderFloat("intensity_3", &_spotLight->intensity, 0.0f, 10.0f);
        ImGui::ColorEdit3("color_3", (float*)&_spotLight->color);
        ImGui::SliderFloat(
            "angle", (float*)&_spotLight->angle, 0.0f, glm::radians(180.0f), "%f rad");
        ImGui::NewLine();

        ImGui::SliderFloat("length of cube", &cube.transform.scale.x, 10.0f, 20.0f);
        ImGui::NewLine();

        ImGui::SliderFloat("height of cube", &cube.transform.scale.y, 10.0f, 20.0f);
        ImGui::NewLine();

        ImGui::SliderFloat("width of cube", &cube.transform.scale.z, 10.0f, 20.0f);
        ImGui::NewLine();

        ImGui::SliderFloat("Angle", &angle_cy, 0.0f, 1.0f);
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