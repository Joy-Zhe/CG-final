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
#include "simplefigure.h"

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

struct CheckerMaterial {
    int repeat;
    glm::vec3 colors[2];
};

class TextureMapping : public Application {
public:
    TextureMapping(const Options& options);

    ~TextureMapping();

private:
    //model
    std::unique_ptr<newmodel> mario;

    std::unique_ptr<SimpleMaterial> _simpleMaterial;
    std::unique_ptr<BlendMaterial> _blendMaterial;
    std::unique_ptr<CheckerMaterial> _checkerMaterial;

   // std::unique_ptr<PerspectiveCamera> _camera;
    std::unique_ptr<DirectionalLight> _light;

    std::unique_ptr<GLSLProgram> _simpleShader;
    std::unique_ptr<GLSLProgram> _blendShader;
    std::unique_ptr<GLSLProgram> _checkerShader;

    std::unique_ptr<SkyBox> _skybox;
    Triangle tri;

    std::vector<std::unique_ptr<Camera>> _cameras;
    int activeCameraIndex = 0;

    enum RenderMode _renderMode = RenderMode::Simple;

    void initSimpleShader();

    void initBlendShader();

    void handleInput() override;

    void renderFrame() override;
};
