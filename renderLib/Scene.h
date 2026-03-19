#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "Shape.h"
#include "HitRecord.h"
#include "Camera.h"
#include "Shader.h"
#include "BVHNode.h"

struct PointLight {
    vec3 position;
    vec3 color;

    PointLight(const vec3& pos, const vec3& col)
        : position(pos), color(col) {}
};

class Scene  {
    public:
        std::vector<std::shared_ptr<Camera>> cameras;
        std::vector<std::shared_ptr<PointLight>> lights;
        std::vector<std::shared_ptr<Shape>> objects;
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaderRegistry;
        std::shared_ptr<Shape> bvhRoot = nullptr; 

        Scene() = default;

        Scene(const std::shared_ptr<Camera>& camera, const std::shared_ptr<PointLight>& light) {
            cameras.push_back(camera);
            lights.push_back(light);
        }

        void addObject(const std::shared_ptr<Shape>& obj) {
            objects.push_back(obj);
        }

        void addLight(const std::shared_ptr<PointLight>& light) {
            lights.push_back(light);
        }

        void addCamera(const std::shared_ptr<Camera>& cam) {
            cameras.push_back(cam);
        }

        void buildBVH() {
            if (!objects.empty()) {
                bvhRoot = std::make_shared<BVHNode>(objects, 0, objects.size());
            }
        }

        bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
            if (!bvhRoot) return false;

            bool hitAnything = bvhRoot->intersect(r, tMin, tMax, rec);

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
                for(const auto& light : lights) {
                    if(isInShadow(rec.point, light)) {
                        rec.inShadow = true;
                        break;
                    }
                }
                return rec.shader->rayColor(rec, *this, depth - 1);
            }
            // Background color
            return vec3(0.529f, 0.808f, 0.922f); // Light blue
        }

        bool isInShadow(const vec3& point, const std::shared_ptr<PointLight>& light) const {
            vec3 lightDir = unit_vector(light->position - point);
            float maxDist = (light->position - point).length();

            Ray shadowRay(point + 0.001f * lightDir, lightDir);
            HitRecord tempRec;

            if (!bvhRoot) return false;

            return bvhRoot->intersect(shadowRay, 0.001f, maxDist, tempRec);
        }
};
