#pragma once
#include <vector>
#include <memory>
#include "Shape.h"
#include "hit_record.h"
#include "Camera.h"

struct PointLight {
    vec3 position;
    vec3 color;

    PointLight(const vec3& pos, const vec3& col)
        : position(pos), color(col) {}
};

class Scene {
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
                // Fill shading info directly into HitRecord
                rec.viewDir    = unit_vector(camera.getPos() - rec.point);
                rec.lightPos   = light.position;
                rec.lightColor = light.color;
            }
            return hitAnything;
    }

    bool isInShadow(const vec3& point, const std::shared_ptr<Shape>& ignore) const {
            vec3 lightDir = unit_vector(light.position - point);
            Ray shadowRay(point + 0.001f * lightDir, lightDir); 
            float maxDist = (light.position - point).length();
            
            HitRecord tempRec;

            for (const auto& obj : objects) {
                if (obj == ignore) continue;
                if (obj->intersect(shadowRay, 0.001f, maxDist, tempRec)) {
                    return true; 
                }
            }
            return false; 
        }
};