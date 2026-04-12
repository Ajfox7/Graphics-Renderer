#include "Shape.h"
#include "Triangle.h"
#include "BVHNode.h"
#include <vector>
#include <memory>

class TriangleMesh : public Shape {
    public:
        TriangleMesh(const std::vector<float>& vertexData,
                     const std::vector<unsigned int>& indexData,
                     std::shared_ptr<Shader> s )
                     : host_vertices(vertexData), host_indices(indexData), shader(s) 
        {
            std::vector<std::shared_ptr<Shape>> triangles;

            // stride = 6 (3 for vertex position, 3 for normal)
            for(size_t i = 0; i < indexData.size(); i += 3) {
                unsigned int idx0 = indexData[i] * 6;
                unsigned int idx1 = indexData[i + 1] * 6;
                unsigned int idx2 = indexData[i + 2] * 6;

                vec3 v0(vertexData[idx0], vertexData[idx0 + 1], vertexData[idx0 + 2]);
                vec3 v1(vertexData[idx1], vertexData[idx1 + 1], vertexData[idx1 + 2]);
                vec3 v2(vertexData[idx2], vertexData[idx2 + 1], vertexData[idx2 + 2]);

                triangles.push_back(std::make_shared<Triangle>(v0, v1, v2, s));
            }

            bvhRoot = std::make_unique<BVHNode>(triangles, 0, triangles.size());
            setBoundingBox(bvhRoot->getBoundingBox());
        }

        bool intersect(const Ray& r,const float tmin, float& tmax, HitRecord& rec) const override {
            return bvhRoot->intersect(r, tmin, tmax, rec);
        }

        void getMeshData(std::vector<float>& vertices, std::vector<unsigned int>& indices, int subdivisions = 20) const override {
            vertices = host_vertices;
            indices = host_indices;
        }

    private:
        std::vector<float> host_vertices;
        std::vector<unsigned int> host_indices;
        std::shared_ptr<Shader> shader;
        std::unique_ptr<BVHNode> bvhRoot;
};