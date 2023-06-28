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

void Model::setModel(const std::string newmodelRelPath[20], std::string wall2, std::string wall1, int i)
{
    // init textures
    std::string _assetRootDir = "../media/";

    std::shared_ptr<Texture2D> texture1 = std::make_shared<ImageTexture2D>(_assetRootDir + wall1);
    std::shared_ptr<Texture2D> texture2 = std::make_shared<ImageTexture2D>(_assetRootDir + wall2);
    

    model.reset(new newmodel(_assetRootDir + newmodelRelPath[i]));
    

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
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec4 aColor;\n"
        "layout(location = 2) in vec2 aTexCoord;\n"
        "layout(location = 3) in vec3 aNormal;\n"

        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 transform;\n"

        "out vec4 color;\n"
        "out vec2 TexCoord;\n"
        "out vec3 fNormal;\n"
        "out vec3 fPosition;\n"

        "void main() {\n"
        "    fPosition = vec3(transform * vec4(aPos, 1.0f));\n"
        "    gl_Position = projection * view * transform* vec4(aPos, 1.0f);\n"
        "    fNormal = mat3(transpose(inverse(transform))) * aNormal;\n"
        "    color = aColor;\n"
        "    TexCoord = aTexCoord;\n"
        "}\n";

    const char* fsCode =
        "#version 330 core\n"
        "in vec4 color;\n"
        "in vec2 TexCoord;\n"
        "in vec3 fNormal;\n"
        "in vec3 fPosition;\n"

        "struct DirectionalLight {\n"
        "    vec3 direction;\n"
        "    vec3 color;\n"
        "    float intensity;\n"
        "};\n"

        "struct AmbientLight {\n"
        "    vec3 color;\n"
        "    float intensity;\n"
        "};\n"

        "struct SpotLight {\n"
        "    vec3 position;\n"
        "    vec3 direction;\n"
        "    float intensity;\n"
        "    vec3 color;\n"
        "    float angle;\n"
        "    float kc;\n"
        "    float kl;\n"
        "    float kq;\n"
        "};\n"

        "struct Material {\n"
        "    vec3 ka;\n"
        "    vec3 kd;\n"
        "    vec3 ks;\n"         // add specular coefficient
        "    float ns;\n" // add shininess factor
        "};\n"

        "uniform AmbientLight ambientLight;\n"
        "uniform DirectionalLight light;\n"
        "uniform SpotLight spotLight;\n"

        "uniform sampler2D mapKd;\n"
        "uniform vec3 viewPos;\n"
        "uniform Material material;\n"


        "out vec4 outColor;\n"

        "vec3 calcDirectionalLight(vec3 normal, vec3 viewDir) {\n"
        "    vec3 lightDir = normalize(light.direction);\n"
        "    vec3 reflectDir = reflect(-lightDir, normal);\n" // calculate reflection direction for
                                                              // specular
        "    vec3 diffuse = light.color * max(dot(lightDir, normal), 0.0f) * "
        "material.kd;\n"
        "    vec3 specular = material.ks * light.color * pow(max(dot(viewDir, "
        "reflectDir), 0.0), material.ns);\n" // calculate specular light
        "    return light.intensity * (diffuse + specular);\n" // add specular light
        "}\n"

        "vec3 calcSpotLight(vec3 normal, vec3 viewDir) {\n"
        "    vec3 lightDir = normalize(spotLight.position - fPosition);\n"
        "    vec3 reflectDir = reflect(-lightDir, normal);\n" // calculate reflection direction for
                                                              // specular
        "    float theta = acos(-dot(lightDir, normalize(spotLight.direction)));\n"
        "    if (theta > spotLight.angle) {\n"
        "        return vec3(0.0f, 0.0f, 0.0f);\n"
        "    }\n"
        "    vec3 diffuse = spotLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;\n"
        "    vec3 specular = material.ks * spotLight.color * pow(max(dot(viewDir, reflectDir), "
        "0.0), material.ns);\n" // calculate specular light
        "    float distance = length(spotLight.position - fPosition);\n"
        "    float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * "
        "distance * distance);\n"
        "    return spotLight.intensity * attenuation * (diffuse + specular);\n" // add specular
                                                                                 // light
        "}\n"

        "void main() {\n"

        "vec4 Color=texture(mapKd, TexCoord);"
        
        // 计算反射光照强度
        "   vec3 N = normalize(fNormal);"
        "    vec3 L = normalize(-light.direction);"
        //"    float diff = max(dot(N, L), 0.0);"
       // "    vec3 diffuse = diff * light.color * light.intensity * Color.rgb;"

        "    vec3 viewDir = normalize(viewPos - fPosition);\n " // calculate view direction
        "    vec3 diffuseSpecular = calcDirectionalLight(N, viewDir) + calcSpotLight(N, "
        "viewDir);\n"

        // 计算环境光照强度
        "    vec3 ambient = material.ka * ambientLight.color * ambientLight.intensity;\n"

        "    vec3 finalColor = (ambient + diffuseSpecular)*Color.rgb;\n"
        "    outColor = vec4(finalColor, Color.a);"

        "}\n";

    _simpleShader.reset(new GLSLProgram);
    _simpleShader->attachVertexShader(vsCode);
    _simpleShader->attachFragmentShader(fsCode);
    _simpleShader->link();
}

