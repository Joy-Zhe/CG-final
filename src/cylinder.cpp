#include "cylinder.h"
#include <iostream>


Cylinder::Cylinder(std::string texture_path) {

    setdata();
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

        "void main() {\n"
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

        "struct DirectionalLight {\n"
        "    vec3 direction;\n"
        "    vec3 color;\n"
        "    float intensity;\n"
        "};\n"

        "uniform DirectionalLight light;\n"
        "uniform sampler2D texture1;\n"

        "out vec4 outColor;\n"

        "void main() {\n"

        "vec4 Color=texture(texture1, TexCoord);"

        // 计算反射光照强度
        "   vec3 N = normalize(fNormal);"
        "    vec3 L = normalize(-light.direction);"
        "    float diff = max(dot(N, L), 0.0);"
        "    vec3 diffuse = diff * light.color * light.intensity * Color.rgb;"

        // 计算环境光照强度
        "    vec3 ambient = light.color * light.intensity*10 * 0.1 * Color.rgb;"

        "    vec3 finalColor = ambient + diffuse;"
        "    outColor = vec4(finalColor, Color.a);"

        "}\n";

    _shader.reset(new GLSLProgram());
    _shader->attachVertexShader(vsCode);
    _shader->attachFragmentShader(fsCode);

    _shader->link();
}

Cylinder::~Cylinder() {
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}


void Cylinder::draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light) {

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

    _shader->setUniformVec3("light.direction", _light->transform.getFront());
    _shader->setUniformVec3("light.color", _light->color);
    _shader->setUniformFloat("light.intensity", _light->intensity);


    // 绘制代码
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

}


void Cylinder::setdata()
{
    const int x_segment = 10;
    const int y_segment = 10;

    // 生成顶点数组和索引数组
    const int vertexCount = (x_segment + 1) * (y_segment + 1) + 2; // 顶点数量
    const int indexCount = x_segment * y_segment * 6 + x_segment * 3; // 索引数量

    // 生成顶面和底面顶点
    float angleIncrement = 2.0f * glm::pi<float>() / x_segment;
    float currentAngle = 0.0f;

    glm::vec3 topCenterVertex = glm::vec3(0.0f, height / 2.0f, 0.0f);
    glm::vec3 bottomCenterVertex = glm::vec3(0.0f, -height / 2.0f, 0.0f);

    vertices[0].position = topCenterVertex; // 顶面顶点
    vertices[0].color = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[0].texCoord = glm::vec2(0.5f, 0.5f); // 纹理坐标

    vertices[1].position = bottomCenterVertex; // 底面顶点
    vertices[1].color = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[1].texCoord = glm::vec2(0.5f, 0.5f); // 纹理坐标

    // 生成侧面顶点
    int vertexIndex = 2;
    for (int y = 0; y <= y_segment; y++)
    {
        for (int x = 0; x <= x_segment; x++)
        {
            float xSegment = (float)x / (float)x_segment;
            float ySegment = (float)y / (float)y_segment;
            float xPos = radius * std::cos(xSegment * 2.0f * glm::pi<float>());
            float zPos = radius * std::sin(xSegment * 2.0f * glm::pi<float>());
            float yPos = height * (y / (float)y_segment - 0.5f);

            vertices[vertexIndex].position = glm::vec3(xPos, yPos, zPos);
            vertices[vertexIndex].color = glm::vec3(0.0f, 0.0f, 0.0f);
            vertices[vertexIndex].texCoord = glm::vec2(xSegment, ySegment);
            vertexIndex++;
        }
    }

    // 生成顶面索引
    int index = 0;
    for (int x = 0; x < x_segment; x++)
    {
        indices[index++] = 0;
        indices[index++] = x + 2;
        indices[index++] = x + 1;
    }

    // 生成底面索引
    for (int x = 0; x < x_segment; x++)
    {
        indices[index++] = 1;
        indices[index++] = (y_segment - 1) * (x_segment + 1) + x + 1;
        indices[index++] = (y_segment - 1) * (x_segment + 1) + x + 2;
    }

    // 生成侧面索引
    for (int y = 0; y < y_segment; y++)
    {
        for (int x = 0; x < x_segment; x++)
        {
            int currentVertex = y * (x_segment + 1) + x + 2;

            indices[index++] = currentVertex;
            indices[index++] = currentVertex + x_segment + 1;
            indices[index++] = currentVertex + 1;

            indices[index++] = currentVertex + 1;
            indices[index++] = currentVertex + x_segment + 1;
            indices[index++] = currentVertex + x_segment + 2;
        }
    }

  
}
