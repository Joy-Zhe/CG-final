#include "prismatic.h"

#include <iostream>


void Prismatic::generateHexagonalPrism()
{
    float radius = 1.0f; // 六棱柱的半径
    float height = 2.0f; // 六棱柱的高度
    glm::vec3 color(0.0f, 0.0f, 0.0f); // 顶点颜色

    // 生成顶面顶点
    vertex_tri topCenterVertex;
    topCenterVertex.position = glm::vec3(0.0f, 0.0f, height / 2.0f);
    topCenterVertex.color = color;
    topCenterVertex.texCoord = glm::vec2(0.5f, 0.5f);
    vertices[0] = topCenterVertex;

    // 生成底面顶点
    vertex_tri bottomCenterVertex;
    bottomCenterVertex.position = glm::vec3(0.0f, 0.0f, -height / 2.0f);
    bottomCenterVertex.color = color;
    bottomCenterVertex.texCoord = glm::vec2(0.5f, 0.5f);
    vertices[1] = bottomCenterVertex;

    // 生成侧面顶点
    for (int i = 0; i < 6; i++)
    {
        float angle = i * glm::pi<float>() / 3.0f;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);

        // 顶面顶点
        vertex_tri topVertex;
        topVertex.position = glm::vec3(x, y, height / 2.0f);
        topVertex.color = color;
        topVertex.texCoord = glm::vec2(static_cast<float>(i) / 6.0f, 1.0f);
        vertices[2 + i * 2] = topVertex;

        // 底面顶点
        vertex_tri bottomVertex;
        bottomVertex.position = glm::vec3(x, y, -height / 2.0f);
        bottomVertex.color = color;
        bottomVertex.texCoord = glm::vec2(static_cast<float>(i) / 6.0f, 0.0f);
        vertices[3 + i * 2] = bottomVertex;
    }

    // 生成顶面索引
    for (int i = 0; i < 6; i++)
    {
        indices[i * 3] = 0;
        indices[i * 3 + 1] = 2 + i * 2;
        indices[i * 3 + 2] = 2 + ((i + 1) % 6) * 2;
    }

    // 生成底面索引
    for (int i = 0; i < 6; i++)
    {
        indices[18 + i * 3] = 1;
        indices[18 + i * 3 + 1] = 3 + ((i + 1) % 6) * 2;
        indices[18 + i * 3 + 2] = 3 + i * 2;
    }

    // 生成侧面索引
    for (int i = 0; i < 6; i++)
    {
        indices[36 + i * 6] = 2 + i * 2;
        indices[36 + i * 6 + 1] = 2 + ((i + 1) % 6) * 2;
        indices[36 + i * 6 + 2] = 3 + i * 2;
        indices[36 + i * 6 + 3] = 3 + i * 2;
        indices[36 + i * 6 + 4] = 2 + ((i + 1) % 6) * 2;
        indices[36 + i * 6 + 5] = 3 + ((i + 1) % 6) * 2;
    }

}


Prismatic::Prismatic(std::string texture_path) {

    generateHexagonalPrism();
    // init textures
    std::string _assetRootDir = "../media/";
    std::shared_ptr<Texture2D> texture = std::make_shared<ImageTexture2D>(_assetRootDir + texture_path);

    simpleMaterial.reset(new SimpleMaterial);
    simpleMaterial->mapKd = texture;

    // create a vertex array object
    glGenVertexArrays(1, &_vao);
    // create a vertex buffer object
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // specify layout, size of a vertex, data type, normalize, sizeof vertex array, offset of the
    // attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_tri), (void*)offsetof(vertex_tri, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_tri), (void*)offsetof(vertex_tri, color));
    glEnableVertexAttribArray(1);

    // 启用纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_tri), (void*)offsetof(vertex_tri, texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    //load texture
    glGenTextures(1, &_textureID);
    //glBindTexture(GL_TEXTURE_2D, _textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load((_assetRootDir + texture_path).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }


    // create shader
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
        "uniform sampler2D texture1;\n"
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

        "vec4 Color=texture(texture1, TexCoord);"

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

    _shader.reset(new GLSLProgram());
    _shader->attachVertexShader(vsCode);
    _shader->attachFragmentShader(fsCode);

    _shader->link();
}

Prismatic::~Prismatic() {
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}


void Prismatic::draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light, glm::vec3 position, std::shared_ptr<AmbientLight> _ambientLight, std::shared_ptr<SpotLight> _spotLight, std::shared_ptr<Material> _material) {

    _shader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    // 设置纹理采样器的uniform值
    _shader->setUniformInt("texture1", 1);
    // enable textures and transform textures to gpu
    simpleMaterial->mapKd->bind(1);
    _shader->setUniformMat4("projection", projection);
    _shader->setUniformMat4("view", view);
    _shader->setUniformMat4("transform", transform.getLocalMatrix());
    _shader->setUniformVec3("viewPos", position);

    _shader->setUniformVec3("light.direction", _light->transform.getFront());
    _shader->setUniformVec3("light.color", _light->color);
    _shader->setUniformFloat("light.intensity", _light->intensity);

    _shader->setUniformVec3("ambientLight.color", _ambientLight->color);
    _shader->setUniformFloat("ambientLight.intensity", _ambientLight->intensity);

    _shader->setUniformVec3("spotLight.position", _spotLight->transform.position);
    _shader->setUniformVec3("spotLight.direction", _spotLight->transform.getFront());
    _shader->setUniformFloat("spotLight.intensity", _spotLight->intensity);
    _shader->setUniformVec3("spotLight.color", _spotLight->color);
    _shader->setUniformFloat("spotLight.angle", _spotLight->angle);
    _shader->setUniformFloat("spotLight.kc", _spotLight->kc);
    _shader->setUniformFloat("spotLight.kl", _spotLight->kl);
    _shader->setUniformFloat("spotLight.kq", _spotLight->kq);

    _shader->setUniformVec3("material.ka", _material->ka);
    _shader->setUniformVec3("material.kd", _material->kd);
    _shader->setUniformVec3("material.ks", _material->ks);
    _shader->setUniformFloat("material.ns", _material->ns);


    // 绘制代码
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

}