#include "Framebuffer.h"
#include "png++/png.hpp"
#include <algorithm>
#include "PerspectiveCamera.h"

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

void Framebuffer::SetPixelColor(int x, int y, vec3 color) {
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

        // The origin for indexing the height is in lower left...
        imData[y][x] = png::rgb_pixel( r, g, b );
    }
    imData.write( filename );
}

int main(int argc, char* argv[]) {
    //Framebuffer fb1(1024, 768);
    //fb1.clearGradient(vec3(0,1.0,0.3), vec3(0.7,0.2,0.3));
    //fb1.exportAsPNG("output.png");

    Framebuffer fb;
    PerspectiveCamera p(800,600);

    for(int j=0; j<600; ++j){
        for(int i=0; i<800; ++i){
            Ray r = p.generateRay((float)i, (float)j);

            //convert Ray direction to color
            vec3 d = unit_vector(r.direction());
            vec3 color = vec3( 0.5f*(d.x()+1.0f), 0.5f*(d.y()+1.0f), 0.5f*(d.z()+1.0f) );
            fb.SetPixelColor(i, j, color);
        }
    }
    fb.exportAsPNG("perspective_camera_output.png");

    Framebuffer fb2(200,200);
    PerspectiveCamera p2( vec3(1,0,1), vec3(1,0,-1), vec3(0,1,0),
                             0.75f, 0.5f, 0.5f,
                             200, 200 );
    for(int j=0; j<200; ++j){
        for(int i=0; i<200; ++i){
            Ray r = p2.generateRay((float)i, (float)j);

            //convert Ray direction to color
            vec3 d = unit_vector(r.direction());
            vec3 color = vec3( 0.5f*(d.x()+1.0f), 0.5f*(d.y()+1.0f), 0.5f*(d.z()+1.0f) );
            fb2.SetPixelColor(i, j, color);
        }
    }
    fb2.exportAsPNG("perspective_camera_output_200x200.png");

}