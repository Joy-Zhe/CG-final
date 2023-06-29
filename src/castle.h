#pragma once
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
#include"model.h"


class Castle {
public:
    std::unique_ptr<SimpleMaterial> _simpleMaterial;
    std::unique_ptr<newmodel> model;

    Castle(std::string newmodelRelPath, std::string wall2, std::string wall1);

    void draw(const glm::mat4& projection, const glm::mat4& view, std::shared_ptr<DirectionalLight> _light, glm::vec3 position, std::shared_ptr<AmbientLight> _ambientLight, std::shared_ptr<SpotLight> _spotLight, std::shared_ptr<Material> _material);

    void setRenderMode(RenderMode mode);
   // void setModel(const std::string newmodelRelPath[20], std::string wall2, std::string wall1, int i, enum RenderMode mario_renderMode);

    ~Castle();

private:

    std::unique_ptr<GLSLProgram> _simpleShader;
    std::unique_ptr<GLSLProgram>_MaterialShader;

    enum RenderMode _renderMode = RenderMode::Simple;

    void initSimpleShader();

    void initMaterialShader();
};
