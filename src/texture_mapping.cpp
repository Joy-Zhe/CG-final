#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "texture_mapping.h"

//接口
const std::string modelRelPath = "obj/sphere.obj";
const std::string newmodelRelPath = "obj/mario (2).obj";
const std::string wall2 = "texture/miscellaneous/Frame 34.jpg";
const std::string wall1= "texture/miscellaneous/F.jpg";

//bk ft exchange
const std::vector<std::string> skyboxTextureRelPaths = {
    "texture/skybox/gloomy_rt.png", "texture/skybox/gloomy_lf.png",  "texture/skybox/gloomy_up.png",
    "texture/skybox/gloomy_dn.png",  "texture/skybox/gloomy_bk.png", "texture/skybox/gloomy_ft.png"};

TextureMapping::TextureMapping(const Options& options) : Application(options) {
    // init model

    mario.reset(new newmodel(getAssetFullPath(newmodelRelPath)));
    mario->transform.scale = glm::vec3(0.08f,0.08f, 0.08f);
    mario->transform.position = glm::vec3(3.0f, -4.0f, -2.0f);
    mario->transform.rotation= { 1.0f, 0.0f, 0.0f, 0.0f };
    // init textures
    std::shared_ptr<Texture2D> earthTexture =
        std::make_shared<ImageTexture2D>(getAssetFullPath(wall1));
    std::shared_ptr<Texture2D> planetTexture =
        std::make_shared<ImageTexture2D>(getAssetFullPath(wall2));

    // init materials
    _simpleMaterial.reset(new SimpleMaterial);
    _simpleMaterial->mapKd = planetTexture;

    _blendMaterial.reset(new BlendMaterial);
    _blendMaterial->kds[0] = glm::vec3(1.0f, 1.0f, 1.0f);
    _blendMaterial->kds[1] = glm::vec3(1.0f, 1.0f, 1.0f);
    _blendMaterial->mapKds[0] = planetTexture;
    _blendMaterial->mapKds[1] = earthTexture;
    _blendMaterial->blend = 0.0f;

    _checkerMaterial.reset(new CheckerMaterial);
    _checkerMaterial->repeat = 10;
    _checkerMaterial->colors[0] = glm::vec3(1.0f, 1.0f, 1.0f);
    _checkerMaterial->colors[1] = glm::vec3(0.0f, 0.0f, 0.0f);

    
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

    // init shaders
    initSimpleShader();
    initBlendShader();

    // init imGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init();
}

TextureMapping::~TextureMapping() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void TextureMapping::initSimpleShader() {
    const char* vsCode =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPosition;\n"
        "layout(location = 1) in vec3 aNormal;\n"
        "layout(location = 2) in vec2 aTexCoord;\n"
        "out vec2 fTexCoord;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"

        "void main() {\n"
        "    fTexCoord = aTexCoord;\n"
        "    gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
        "}\n";

    const char* fsCode =
        "#version 330 core\n"
        "in vec2 fTexCoord;\n"
        "out vec4 color;\n"
        "uniform sampler2D mapKd;\n"
        "void main() {\n"
        "    color = texture(mapKd, fTexCoord);\n"
        "}\n";

    _simpleShader.reset(new GLSLProgram);
    _simpleShader->attachVertexShader(vsCode);
    _simpleShader->attachFragmentShader(fsCode);
    _simpleShader->link();
}

