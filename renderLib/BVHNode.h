#pragma once

#include "Shape.h"
#include "BoundingBox.h"
#include <memory>
#include <vector>
#include <algorithm>

class BVHNode : public Shape {
    public:
        BVHNode(std::vector<std::shared_ptr<Shape>>& objects, size_t start, size_t end, int axis = 0){
            size_t span = end - start;

            if (span == 1) {
                left  = objects[start];
                right = objects[start];
                setBoundingBox(objects[start]->getBoundingBox());
                return;
            }

            if (span == 2) {
                auto& a = objects[start];
                auto& b = objects[start + 1];

                if (boxCentroid(a->getBoundingBox(), axis) < boxCentroid(b->getBoundingBox(), axis)) {
                    left  = a;
                    right = b;
                } else {
                    left  = b;
                    right = a;
                }

                BoundingBox merged = left->getBoundingBox().merge(right->getBoundingBox());
                setBoundingBox(merged);
                return;
            }

            auto comparator = [axis](const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b) {
                return boxCentroid(a->getBoundingBox(), axis) < boxCentroid(b->getBoundingBox(), axis);
            };

            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            size_t mid = start + span / 2;
            int nextAxis = (axis + 1) % 3;

            left  = std::make_shared<BVHNode>(objects, start, mid, nextAxis);
            right = std::make_shared<BVHNode>(objects, mid, end, nextAxis);

            BoundingBox merged = left->getBoundingBox().merge(right->getBoundingBox());
            setBoundingBox(merged);
        }

        bool intersect(const Ray& r, float tmin, float& tmax, HitRecord& rec) const override {
            const BoundingBox& box = getBoundingBox();
            if (!box.hit(r, tmin, tmax))
                return false;

            HitRecord leftRec, rightRec;

            bool hitLeft  = left->intersect(r, tmin, tmax, leftRec);
            bool hitRight = right->intersect(r, tmin, hitLeft ? leftRec.t : tmax, rightRec);

            if (hitLeft && hitRight) {
                rec = (leftRec.t < rightRec.t) ? leftRec : rightRec;
            } else if (hitLeft) {
                rec = leftRec;
            } else if (hitRight) {
                rec = rightRec;
            }

            return hitLeft || hitRight;
        }

        void getMeshData(std::vector<float>& vertices, std::vector<unsigned int>& indices, int subdivisions = 20) const override {
            left->getMeshData(vertices, indices, subdivisions);
            right->getMeshData(vertices, indices, subdivisions);
        }

    private:
        static float boxCentroid(const BoundingBox& box, int axis) {
            return 0.5f * (box.llb[axis] + box.urf[axis]);
        }

        std::shared_ptr<Shape> left;
        std::shared_ptr<Shape> right;
};