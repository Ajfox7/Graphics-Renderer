#pragma once
#include <memory>
#include "Ray.h"
#include "HitRecord.h"
#include "BoundingBox.h"

class Shape{
    public:
        virtual ~Shape() = default;

        virtual bool intersect(const Ray& r,const float tmin, float& tmax, HitRecord& rec) const = 0;

        const BoundingBox& getBoundingBox() const {
            return *bbox;
        }
        
    protected:
        void setBoundingBox(const BoundingBox& box) {
            bbox = std::make_shared<BoundingBox>(box);
        }
    
    private:
        std::shared_ptr<BoundingBox> bbox;
};

