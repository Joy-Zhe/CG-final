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
#include"Ball.h"

class Drawing : public Application {
public:
    Drawing(const Options& options);

    ~Drawing();

private:
    
    std::shared_ptr<DirectionalLight> _light;

    std::unique_ptr<SkyBox> _skybox;
    float scale=10.0;

    Cube cube;
    Cube cube1;
    Cube cube2;
    Cube land;

    Model mario;
    Model castle;

    Ball ball;

    std::vector<std::unique_ptr<Camera>> _cameras;
    int activeCameraIndex = 0;

    enum RenderMode mario_renderMode = RenderMode::Simple;
    enum RenderMode castle_renderMode = RenderMode::Simple;

    void settransform();

    void handleInput() override;

    void renderFrame() override;
};