#pragma once
#include <vector>
#include <memory>
#include "Shape.h"
#include "HitRecord.h"
#include "Camera.h"
#include "Shader.h"

struct PointLight {
    vec3 position;
    vec3 color;

    PointLight(const vec3& pos, const vec3& col)
        : position(pos), color(col) {}
};

class Scene  {
    public:
        Camera& camera;
        PointLight light;
        std::vector<std::shared_ptr<Shape>> objects;

        Scene(Camera& cam, const PointLight& light)
        : camera(cam), light(light) {}

        void addObject(const std::shared_ptr<Shape>& obj) {
            objects.push_back(obj);
        }

        bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
            HitRecord tempRec;
            bool hitAnything = false;
            float closestSoFar = tMax;

            for (const auto& obj : objects) {
                if (obj->intersect(r, tMin, closestSoFar, tempRec)) {
                    hitAnything = true;
                    closestSoFar = tempRec.t;
                    rec = tempRec;
                }
            }

            if (hitAnything) {
                // Direction from hit point back toward the ray origin
                rec.viewDir  = unit_vector(-r.direction());
            }
            return hitAnything;
        }

        vec3 computeRayColor(const Ray& r, int depth) const {
            if(depth <= 0) {
                return vec3(0, 0, 0); // No more light is gathered
            }
            HitRecord rec;
            
            if (hit(r, 1.0f, FLT_MAX, rec)) {
                rec.inShadow = isInShadow(rec.point);
                return rec.shader->rayColor(rec, *this, depth - 1);
            }
            // Background color
            return vec3(0.529f, 0.808f, 0.922f); // Light blue
        }

        bool isInShadow(const vec3& point) const {
            vec3 lightDir = unit_vector(light.position - point);
            Ray shadowRay(point + 0.001f * lightDir, lightDir); 
            float maxDist = (light.position - point).length();
            
            HitRecord tempRec;

            for (const auto& obj : objects) {
                if (obj->intersect(shadowRay, 0.001f, maxDist, tempRec)) {
                    return true; 
                }
            }
            return false; 
        }
};