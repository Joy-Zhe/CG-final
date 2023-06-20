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
#include"prismatic.h"
#include"pyramid.h"
#include"cylinder.h"


class Drawing : public Application {
public:
    Drawing(const Options& options);

    ~Drawing();

private:
    
    std::shared_ptr<DirectionalLight> _light;

    std::unique_ptr<SkyBox> _skybox;

    //这个东西要和camera的功能连在一起
    float scale=1.0;

    Cube cube;
    Cube cube1;
    Cube cube2;
    Cube land;

    Model mario;
    Model castle;

    Ball ball;
    Prismatic pris;
    Pyramid pyramid;
    Cylinder cylinder;

    std::vector<std::unique_ptr<Camera>> _cameras;
    int activeCameraIndex = 0;

    float scaleFactor = 1.0;
    float distance = 90.0f;
    float angle_cy = 0.0;

    enum RenderMode mario_renderMode = RenderMode::Simple;
    enum RenderMode castle_renderMode = RenderMode::Simple;

    void settransform();
    void setscale();

    void handleInput() override;

    void renderFrame() override;
};
