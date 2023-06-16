#pragma once

#include <memory>
#include <string>

#include "base/application.h"
#include "base/camera.h"
#include "base/glsl_program.h"
#include "base/light.h"
#include "base/skybox.h"
#include "base/texture2d.h"
#include "base/newmodel.h"
#include "cube.h"

enum class RenderMode {
    Simple,
    Blend,
    Checker
};

struct BlendMaterial {
    glm::vec3 kds[2];
    std::shared_ptr<Texture2D> mapKds[2];
    float blend;
};


class Model{
public:
    std::unique_ptr<SimpleMaterial> _simpleMaterial;
    std::unique_ptr<BlendMaterial> _blendMaterial;
    std::unique_ptr<newmodel> model;

    Model(std::string newmodelRelPath, std::string wall2, std::string wall1);

    void draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light);

    void setRenderMode(RenderMode mode);

    ~Model();

private:
  
    std::unique_ptr<GLSLProgram> _simpleShader;
    std::unique_ptr<GLSLProgram> _blendShader;

    enum RenderMode _renderMode = RenderMode::Simple;

    void initSimpleShader();

    void initBlendShader();
};
