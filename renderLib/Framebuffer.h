#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "vec3.h"
#include <vector>

class Framebuffer{
    public:
        Framebuffer();
        Framebuffer(int width, int height);
        void clear( vec3 color );
        void clearGradient( vec3 topColor, vec3 bottomColor );
        void exportAsPNG( std::string filename );
        void setPixelColor(int x, int y, vec3 color);

        int getWidth(){return this->width;}
        int getHeight(){return this->height;}

    private:
        int width;
        int height;
        std::vector< vec3 > fbStorage;
};

#endif