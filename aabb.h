#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>
#include "ray.h"
#include "intersection_record.h"

class AABB
{
    public:
        AABB();
        AABB(const glm::vec3 &min, const glm::vec3 &max);
        ~AABB();

        float GetArea();
        bool intersect( const Ray &ray) const;

        AABB operator+(const AABB &rhs) const
        { return AABB( glm::min(this->min_, rhs.min_), glm::max(this->max_, rhs.max_) ); }
        
        glm::vec3 min_;
        glm::vec3 max_;
        glm::vec3 centroid_;

};



#endif //AABB_H