void TextureMapping::initBlendShader() {
    const char* vsCode =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPosition;\n"
        "layout(location = 1) in vec3 aNormal;\n"
        "layout(location = 2) in vec2 aTexCoord;\n"

        "out vec3 fPosition;\n"
        "out vec3 fNormal;\n"
        "out vec2 fTexCoord;\n"

        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"

        "void main() {\n"
        "    fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
        "    fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
        "    fTexCoord = aTexCoord;\n"
        "    gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
        "}\n";

    const char* fsCode =
        "#version 330 core\n"
        "in vec3 fPosition;\n"
        "in vec3 fNormal;\n"//
        "in vec2 fTexCoord;\n"//
        "out vec4 color;\n"//

        "struct DirectionalLight {\n"
        "    vec3 direction;\n"//
        "    vec3 color;\n"
        "    float intensity;\n"//
        "};\n"

        "struct Material {\n"
        "    vec3 kds[2];\n"//color
        "    float blend;\n"//
        "};\n"

        "uniform Material material;\n"
        "uniform DirectionalLight light;\n"
        "uniform sampler2D mapKds[2];\n"

        "void main() {\n"
       // "  color = vec4(material.kds[0], 1.0f);\n"
        // 从两个纹理中采样颜色，且纹理可富有颜色
        "vec4 color1 = texture(mapKds[0], fTexCoord)*vec4(material.kds[0],1.0);"
        "vec4 color2 = texture(mapKds[1], fTexCoord)*vec4(material.kds[1],1.0);"

         // 混合两个纹理的颜色
        "vec4 blendedColor = mix(color1, color2, material.blend);"

        // 计算反射光照强度
        "   vec3 N = normalize(fNormal);"
        "    vec3 L = normalize(-light.direction);"
        "    float diff = max(dot(N, L), 0.0);"
        "    vec3 diffuse = diff * light.color * light.intensity * blendedColor.rgb;"

        // 计算环境光照的影响
        "    vec3 ambient = light.color * light.intensity * 0.1 * blendedColor.rgb;"

        // 计算最终颜色
        "    vec3 finalColor = ambient + diffuse;"
        "    color = vec4(finalColor, blendedColor.a);"
       
        "}\n";

    _blendShader.reset(new GLSLProgram);
    _blendShader->attachVertexShader(vsCode);
    _blendShader->attachFragmentShader(fsCode);
    _blendShader->link();

}


void TextureMapping::handleInput() {

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

    const float angluarVelocity = 0.1f;
    const float angle = angluarVelocity * static_cast<float>(_deltaTime);
    const glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
    mario->transform.rotation = glm::angleAxis(angle, axis) * mario->transform.rotation;
}

void TextureMapping::renderFrame() {
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

    // draw planet
    switch (_renderMode) {
    case RenderMode::Simple:
        // 1. use the shader
        _simpleShader->use();
        // 2. transfer mvp matrices to gpu
        _simpleShader->setUniformMat4("projection", projection);
        _simpleShader->setUniformMat4("view", view);
        _simpleShader->setUniformMat4("model", mario->transform.getLocalMatrix());
        // 3. enable textures and transform textures to gpu
        _simpleMaterial->mapKd->bind();
        break;
    case RenderMode::Blend:

        // 1. use the shader
        _blendShader->use();
        // 2. transfer mvp matrices to gpu
        _blendShader->setUniformMat4("projection", projection);
        _blendShader->setUniformMat4("view", view);
        _blendShader->setUniformMat4("model", mario->transform.getLocalMatrix());
        // 3. transfer light attributes to gpu
        _blendShader->setUniformVec3("light.direction", _light->transform.getFront());
        _blendShader->setUniformVec3("light.color", _light->color);
        _blendShader->setUniformFloat("light.intensity", _light->intensity);
        // 4. transfer materials to gpu
        // 4.1 transfer simple material attributes
        _blendShader->setUniformVec3("material.kds[0]", _blendMaterial->kds[0]);
        _blendShader->setUniformVec3("material.kds[1]", _blendMaterial->kds[1]);
        // 4.2 transfer blend cofficient to gpu
        _blendShader->setUniformFloat("material.blend", _blendMaterial->blend);

        // 4.3 TODO: enable textures and transform textures to gpu

        _blendMaterial->mapKds[0]->bind(0);
        _blendShader->setUniformInt("mapKds[0]", 0);

        _blendMaterial->mapKds[1]->bind(1);
        _blendShader->setUniformInt("mapKds[1]", 1);
     
        break;
    
    }

    mario->draw();

    // draw skybox
    _skybox->draw(projection, view);

    tri.draw(projection,view);
    
    // draw ui elements
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

    if (!ImGui::Begin("Control Panel", nullptr, flags)) {
        ImGui::End();
    }
    else {
        ImGui::Text("Render Mode");
        ImGui::Separator();
        ImGui::RadioButton("Simple Texture Shading", (int*)&_renderMode, (int)(RenderMode::Simple));
        ImGui::NewLine();

        ImGui::RadioButton("Blend Texture Shading", (int*)&_renderMode, (int)(RenderMode::Blend));
        ImGui::ColorEdit3("kd1", (float*)&_blendMaterial->kds[0]);
        ImGui::ColorEdit3("kd2", (float*)&_blendMaterial->kds[1]);
        ImGui::SliderFloat("blend", &_blendMaterial->blend, 0.0f, 1.0f);
        ImGui::NewLine();


        ImGui::Text("Directional light");
        ImGui::Separator();
        ImGui::SliderFloat("intensity", &_light->intensity, 0.0f, 2.0f);
        ImGui::ColorEdit3("color", (float*)&_light->color);
        ImGui::NewLine();

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
