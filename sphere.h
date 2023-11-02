#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include "primitive.h"
#include "intersection_record.h"
#include "ray.h"

class Sphere : public Primitive
{
    public:

        Sphere( void );

        Sphere( const glm::vec3 &center,
                float radius);

        Sphere( const glm::vec3 &center,
                float radius,
                const glm::vec3 &color);

        Sphere( const glm::vec3 &center,
                float radius,
                const glm::vec3 &color,
                const Material &material);


        bool intersect( const Ray &ray,
                        IntersectionRecord &intersection_record) const;

        glm::vec3 center_ = { 0.0f, 0.0f, 0.0f };

        float radius_= 1.0f;

        AABB GetAABB() const;

    private:

        static const float kIntersectionTestEpsilon_;
};

#endif //SPHERE_H

