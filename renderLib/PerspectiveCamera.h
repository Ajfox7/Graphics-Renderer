#pragma once

#include "Camera.h"
#include "vec3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

        glm::mat4 getViewMatrix() const {
            glm::mat4 M(1.0f);

            M[0][0] = U.x();  M[1][0] = U.y();  M[2][0] = U.z();
            M[0][1] = V.x();  M[1][1] = V.y();  M[2][1] = V.z();
            M[0][2] = W.x();  M[1][2] = W.y();  M[2][2] = W.z();

            M[3][0] = -dot(U, position);
            M[3][1] = -dot(V, position);
            M[3][2] = -dot(W, position);

            return M;
        }

        glm::mat4 getProjectionMatrix(float nearPlane, float farPlane) const {
            float aspect = imageplaneWidth / imageplaneHeight;
            float fovY = 2.0f * std::atan(imageplaneHeight / (2.0f * focalLength));
            return glm::perspective(fovY, aspect, nearPlane, farPlane);
        }

        void moveForward(float amount) {
            position = position + -W * amount;
        }

        void moveBackward(float amount) {
            position = position + W * amount;
        }

        void moveRight(float amount) {
            position = position + U * amount;
        }

        void moveLeft(float amount) {
            position = position - U * amount;
        }

        void moveUp(float amount) {
            position = position + V * amount;
        }

        void moveDown(float amount) {
            position = position - V * amount;
        }
};