#include "screenShot.h"

Screenshot::Screenshot(int width, int height) : width_(width), height_(height) {
    pixelData_.resize(width_ * height_ * 3);
}

void Screenshot::capture() {
    //// ��ȡ֡�������������
    //glReadPixels(0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelData_.data());

    //// ��תͼ�����ݣ���ΪOpenGL����ϵ��ԭ�������½ǣ�
    //for (int y = 0; y < height_ / 2; y++) {
    //    for (int x = 0; x < width_; x++) {
    //        for (int c = 0; c < 3; c++) {
    //            std::swap(pixelData_[(y * width_ + x) * 3 + c], pixelData_[((height_ - y - 1) * width_ + x) * 3 + c]);
    //        }
    //    }
    //}
}

void Screenshot::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to save screenshot: " << filename << std::endl;
        return;
    }

    std::stringstream stream;
    stbi_write_png_to_func(writeCallback, &stream, width_, height_, 3, pixelData_.data(), width_ * 3);

    file << stream.rdbuf();

    std::cout << "Screenshot saved: " << filename << std::endl;
}