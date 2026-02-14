#pragma once

#include "vec3.h"
#include "Ray.h"

class Camera {
    public:
        Camera() : position(0, 0, 0), U(1, 0, 0), V(0, 1, 0), W(0, 0, 1),
        focalLength(1.0f), imageplaneWidth(0.25f), imageplaneHeight(0.25f)
         {}

        Camera(int pixel_nx, int pixel_ny )
            : position(0, 0, 0), U(1, 0, 0), V(0, 1, 0), W(0, 0, 1),
            focalLength(1.0f), imageplaneWidth(0.25f), imageplaneHeight(0.25f),
            nx(pixel_nx), ny(pixel_ny)
         {
            imageplaneWidth= 0.25f;
            float aspectRatio = static_cast<float>(nx) / static_cast<float>(ny);
            imageplaneHeight = imageplaneWidth / aspectRatio;
         }

        virtual ~Camera() {}

        virtual Ray generateRay( float i, float j) = 0;

        vec3 getPos() const { return position; }

    protected:
        vec3 position;

        // basis vectors
        vec3 U, V, W;

        float focalLength;

        float imageplaneWidth, imageplaneHeight;
        int nx, ny;

};
