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

    // init shaders
    initSimpleShader();

}

Model::Model(const Model& m) {
	_simpleMaterial = std::move(m._simpleMaterial);
	model = std::move(m.model);
	_simpleShader = std::move(m._simpleShader);
	_renderMode = m._renderMode;
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


void Model::setRenderMode(RenderMode mode)
{
    _renderMode = mode;
}

void Model::draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light, glm::vec3 position, std::shared_ptr<AmbientLight> _ambientLight, std::shared_ptr<SpotLight> _spotLight, std::shared_ptr<Material> _material) {

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
        _simpleMaterial->mapKd->bind(0);
        _simpleShader->setUniformInt("mapKd", 0);

    model->draw();
}