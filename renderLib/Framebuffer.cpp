#include "Framebuffer.h"
#include "png++/png.hpp"
#include <algorithm>

Framebuffer::Framebuffer(int width, int height) {
    this->width = width;
    this->height = height;
    fbStorage.resize(width * height);
}

Framebuffer::Framebuffer() : Framebuffer(800, 600) {
    // Default size 800x600
}

void Framebuffer::clear( vec3 color ) {
    for(auto idx=0u; idx<fbStorage.size(); ++idx){
        fbStorage[idx] = color;
    }
}

void Framebuffer::clearGradient( vec3 topColor, vec3 bottomColor ) {
    for(auto x=0; x<width; ++x){
        for(auto y=0; y<height; ++y){
            float t = y / (float)height;
            vec3 blendedColor = topColor * t + bottomColor * (1.0f - t);
            fbStorage[y * width + x] = blendedColor;
        }
    }
}

void Framebuffer::setPixelColor(int x, int y, vec3 color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        fbStorage[y * width + x] = color;
    }
}

void Framebuffer::exportAsPNG( std::string filename ) {
    png::image< png::rgb_pixel > imData(width, height);
    for (unsigned int idx=0; idx<imData.get_height()*imData.get_width(); ++idx)
    {
        size_t x = idx % width;
        size_t y = static_cast<size_t>( floor(idx / static_cast<float>(imData.get_width())) );

        vec3 color = fbStorage[idx];
        png::byte r = static_cast<png::byte>(std::clamp( color.x() * 255.0f, 0.0f, 255.0f ));
        png::byte g = static_cast<png::byte>(std::clamp( color.y() * 255.0f, 0.0f, 255.0f ));
        png::byte b = static_cast<png::byte>(std::clamp( color.z() * 255.0f, 0.0f, 255.0f ));

        imData[imData.get_height() - 1 -y][x] = png::rgb_pixel( r, g, b );
    }
    imData.write( filename );
}
