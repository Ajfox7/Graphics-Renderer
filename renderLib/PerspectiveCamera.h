#pragma once
#include "Camera.h"

class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera()
            : Camera() {}

        PerspectiveCamera( int pixel_nx, int pixel_ny )
            : Camera( pixel_nx, pixel_ny ) {}

        PerspectiveCamera( vec3 origin, vec3 viewDir, vec3 upDir,
                           float fLength, float imgPlaneWidth, float imgPlaneHeight,
                           int pixel_nx, int pixel_ny )
            : Camera( pixel_nx, pixel_ny )
        {
            position = origin;
            vec3 up = unit_vector(upDir);
            W = -unit_vector(viewDir);
            U = unit_vector(cross(up, W));
            V = cross( W, U );

            focalLength = fLength;
            imageplaneWidth = imgPlaneWidth;
            imageplaneHeight = imgPlaneHeight;
        }

        Ray generateRay( float i, float j ) override {
            float u = ( (i + 0.5f) / nx - 0.5f ) * imageplaneWidth;
            float v = ( (j + 0.5f) / ny - 0.5f ) * imageplaneHeight;

            return Ray(position, -W*focalLength + U*u + V*v);
        }
};