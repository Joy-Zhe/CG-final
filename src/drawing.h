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
#include"castle.h"
#include "screenShot.h"


class Drawing : public Application {
public:
    Drawing(const Options& options);

    ~Drawing();

private:
    
    float offset_x = 0.0f, offset_y = 0.0f;

    std::shared_ptr<DirectionalLight> _light;
    std::shared_ptr<AmbientLight> _ambientLight;
    std::shared_ptr<SpotLight> _spotLight;

    std::unique_ptr<SkyBox> _skybox;

    //这个东西要和camera的功能连在一起
    float scale=1.0;
    float scale_mush = 1.0;

    Cube cube;
    Cube cube1;
    Cube cube2;
    Cube cube3;
    Cube cube4;
    Cube cube5;

    Cube land;

    //Model mario;
    Model mush[24];
    Model* mushroom;
    Castle castle;

    //the mountains
    Ball ball;
    Ball ball1;
    Ball ball2;
    Cylinder cylinder;
    Cylinder cylinder1;
    Cylinder cylinder2;

    //the small mountains
    Pyramid pyramid;
    Pyramid pyramid1;

    Prismatic pris;
    Prismatic pris1;

    std::vector<std::unique_ptr<Camera>> _cameras;
    std::shared_ptr<Material> _material;
    int activeCameraIndex = 0;

    float cubeangle_x = 0;
    float cubeangle_y = 0;
    float cubeangle_z = 0;

    float scaleFactor = 1.0;
    float distance = 90.0f;
    float angle_cy = 0.0;
    Screenshot screenshot;

    enum RenderMode mario_renderMode = RenderMode::Simple;
    enum RenderMode castle_renderMode = RenderMode::Simple;

    void settransform();
    void setscale();

    void handleInput() override;

    void renderFrame() override;
};