void Model::initBlendShader() {
    const char* vsCode =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec4 aColor;\n"
        "layout(location = 2) in vec2 aTexCoord;\n"
        "layout(location = 3) in vec3 aNormal;\n"

        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 transform;\n"

        "out vec4 color;\n"
        "out vec2 TexCoord;\n"
        "out vec3 fNormal;\n"
        "out vec3 fPosition;\n"

        "void main() {\n"
        "    fPosition = vec3(transform * vec4(aPos, 1.0f));\n"
        "    gl_Position = projection * view * transform* vec4(aPos, 1.0f);\n"
        "    fNormal = mat3(transpose(inverse(transform))) * aNormal;\n"
        "    color = aColor;\n"
        "    TexCoord = aTexCoord;\n"
        "}\n";

    const char* fsCode =
        "#version 330 core\n"
        "in vec4 color;\n"
        "in vec2 TexCoord;\n"
        "in vec3 fNormal;\n"
        "in vec3 fPosition;\n"

        "struct DirectionalLight {\n"
        "    vec3 direction;\n"
        "    vec3 color;\n"
        "    float intensity;\n"
        "};\n"

        "struct AmbientLight {\n"
        "    vec3 color;\n"
        "    float intensity;\n"
        "};\n"

        "struct SpotLight {\n"
        "    vec3 position;\n"
        "    vec3 direction;\n"
        "    float intensity;\n"
        "    vec3 color;\n"
        "    float angle;\n"
        "    float kc;\n"
        "    float kl;\n"
        "    float kq;\n"
        "};\n"

        "struct Material {\n"
        "    vec3 ka;\n"
        "    vec3 kd;\n"
        "    vec3 ks;\n"         // add specular coefficient
        "    float ns;\n" // add shininess factor
        "    vec3 kds[2];\n"//color
        "    float blend;\n"//
        "};\n"

        "uniform AmbientLight ambientLight;\n"
        "uniform DirectionalLight light;\n"
        "uniform SpotLight spotLight;\n"

        "uniform sampler2D mapKd;\n"
        "uniform vec3 viewPos;\n"
        "uniform Material material;\n"
        "uniform sampler2D mapKds[2];\n"

        "out vec4 outColor;\n"

        "vec3 calcDirectionalLight(vec3 normal, vec3 viewDir) {\n"
        "    vec3 lightDir = normalize(light.direction);\n"
        "    vec3 reflectDir = reflect(-lightDir, normal);\n" // calculate reflection direction for
                                                              // specular
        "    vec3 diffuse = light.color * max(dot(lightDir, normal), 0.0f) * "
        "material.kd;\n"
        "    vec3 specular = material.ks * light.color * pow(max(dot(viewDir, "
        "reflectDir), 0.0), material.ns);\n" // calculate specular light
        "    return light.intensity * (diffuse + specular);\n" // add specular light
        "}\n"

        "vec3 calcSpotLight(vec3 normal, vec3 viewDir) {\n"
        "    vec3 lightDir = normalize(spotLight.position - fPosition);\n"
        "    vec3 reflectDir = reflect(-lightDir, normal);\n" // calculate reflection direction for
                                                              // specular
        "    float theta = acos(-dot(lightDir, normalize(spotLight.direction)));\n"
        "    if (theta > spotLight.angle) {\n"
        "        return vec3(0.0f, 0.0f, 0.0f);\n"
        "    }\n"
        "    vec3 diffuse = spotLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;\n"
        "    vec3 specular = material.ks * spotLight.color * pow(max(dot(viewDir, reflectDir), "
        "0.0), material.ns);\n" // calculate specular light
        "    float distance = length(spotLight.position - fPosition);\n"
        "    float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * "
        "distance * distance);\n"
        "    return spotLight.intensity * attenuation * (diffuse + specular);\n" // add specular
                                                                                 // light
        "}\n"

        "void main() {\n"
        // "  color = vec4(material.kds[0], 1.0f);\n"
        // 从两个纹理中采样颜色，且纹理可富有颜色
        "vec4 color1 = texture(mapKds[0], TexCoord)*vec4(material.kds[0],1.0);"
        "vec4 color2 = texture(mapKds[1], TexCoord)*vec4(material.kds[1],1.0);"

        // 混合两个纹理的颜色
        "vec4 blendedColor = mix(color1, color2, material.blend);"

        // 计算反射光照强度
        "   vec3 N = normalize(fNormal);"
        "    vec3 L = normalize(-light.direction);"

        "    vec3 viewDir = normalize(viewPos - fPosition);\n " // calculate view direction
        "    vec3 diffuseSpecular = calcDirectionalLight(N, viewDir) + calcSpotLight(N, "
        "viewDir);\n"

        // 计算环境光照强度
        "    vec3 ambient = material.ka * ambientLight.color * ambientLight.intensity;\n"

        "    vec3 finalColor = (ambient + diffuseSpecular)*blendedColor.rgb;\n"
        "    outColor = vec4(finalColor, blendedColor.a);"

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

void Model::draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light, glm::vec3 position, std::shared_ptr<AmbientLight> _ambientLight, std::shared_ptr<SpotLight> _spotLight, std::shared_ptr<Material> _material) {

    switch (_renderMode) {
    case RenderMode::Simple:
        // 1. use the shader
        _simpleShader->use();
        // 2. transfer mvp matrices to gpu
        _simpleShader->setUniformMat4("projection", projection);
        _simpleShader->setUniformMat4("view", view);
        _simpleShader->setUniformMat4("transform", model->transform.getLocalMatrix());
        _simpleShader->setUniformVec3("viewPos", position);

        _simpleShader->setUniformVec3("light.direction", _light->transform.getFront());
        _simpleShader->setUniformVec3("light.color", _light->color);
        _simpleShader->setUniformFloat("light.intensity", _light->intensity);

        _simpleShader->setUniformVec3("ambientLight.color", _ambientLight->color);
        _simpleShader->setUniformFloat("ambientLight.intensity", _ambientLight->intensity);

        _simpleShader->setUniformVec3("spotLight.position", _spotLight->transform.position);
        _simpleShader->setUniformVec3("spotLight.direction", _spotLight->transform.getFront());
        _simpleShader->setUniformFloat("spotLight.intensity", _spotLight->intensity);
        _simpleShader->setUniformVec3("spotLight.color", _spotLight->color);
        _simpleShader->setUniformFloat("spotLight.angle", _spotLight->angle);
        _simpleShader->setUniformFloat("spotLight.kc", _spotLight->kc);
        _simpleShader->setUniformFloat("spotLight.kl", _spotLight->kl);
        _simpleShader->setUniformFloat("spotLight.kq", _spotLight->kq);

        _simpleShader->setUniformVec3("material.ka", _material->ka);
        _simpleShader->setUniformVec3("material.kd", _material->kd);
        _simpleShader->setUniformVec3("material.ks", _material->ks);
        _simpleShader->setUniformFloat("material.ns", _material->ns);
        // 3. enable textures and transform textures to gpu
        _simpleMaterial->mapKd->bind();

        break;
    case RenderMode::Blend:

        // 1. use the shader
        _blendShader->use();
        // 2. transfer mvp matrices to gpu
        _blendShader->setUniformMat4("projection", projection);
        _blendShader->setUniformMat4("view", view);
        _blendShader->setUniformMat4("transform", model->transform.getLocalMatrix());
        _simpleShader->setUniformVec3("viewPos", position);
        // 3. transfer light attributes to gpu
        _blendShader->setUniformVec3("light.direction", _light->transform.getFront());
        _blendShader->setUniformVec3("light.color", _light->color);
        _blendShader->setUniformFloat("light.intensity", _light->intensity);
      
        _blendShader->setUniformVec3("ambientLight.color", _ambientLight->color);
        _blendShader->setUniformFloat("ambientLight.intensity", _ambientLight->intensity);

        _blendShader->setUniformVec3("spotLight.position", _spotLight->transform.position);
        _blendShader->setUniformVec3("spotLight.direction", _spotLight->transform.getFront());
        _blendShader->setUniformFloat("spotLight.intensity", _spotLight->intensity);
        _blendShader->setUniformVec3("spotLight.color", _spotLight->color);
        _blendShader->setUniformFloat("spotLight.angle", _spotLight->angle);
        _blendShader->setUniformFloat("spotLight.kc", _spotLight->kc);
        _blendShader->setUniformFloat("spotLight.kl", _spotLight->kl);
        _blendShader->setUniformFloat("spotLight.kq", _spotLight->kq);

        // 4. transfer materials to gpu
        // 4.1 transfer simple material attributes
        _blendShader->setUniformVec3("material.ka", _material->ka);
        _blendShader->setUniformVec3("material.kd", _material->kd);
        _blendShader->setUniformVec3("material.ks", _material->ks);
        _blendShader->setUniformFloat("material.ns", _material->ns);
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