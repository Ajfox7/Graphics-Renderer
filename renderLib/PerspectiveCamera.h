#pragma once

#include "Camera.h"
#include "vec3.h"

class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera()
            : Camera() {}

        PerspectiveCamera(  int pixel_nx, int pixel_ny )
            : Camera( pixel_nx, pixel_ny ) {}

        PerspectiveCamera( vec3 origin, vec3 viewDir, float fLength, float imgPlaneWidth )
        {
            position = origin;
            W = unit_vector(-viewDir);
            if(std::abs(W.y()) > 0.999f) {
                // If the view direction is nearly parallel to the world up vector, use a different up vector to avoid numerical instability
                U = unit_vector(cross(vec3(0, 0, 1), W));
            } else {
                U = unit_vector(cross(vec3(0, 1, 0), W));
            }
            V = cross( W, U );

            focalLength = fLength;
            imageplaneWidth = imgPlaneWidth;
        }

        PerspectiveCamera( vec3 origin, vec3 viewDir, vec3 upDir,
                           float fLength, float imgPlaneWidth, float imgPlaneHeight,
                           int pixel_nx, int pixel_ny )
            : Camera( pixel_nx, pixel_ny )
        {
            position = origin;
            vec3 up = unit_vector(upDir);
            W = unit_vector(-viewDir);
            U = unit_vector(cross(up, W));
            V = cross( W, U );

            focalLength = fLength;
            imageplaneWidth = imgPlaneWidth;
            imageplaneHeight = imgPlaneHeight;
        }

        Ray generateRay( float i, float j ) override {
            // `i` and `j` are continuous sample positions in pixel coordinates
            // (e.g. i = pixel_x + subpixel_offset). Map them to the image
            // plane by dividing by the resolution (nx, ny).
            float u = ( (i) / nx - 0.5f ) * imageplaneWidth;
            float v = ( (j) / ny - 0.5f ) * imageplaneHeight;

            return Ray(position, -W*focalLength + U*u + V*v);
        }
};