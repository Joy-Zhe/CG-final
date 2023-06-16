#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "model.h"
#include"drawing.h"


Model::Model(std::string newmodelRelPath, std::string wall2, std::string wall1){

    // init textures
    std::string _assetRootDir = "../media/";

    std::shared_ptr<Texture2D> texture1 = std::make_shared<ImageTexture2D>(_assetRootDir + wall1);
    std::shared_ptr<Texture2D> texture2 = std::make_shared<ImageTexture2D>(_assetRootDir + wall2);

    model.reset(new newmodel(_assetRootDir + newmodelRelPath));

    // init materials
    _simpleMaterial.reset(new SimpleMaterial);
    _simpleMaterial->mapKd = texture2;

    _blendMaterial.reset(new BlendMaterial);
    _blendMaterial->kds[0] = glm::vec3(1.0f, 1.0f, 1.0f);
    _blendMaterial->kds[1] = glm::vec3(1.0f, 1.0f, 1.0f);
    _blendMaterial->mapKds[0] = texture2;
    _blendMaterial->mapKds[1] = texture1;
    _blendMaterial->blend = 0.0f;

    // init shaders
    initSimpleShader();
    initBlendShader();

}

Model::~Model() {
   
}

void Model::initSimpleShader() {
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

void Model::initBlendShader() {
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

void Model::setRenderMode(RenderMode mode)
{
    _renderMode = mode;
}

void Model::draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light) {

    switch (_renderMode) {
    case RenderMode::Simple:
        // 1. use the shader
        _simpleShader->use();
        // 2. transfer mvp matrices to gpu
        _simpleShader->setUniformMat4("projection", projection);
        _simpleShader->setUniformMat4("view", view);
        _simpleShader->setUniformMat4("model", model->transform.getLocalMatrix());
        // 3. enable textures and transform textures to gpu
        _simpleMaterial->mapKd->bind();
        break;
    case RenderMode::Blend:

        // 1. use the shader
        _blendShader->use();
        // 2. transfer mvp matrices to gpu
        _blendShader->setUniformMat4("projection", projection);
        _blendShader->setUniformMat4("view", view);
        _blendShader->setUniformMat4("model", model->transform.getLocalMatrix());
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

    model->draw();
}