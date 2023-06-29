#pragma once
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image_write.h>

class Screenshot {
public:
    Screenshot(int width, int height);

    void capture();

    void save(const std::string& filename);

private:
    static void writeCallback(void* context, void* data, int size) {
        std::stringstream* stream = static_cast<std::stringstream*>(context);
        const char* buffer = static_cast<const char*>(data);
        stream->write(buffer, size);
    }

    int width_;
    int height_;
    std::vector<unsigned char> pixelData_;
};