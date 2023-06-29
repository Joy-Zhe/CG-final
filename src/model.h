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
    Material
};



class Model{
public:
    std::shared_ptr<SimpleMaterial> _simpleMaterial;
    std::shared_ptr<newmodel> model;

    Model(std::string newmodelRelPath, std::string wall2, std::string wall1);
    Model(const Model& m);
    void draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light, glm::vec3 position, std::shared_ptr<AmbientLight> _ambientLight, std::shared_ptr<SpotLight> _spotLight, std::shared_ptr<Material> _material);

    void setRenderMode(RenderMode mode);
   
    ~Model();

private:
  
    std::shared_ptr<GLSLProgram> _simpleShader;

    enum RenderMode _renderMode = RenderMode::Simple;

    void initSimpleShader();
};
